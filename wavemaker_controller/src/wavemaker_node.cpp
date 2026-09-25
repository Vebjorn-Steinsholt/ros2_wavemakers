#include <memory>
#include <string>
#include <cstdint>
#include <exception>
#include <mutex>
#include <thread>
#include <atomic>

#include "lifecycle_msgs/msg/transition.hpp"
#include "lifecycle_msgs/msg/state.hpp"
#include "rclcpp/rclcpp.hpp"
#include "rclcpp_lifecycle/lifecycle_node.hpp"
#include "rclcpp_action/rclcpp_action.hpp"
#include "bondcpp/bond.hpp"
#include "wavemaker_interfaces/action/move_wavemaker.hpp"
#include "std_msgs/msg/float64.hpp"
#include "indradrive_actuator.hpp"

using MoveWavemaker = wavemaker_interfaces::action::MoveWavemaker;
using MoveWavemakerGoalHandle = rclcpp_action::ServerGoalHandle<MoveWavemaker>;



using CallbackReturn = rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn;

class WavemakerNode final : public rclcpp_lifecycle::LifecycleNode
{
public:
  explicit WavemakerNode(const rclcpp::NodeOptions & options = rclcpp::NodeOptions())
  : LifecycleNode("controller", options)
  {
      declare_parameter<std::string>("wavemaker_type", "piston");
      declare_parameter<std::string>("driver_address", "");
      declare_parameter<std::string>("wavemaker_id", "");
      declare_parameter<double>("wavemaker_minimum", 0.0);
      declare_parameter<double>("wavemaker_maximum", 1.0);
      declare_parameter<double>("water_depth", 0.6);
      declare_parameter<bool>("wavemaker_mode_pregenerated", false);
      declare_parameter<double>("flap_attachment_height", 0.0);

    
  }

  ~WavemakerNode() override
  {
    abort_active_goal();
    stop_execution();
  }

  CallbackReturn on_configure(const rclcpp_lifecycle::State & previous_state) override
  {
    wavemaker_type_ = get_parameter("wavemaker_type").as_string();
    driver_address_ = get_parameter("driver_address").as_string();
    wavemaker_id_ = get_parameter("wavemaker_id").as_string();
    wavemaker_minimum_ = get_parameter("wavemaker_minimum").as_double();
    wavemaker_maximum_ = get_parameter("wavemaker_maximum").as_double();
    wavemaker_mode_pregenerated_ = get_parameter("wavemaker_mode_pregenerated").as_bool();
    water_depth_ = get_parameter("water_depth").as_double();
    flap_attachment_height_ = get_parameter("flap_attachment_height").as_double();

    if (wavemaker_minimum_ >= wavemaker_maximum_) {
      RCLCPP_ERROR(
        get_logger(), "wavemaker_minimum must be less than wavemaker_maximum");
      return CallbackReturn::FAILURE;
    }
    wavemaker_position_offset_ = (wavemaker_minimum_ + wavemaker_maximum_) / 2.0;
    
    if (wavemaker_type_ == "flap" && flap_attachment_height_ <= 0.0) {
      RCLCPP_ERROR(get_logger(), "flap_attachment_height must be set (> 0) for flap-type wavemakers");
      return CallbackReturn::FAILURE;
    }
    
    try {
      actuator_ = std::make_unique<wavemaker_controller::IndraDriveActuator>(*this);
    } catch (const std::exception & error) {
      RCLCPP_ERROR(get_logger(), "Failed to configure actuator: %s", error.what());
      return CallbackReturn::FAILURE;
    }
    action_callback_group_ = create_callback_group(rclcpp::CallbackGroupType::MutuallyExclusive);
    action_server_ = rclcpp_action::create_server<MoveWavemaker>(
  shared_from_this(),
  "move_wavemaker",
  std::bind(&WavemakerNode::goal_callback, this, std::placeholders::_1, std::placeholders::_2),
  std::bind(&WavemakerNode::cancel_callback, this, std::placeholders::_1),
  std::bind(&WavemakerNode::handle_accepted_callback, this, std::placeholders::_1),
  rcl_action_server_get_default_options(),
  action_callback_group_);

  velocity_publisher_ = create_publisher<std_msgs::msg::Float64>("wavemaker_velocity", 10);

    RCLCPP_INFO(
      get_logger(), "Configuring from state '%s' as a %s wavemaker",
      previous_state.label().c_str(), wavemaker_type_.c_str());

    return CallbackReturn::SUCCESS;
  }

  CallbackReturn on_activate(const rclcpp_lifecycle::State & previous_state) override
  {
    RCLCPP_INFO(
      get_logger(), "Activating from state '%s'", previous_state.label().c_str());
        bond_ = std::make_unique<bond::Bond>(
    "bond", get_name(), shared_from_this());
    bond_->setHeartbeatPeriod(0.10);
    bond_->setHeartbeatTimeout(4.0);
    bond_->start();
  
    if (!actuator_ || !actuator_->start(get_logger())) {
      return CallbackReturn::FAILURE;
    }

    return CallbackReturn::SUCCESS;
  }

