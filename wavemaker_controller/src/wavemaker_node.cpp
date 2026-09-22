#include <memory>
#include <string>

#include "lifecycle_msgs/msg/transition.hpp"
#include "rclcpp/rclcpp.hpp"
#include "rclcpp_lifecycle/lifecycle_node.hpp"
#include "rclcpp_action/rclcpp_action.hpp"
#include "bondcpp/bond.hpp"
#include "wavemaker_interfaces/action/move_wavemaker.hpp"

using MoveWavemaker = wavemaker_interfaces::action::MoveWavemaker;
using MoveWavemakerGoalHandle = rclcpp_action::ServerGoalHandle<MoveWavemaker>;
using namespace std::placeholders;



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
      declare_parameter<double>("wavemaker_maximum", 1.0);
      declare_parameter<bool>("wavemaker_mode_pregenerated", false);
      server_active_ = false;

    
  }
 

  CallbackReturn on_configure(const rclcpp_lifecycle::State & previous_state) override
  {
    wavemaker_type_ = get_parameter("wavemaker_type").as_string();
    driver_address_ = get_parameter("driver_address").as_string();
    wavemaker_id_ = get_parameter("wavemaker_id").as_string();
    wavemaker_maximum_ = get_parameter("wavemaker_maximum").as_double();
    wavemaker_mode_pregenerated_ = get_parameter("wavemaker_mode_pregenerated").as_bool();
    action_server_ = rclcpp_action::create_server<MoveWavemaker>(
      shared_from_this(),
      "move_wavemaker",
      std::bind(&WavemakerNode::goal_callback, this, std::placeholders::_1, std::placeholders::_2),
      std::bind(&WavemakerNode::cancel_callback, this, std::placeholders::_1),
      std::bind(&WavemakerNode::handle_accepted_callback, this, std::placeholders::_1));
    RCLCPP_INFO(
      get_logger(), "Configuring from state '%s' as a %s wavemaker",
      previous_state.label().c_str(), wavemaker_type_.c_str());



    // --- driver comms: open but do not command motion ---
    // driver_ = DriverFactory::create(wavemaker_type_)
    // if (!driver_->open(driver_address_)) { return CallbackReturn::FAILURE }
    // driver_->set_fault_callback([this](FaultCode code) { handle_driver_fault(code); })
    // optionally: driver_->read_firmware_version() / sanity-check handshake
    // ------------------------------------------------------

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
      
    // --- driver comms: arm actuator, start periodic write loop ---
    // if (!driver_->enable()) { return CallbackReturn::FAILURE }
    // command_timer_ = create_wall_timer(10ms, [this] { publish_and_write_setpoint() })
    // ---------------------------------------------------------------

    return CallbackReturn::SUCCESS;
  }

  CallbackReturn on_deactivate(const rclcpp_lifecycle::State & previous_state) override
  {
    RCLCPP_INFO(
      get_logger(), "Deactivating from state '%s'", previous_state.label().c_str());
      if(bond_) {
        bond_->breakBond();
        bond_.reset();
      }
    // --- driver comms: stop motion, keep connection open ---
    // command_timer_->cancel()
    // driver_->disable()   // e.g. ramp to zero, then hold/disarm
    // ------------------------------------------------------

    return CallbackReturn::SUCCESS;
  }

  CallbackReturn on_cleanup(const rclcpp_lifecycle::State & previous_state) override
  {
    RCLCPP_INFO(
      get_logger(), "Cleaning up from state '%s'", previous_state.label().c_str());

    // --- driver comms: full teardown ---
    // driver_->close()
    // driver_.reset()
    // ------------------------------------

    return CallbackReturn::SUCCESS;
  }

private:
  std::unique_ptr<bond::Bond> bond_;
  // called periodically while active, or driven by an incoming action goal

  rclcpp_action::GoalResponse goal_callback(
    const rclcpp_action::GoalUUID & uuid,
    std::shared_ptr<const MoveWavemaker::Goal> goal)
  {
    if(!server_active_ || goal_active_) {
      RCLCPP_WARN(get_logger(), "Received goal while server inactive, or already active, rejecting");
      return rclcpp_action::GoalResponse::REJECT;
    }
    goal_active_ = true;
    if (goal->amplitude > wavemaker_maximum_) {
      RCLCPP_WARN(get_logger(), "Received goal with amplitude %f exceeding maximum %f, rejecting",
                  goal->amplitude, wavemaker_maximum_);
      return rclcpp_action::GoalResponse::REJECT;
    }


    return rclcpp_action::GoalResponse::ACCEPT_AND_EXECUTE;
  }

  void cancel_callback(
    const rclcpp_action::GoalUUID & uuid)
  {
    goal_active_ = false;
    // Handle cancellation of the goal
  }

  void handle_accepted_callback(
    std::shared_ptr<MoveWavemakerGoalHandle> goal_handle)
  {
    goal_active_ = true;  
    // Handle the accepted goal
  }

  void publish_and_write_setpoint()
  {
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

    const double fc =std::pow(mu0,alpha) * 
                      (std::exp(-beta0+beta1*mu0+beta2*mu0*mu0));
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
  double wavemaker_maximum_;
  bool wavemaker_mode_pregenerated_;
  double mu_;
  double S_;
  bool server_active_;
  std::mutex goal_mutex_;
  std::shared_ptr<MoveWavemakerGoalHandle> goal_handle_;
  rclcpp_action::Server<MoveWavemaker>::SharedPtr action_server_;
  bool goal_active_;

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