  CallbackReturn on_deactivate(const rclcpp_lifecycle::State & previous_state) override
  {
    RCLCPP_INFO(
      get_logger(), "Deactivating from state '%s'", previous_state.label().c_str());

      
      abort_active_goal();
      stop_execution();
      if (actuator_) {
        actuator_->stop();
      }
      if(bond_) {
        bond_->breakBond();
        bond_.reset();
      }
    return CallbackReturn::SUCCESS;
  }

  CallbackReturn on_cleanup(const rclcpp_lifecycle::State & previous_state) override
  {
    RCLCPP_INFO(
      get_logger(), "Cleaning up from state '%s'", previous_state.label().c_str());
      abort_active_goal();
      stop_execution();

    // --- driver comms: full teardown ---
    // driver_->close()
    // driver_.reset()
    // ------------------------------------
    actuator_.reset();

    return CallbackReturn::SUCCESS;
  }

private:
  void stop_execution()
  {
    stop_execution_.store(true);

    if (execution_thread_.joinable()) {
      execution_thread_.join();
    }
  }

  void abort_active_goal()
  {
    std::shared_ptr<MoveWavemakerGoalHandle> goal_to_abort;
    {
      std::lock_guard<std::mutex> lock(goal_mutex_);
      goal_to_abort = goal_handle_;
      goal_handle_.reset();
      goal_pending_ = false;
    }

    if (goal_to_abort && goal_to_abort->is_active()) {
      auto result = std::make_shared<MoveWavemaker::Result>();
      result->success = false;
      result->message = "Goal aborted by lifecycle transition";
      goal_to_abort->abort(result);
    }
  }

  rclcpp::Publisher<std_msgs::msg::Float64>::SharedPtr velocity_publisher_;
  std::unique_ptr<bond::Bond> bond_;
  // called periodically while active, or driven by an incoming action goal

  rclcpp_action::GoalResponse goal_callback(
    const rclcpp_action::GoalUUID & uuid,
    std::shared_ptr<const MoveWavemaker::Goal> goal)
  {
    (void)uuid;
    std::lock_guard<std::mutex> lock(goal_mutex_);
   

    if (goal->amplitude <= 0.0 || goal->period <= 0.0) {
      RCLCPP_WARN(get_logger(), "Received goal with non-positive amplitude or period, rejecting");
      return rclcpp_action::GoalResponse::REJECT;
    }

    double omega = 2.0 * M_PI / goal->period;

    // goal_callback — replace the x_max block
    double mu = solve_dispersion(omega, water_depth_);
    double S = compute_stroke(mu, (goal->amplitude) * 2, wavemaker_type_);
    double actuator_amplitude = (S / 2.0);
    if (wavemaker_type_ == "flap") {
      actuator_amplitude *= (flap_attachment_height_ / water_depth_);
    }
    const double required_minimum = wavemaker_position_offset_ - actuator_amplitude;
    const double required_maximum = wavemaker_position_offset_ + actuator_amplitude;
    if (required_minimum < wavemaker_minimum_ || required_maximum > wavemaker_maximum_) {
      RCLCPP_WARN(
        get_logger(),
        "Received goal requiring position range [%f, %f] outside wavemaker limits [%f, %f], rejecting",
        required_minimum, required_maximum, wavemaker_minimum_, wavemaker_maximum_);
              return rclcpp_action::GoalResponse::REJECT;
            }
            goal_pending_ = true;
            actuator_amplitude_ = actuator_amplitude;
            omega_ = omega;


    return rclcpp_action::GoalResponse::ACCEPT_AND_DEFER;
  }

rclcpp_action::CancelResponse cancel_callback(
const std::shared_ptr<MoveWavemakerGoalHandle> goal_handle)
  {
    (void)goal_handle;
    RCLCPP_INFO(get_logger(), "Received request to cancel goal");
    return rclcpp_action::CancelResponse::ACCEPT;
  }

  void handle_accepted_callback(
    std::shared_ptr<MoveWavemakerGoalHandle> goal_handle)
  {
    {
      std::lock_guard<std::mutex> lock(goal_mutex_);
      goal_handle_ = goal_handle;
      goal_pending_ = false;
    }

    stop_execution();
    stop_execution_.store(false);
    execution_thread_ = std::thread([this, goal_handle]() {
      execute_goal(goal_handle);
    });
  }

void execute_goal(
    std::shared_ptr<MoveWavemakerGoalHandle> goal_handle)
{
    auto result = std::make_shared<MoveWavemaker::Result>();
    auto feedback = std::make_shared<MoveWavemaker::Feedback>();

    goal_handle->execute();
    const auto start_time = std::chrono::steady_clock::now();
    rclcpp::Rate loop_rate(100);  // 100 Hz control loop

    while (rclcpp::ok() && !stop_execution_.load()) {

        // Check for cancellation
        if (goal_handle->is_canceling()) {
            goal_handle->canceled(result);

            {
                std::lock_guard<std::mutex> lock(goal_mutex_);
                goal_handle_.reset();
                goal_pending_ = false;
            }

            return;
        }

        const double t = std::chrono::duration<double>(std::chrono::steady_clock::now() - start_time).count();
        double x, v;
        if (wavemaker_mode_pregenerated_) {
            const double a = goal_handle->get_goal()->amplitude;
            const double omega = 2.0 * M_PI / goal_handle->get_goal()->period;
            x = wavemaker_position_offset_ + a * std::sin(omega * t);
            v = a * omega * std::cos(omega * t);
        } else {
            double actuator_amplitude = actuator_amplitude_;
            double omega = omega_;
          x = wavemaker_position_offset_ + actuator_amplitude * std::sin(omega * t);
          v = actuator_amplitude * omega * std::cos(omega * t);
        }

        const auto setpoint = actuator_->to_actuator_setpoint(x, v);

        publish_and_write_setpoint(setpoint.position, setpoint.velocity);

        feedback->desired_position = x;
        feedback->actual_position = actuator_->actual_position_m();
        feedback->elapsed_time = t;
        goal_handle->publish_feedback(feedback);

        loop_rate.sleep();
    }
     {
        std::lock_guard<std::mutex> lock(goal_mutex_);

        if (goal_handle_ == goal_handle) {
            goal_handle_.reset();
            goal_pending_ = false;
        }
    }
    
}

  void publish_and_write_setpoint(double position, double velocity)
  {
      (void)position;
        auto velocity_msg = std_msgs::msg::Float64();
        velocity_msg.data = velocity;
        velocity_publisher_->publish(velocity_msg);
        // setpoint = compute_from_current_goal()   // amplitude/period/ramp logic
        // driver_->write_setpoint(setpoint)         // blocking or async, depends on transport
        // status = driver_->read_status()           // position, fault flags, etc.
        // publish_feedback(status)                  // to action feedback / state topic
  }

  void handle_driver_fault(/* FaultCode code */)
  {
    // log fault
    // trigger_transition(TRANSITION_DEACTIVATE) or TRANSITION_TO_ERROR
    // abort any in-progress action goal
  }

  double solve_dispersion(double omega,double h, double g=9.81){
   //Beji 2013 improved dispersion relation solver
    const double mu0 = (omega*omega*h)/g;
    //Eckart 1952 approximation
    const double mu_a = mu0/std::sqrt(std::tanh(mu0));

    //Beji 2013 correction terms
    constexpr double alpha = 1.09;
    constexpr double beta0 = 1.55;
    constexpr double beta1 = 1.30;
    constexpr double beta2 = 0.216;

    const double fc = std::pow(mu0, alpha) *
                   std::exp(-(beta0 + beta1 * mu0 + beta2 * mu0 * mu0));
    const double mu = mu_a *(1.0+fc);
    return mu;
  }

double compute_stroke(double mu, double target_H, const std::string & type)
{
  double transfer_ratio;  // H/S

  if (type == "piston") {
    transfer_ratio = (4.0 * std::sinh(mu) * std::sinh(mu)) /
                      (std::sinh(2.0 * mu) + 2.0 * mu);
  } else { // "flap"
    double num = mu * std::sinh(mu) - std::cosh(mu) + 1.0;
    transfer_ratio = (4.0 * std::sinh(mu) * num) /
                      (mu * (std::sinh(2.0 * mu) + 2.0 * mu));
  }

  return target_H / transfer_ratio;
}

  std::string wavemaker_type_;
  std::string driver_address_;
  std::string wavemaker_id_;
  double wavemaker_minimum_;
  double wavemaker_maximum_;
  double wavemaker_position_offset_;
  bool wavemaker_mode_pregenerated_;
  double actuator_amplitude_;
  double flap_attachment_height_;
  double water_depth_;
  double omega_;
  std::mutex goal_mutex_;
  std::shared_ptr<MoveWavemakerGoalHandle> goal_handle_;
  bool goal_pending_{false};
  rclcpp_action::Server<MoveWavemaker>::SharedPtr action_server_;
  rclcpp::CallbackGroup::SharedPtr action_callback_group_;
  std::atomic<bool> stop_execution_{false};
  std::thread execution_thread_;
  std::unique_ptr<wavemaker_controller::WavemakerActuator> actuator_;

  // std::unique_ptr<WavemakerDriver> driver_;
  // rclcpp::TimerBase::SharedPtr command_timer_;
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  auto node = std::make_shared<WavemakerNode>();

  rclcpp::executors::MultiThreadedExecutor executor;
  executor.add_node(node->get_node_base_interface());
  executor.spin();

  rclcpp::shutdown();
  return 0;
}


