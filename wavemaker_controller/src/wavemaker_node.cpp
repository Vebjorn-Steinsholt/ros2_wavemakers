#include <memory>
#include <string>
#include <cstdint>
#include <exception>
#include <functional>
#include <mutex>
#include <thread>
#include <atomic>
#include <chrono>
#include <cmath>
#include <rmw/types.h>

#include "lifecycle_msgs/msg/transition.hpp"
#include "lifecycle_msgs/msg/state.hpp"
#include "rclcpp/rclcpp.hpp"
#include "rclcpp_lifecycle/lifecycle_node.hpp"
#include "rclcpp_action/rclcpp_action.hpp"
#include "bondcpp/bond.hpp"
#include "wavemaker_interfaces/action/move_wavemaker.hpp"
#include "wavemaker_interfaces/srv/cancel_all_goals.hpp"
#include "std_msgs/msg/float64.hpp"
#include "indradrive_actuator.hpp"

using MoveWavemaker = wavemaker_interfaces::action::MoveWavemaker;
using MoveWavemakerGoalHandle = rclcpp_action::ServerGoalHandle<MoveWavemaker>;

using CancelAllGoals = wavemaker_interfaces::srv::CancelAllGoals;



using CallbackReturn = rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn;

class WavemakerNode final : public rclcpp_lifecycle::LifecycleNode
{
public:
  using ActuatorFactory = std::function<std::unique_ptr<wavemaker_controller::WavemakerActuator>(
      rclcpp_lifecycle::LifecycleNode &)>;

  explicit WavemakerNode(
    const rclcpp::NodeOptions & options = rclcpp::NodeOptions(),
    ActuatorFactory actuator_factory = [](rclcpp_lifecycle::LifecycleNode & node) {
      return std::make_unique<wavemaker_controller::IndraDriveActuator>(node);
    })
  : LifecycleNode("controller", options), actuator_factory_(std::move(actuator_factory))
  {
      declare_parameter<std::string>("wavemaker_type", "piston");
      declare_parameter<std::string>("driver_address", "");
      declare_parameter<std::string>("wavemaker_id", "");
      declare_parameter<double>("wavemaker_upright_position_m", 0.0);
      declare_parameter<bool>("wavemaker_upright_is_minimum", false);
      declare_parameter<double>("water_depth", 0.6);
      declare_parameter<bool>("wavemaker_mode_pregenerated", false);
      declare_parameter<double>("flap_attachment_height", 0.0);
      declare_parameter<double>("actuator_upright_angle_deg", 0.0);
      declare_parameter<std::string>("actuator_drive_type", "linear");
      if (get_parameter("actuator_drive_type").as_string() == "linear") {
        declare_parameter<double>("wavemaker_minimum", 0.0);
        declare_parameter<double>("wavemaker_maximum", 1.0);
      }
      declare_parameter<double>("actuator_lead_m_per_degree", 0.0);
      declare_parameter<double>("min_position_deg", -550.0);
      declare_parameter<double>("max_position_deg", 500.0);
      declare_parameter<double>("min_position_m", 0.0);
      declare_parameter<double>("max_position_m", 1.0);

    
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
    wavemaker_position_offset_ = get_parameter("wavemaker_upright_position_m").as_double();
    upright_is_minimum_ = get_parameter("wavemaker_upright_is_minimum").as_bool();
    wavemaker_mode_pregenerated_ = get_parameter("wavemaker_mode_pregenerated").as_bool();
    water_depth_ = get_parameter("water_depth").as_double();
    flap_attachment_height_ = get_parameter("flap_attachment_height").as_double();
    actuator_upright_angle_deg_ = get_parameter("actuator_upright_angle_deg").as_double();
    actuator_drive_type_ = get_parameter("actuator_drive_type").as_string();

    if (!std::isfinite(wavemaker_position_offset_) || !std::isfinite(water_depth_) ||
      !std::isfinite(flap_attachment_height_) || !std::isfinite(actuator_upright_angle_deg_))
    {
      RCLCPP_ERROR(get_logger(), "All wavemaker numeric parameters must be finite");
      return CallbackReturn::FAILURE;
    }

    if (actuator_drive_type_ != "angular" && actuator_drive_type_ != "linear") {
      RCLCPP_ERROR(get_logger(), "actuator_drive_type must be 'linear' or 'angular'");
      return CallbackReturn::FAILURE;
    }

    if (actuator_drive_type_ == "angular") {
      const double lead_m_per_degree =
        get_parameter("actuator_lead_m_per_degree").as_double();
      const double minimum_deg = get_parameter("min_position_deg").as_double();
      const double maximum_deg = get_parameter("max_position_deg").as_double();
      if (!std::isfinite(lead_m_per_degree) || lead_m_per_degree <= 0.0 ||
        !std::isfinite(minimum_deg) || !std::isfinite(maximum_deg) ||
        minimum_deg >= maximum_deg || actuator_upright_angle_deg_ < minimum_deg ||
        actuator_upright_angle_deg_ > maximum_deg)
      {
        RCLCPP_ERROR(get_logger(), "Invalid angular actuator scale, reference, or limits");
        return CallbackReturn::FAILURE;
      }
      goal_position_minimum_ = wavemaker_position_offset_ +
        (minimum_deg - actuator_upright_angle_deg_) * lead_m_per_degree;
      goal_position_maximum_ = wavemaker_position_offset_ +
        (maximum_deg - actuator_upright_angle_deg_) * lead_m_per_degree;
    } else {
      wavemaker_minimum_ = get_parameter("wavemaker_minimum").as_double();
      wavemaker_maximum_ = get_parameter("wavemaker_maximum").as_double();
      if (!std::isfinite(wavemaker_minimum_) || !std::isfinite(wavemaker_maximum_) ||
        wavemaker_minimum_ >= wavemaker_maximum_)
      {
        RCLCPP_ERROR(
          get_logger(), "wavemaker_minimum must be less than wavemaker_maximum");
        return CallbackReturn::FAILURE;
      }
      if (wavemaker_position_offset_ < wavemaker_minimum_ ||
        wavemaker_position_offset_ > wavemaker_maximum_)
      {
        RCLCPP_ERROR(
          get_logger(),
          "wavemaker_upright_position_m must be within wavemaker_minimum and wavemaker_maximum");
        return CallbackReturn::FAILURE;
      }
      goal_position_minimum_ = wavemaker_minimum_;
      goal_position_maximum_ = wavemaker_maximum_;
    }
    if (upright_is_minimum_ &&
      std::abs(wavemaker_position_offset_ - goal_position_minimum_) > 1e-9)
    {
      RCLCPP_ERROR(
        get_logger(), "wavemaker_upright_position_m must equal the minimum when "
        "wavemaker_upright_is_minimum is true");
      return CallbackReturn::FAILURE;
    }
    
    if (wavemaker_type_ == "flap" && (flap_attachment_height_ <= 0.0 || flap_attachment_height_<water_depth_)) {
      RCLCPP_ERROR(get_logger(), "flap_attachment_height must be set (> 0) and greater than water_depth for flap-type wavemakers");
      return CallbackReturn::FAILURE;
    }
    if (wavemaker_type_ != "flap" && wavemaker_type_ != "piston") {
      RCLCPP_ERROR(
        get_logger(), "wavemaker_type must be either 'flap' or 'piston'");
      return CallbackReturn::FAILURE;
    }
    if(water_depth_ <= 0.0) {
      RCLCPP_ERROR(get_logger(), "water_depth must be set (> 0)");
      return CallbackReturn::FAILURE;
    }
    try {
      actuator_ = actuator_factory_(*this);
      actuator_->set_fault_callback(
        [this](const std::string & reason) { handle_driver_fault(reason); });
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

  cancel_service_callback_group_ =
    create_callback_group(rclcpp::CallbackGroupType::MutuallyExclusive);
  
  cancel_client_callback_group_ =
    create_callback_group(rclcpp::CallbackGroupType::Reentrant);
  
  cancel_client_ = rclcpp_action::create_client<MoveWavemaker>(
    shared_from_this(), "move_wavemaker", cancel_client_callback_group_);
  
  cancel_service_ = create_service<CancelAllGoals>(
  "cancel_all_goals",
  [this](
    std::shared_ptr<rclcpp::Service<CancelAllGoals>> service,
    std::shared_ptr<rmw_request_id_t> request_id,
    std::shared_ptr<CancelAllGoals::Request> request)
  {
    auto respond =
      [service, request_id, requester = request->requester](
        uint8_t status, uint32_t count, const std::string & message)
      {
        auto response = std::make_shared<CancelAllGoals::Response>();
        response->status = status;
        response->goals_canceling = count;
        response->requester = requester;
        response->message = message;
        service->send_response(*request_id, *response);
      };

    if (!cancel_client_ || !cancel_client_->action_server_is_ready()) {
      respond(
        CancelAllGoals::Response::SERVER_UNAVAILABLE,
        0, "Action server is unavailable");
      return;
    }

    cancel_client_->async_cancel_all_goals(
      [respond](auto cancel_reply)
      {
        const auto count =
          static_cast<uint32_t>(cancel_reply->goals_canceling.size());

        if (cancel_reply->return_code !=
          action_msgs::srv::CancelGoal::Response::ERROR_NONE)
        {
          respond(
            CancelAllGoals::Response::REQUEST_REJECTED,
            count, "Action server rejected cancellation");
        } else if (count == 0) {
          respond(
            CancelAllGoals::Response::NO_ACTIVE_GOALS,
            0, "No active goals");
        } else {
          respond(
            CancelAllGoals::Response::REQUEST_ACCEPTED,
            count, "Cancellation accepted");
        }
      });
  },
  rclcpp::ServicesQoS(),
  cancel_service_callback_group_);
  
  velocity_publisher_ = create_publisher<std_msgs::msg::Float64>("wavemaker_velocity", 10);
  fault_timer_ = create_wall_timer(
    std::chrono::milliseconds(100),
    [this]() { process_driver_fault(); });

    RCLCPP_INFO(
      get_logger(), "Configuring from state '%s' as a %s wavemaker",
      previous_state.label().c_str(), wavemaker_type_.c_str());

    return CallbackReturn::SUCCESS;
  }

  CallbackReturn on_activate(const rclcpp_lifecycle::State & previous_state) override
  {
    RCLCPP_INFO(
      get_logger(), "Activating from state '%s'", previous_state.label().c_str());

    if (!actuator_ || !actuator_->start(get_logger())) {
      return CallbackReturn::FAILURE;
    }

    bond_ = std::make_unique<bond::Bond>(
      "bond", get_name(), shared_from_this());
    bond_->setHeartbeatPeriod(0.10);
    bond_->setHeartbeatTimeout(4.0);
    bond_->start();

    {
      std::lock_guard<std::mutex> lock(goal_mutex_);
      accepting_goals_ = true;
    }
    return CallbackReturn::SUCCESS;
  }

  CallbackReturn on_deactivate(const rclcpp_lifecycle::State & previous_state) override
  {
    RCLCPP_INFO(
      get_logger(), "Deactivating from state '%s'", previous_state.label().c_str());

      
    {
      std::lock_guard<std::mutex> lock(goal_mutex_);
      accepting_goals_ = false;
    }
      abort_active_goal();
      stop_execution();
      if (actuator_) {
        actuator_->stop();
      }
      if(bond_) {
        bond_->breakBond();
        bond_.reset();
      }
    if (fault_transition_pending_.exchange(false)) {
      return CallbackReturn::ERROR;
    }
    return CallbackReturn::SUCCESS;
  }

  CallbackReturn on_cleanup(const rclcpp_lifecycle::State & previous_state) override
  {
    RCLCPP_INFO(
      get_logger(), "Cleaning up from state '%s'", previous_state.label().c_str());

    {
      std::lock_guard<std::mutex> lock(goal_mutex_);
      accepting_goals_ = false;
    }
   
    abort_active_goal();
    stop_execution();
     if (actuator_) {
      actuator_->stop();
    }
    action_server_.reset();
    action_callback_group_.reset();
    cancel_service_.reset();
    cancel_client_.reset();
    cancel_service_callback_group_.reset();
    cancel_client_callback_group_.reset();
    velocity_publisher_.reset();
    fault_timer_.reset();
   
    actuator_.reset();

    return CallbackReturn::SUCCESS;
  }
  CallbackReturn on_error(const rclcpp_lifecycle::State & previous_state) override
  {
    RCLCPP_ERROR(
      get_logger(), "Error occurred in state '%s'", previous_state.label().c_str());

    {
      std::lock_guard<std::mutex> lock(goal_mutex_);
      accepting_goals_ = false;
    }
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

  CallbackReturn on_shutdown(const rclcpp_lifecycle::State & previous_state) override
  {
    RCLCPP_INFO(
      get_logger(), "Shutting down from state '%s'", previous_state.label().c_str());

    {
      std::lock_guard<std::mutex> lock(goal_mutex_);
      accepting_goals_ = false;
    }
    abort_active_goal();
    stop_execution();
    if (actuator_) {
      actuator_->stop();
    }
    if(bond_) {
      bond_->breakBond();
      bond_.reset();
    }
    action_server_.reset();
    action_callback_group_.reset();
    velocity_publisher_.reset();
    fault_timer_.reset();
    actuator_.reset();

    return CallbackReturn::SUCCESS;
  }

private:
  bool prepare_goal(const MoveWavemaker::Goal & goal)
  {
    if (wavemaker_mode_pregenerated_) {
      if (goal.positions.empty() || !std::isfinite(goal.sample_interval) ||
        goal.sample_interval <= 0.0)
      {
        RCLCPP_WARN(
          get_logger(), "Pregenerated goal requires positions and a positive sample_interval");
        return false;
      }
      for (const double position : goal.positions) {
        if (!std::isfinite(position) || position < goal_position_minimum_ ||
          position > goal_position_maximum_)
        {
          RCLCPP_WARN(
            get_logger(), "Pregenerated position %f is outside configured actuator limits [%f, %f]",
            position, goal_position_minimum_, goal_position_maximum_);
          return false;
        }
      }
      const double duration = goal.sample_interval * 
        static_cast<double>(goal.positions.size() - 1);
      if (!std::isfinite(duration)) {
        RCLCPP_WARN(get_logger(), "Pregenerated trajectory duration is not finite");
        return false;
      }
      return true;
    }

    if (!std::isfinite(goal.amplitude) || !std::isfinite(goal.period) ||
      goal.amplitude <= 0.0 || goal.period <= 0.0)
    {
      RCLCPP_WARN(get_logger(), "Received goal with non-positive amplitude or period, rejecting");
      return false;
    }

    omega_ = 2.0 * M_PI / goal.period;
    const double mu = solve_dispersion(omega_, water_depth_);
    const double stroke = compute_stroke(mu, goal.amplitude * 2, wavemaker_type_);
    actuator_amplitude_ = stroke / 2.0;
    if (wavemaker_type_ == "flap") {
      actuator_amplitude_ *= (flap_attachment_height_ / water_depth_);
    }

    const double required_minimum = upright_is_minimum_ ?
      wavemaker_position_offset_ : wavemaker_position_offset_ - actuator_amplitude_;
    const double required_maximum = wavemaker_position_offset_ +
      actuator_amplitude_ * (upright_is_minimum_ ? 2.0 : 1.0);
    if (required_minimum < goal_position_minimum_ ||
      required_maximum > goal_position_maximum_)
    {
      RCLCPP_WARN(
        get_logger(),
        "Received goal requiring position range [%f, %f] outside configured actuator limits [%f, %f], rejecting",
        required_minimum, required_maximum, goal_position_minimum_, goal_position_maximum_);
      return false;
    }

    trajectory_start_position_ = actuator_->actual_position_m();
    if (!std::isfinite(trajectory_start_position_) ||
      trajectory_start_position_ < goal_position_minimum_ ||
      trajectory_start_position_ > goal_position_maximum_)
    {
      RCLCPP_WARN(
        get_logger(), "Current position %f is outside configured actuator limits [%f, %f]",
        trajectory_start_position_, goal_position_minimum_, goal_position_maximum_);
      return false;
    }
    startup_transition_duration_ = goal.period;
    return true;
  }

  bool sample_goal(
    const MoveWavemaker::Goal & goal, double elapsed, double trajectory_duration,
    double & position, double & velocity) const
  {
    if (!wavemaker_mode_pregenerated_) {
      if (upright_is_minimum_) {
        if (elapsed < startup_transition_duration_) {
          const double u = elapsed / startup_transition_duration_;
          const double u2 = u * u;
          const double u3 = u2 * u;
          const double u4 = u3 * u;
          const double u5 = u4 * u;
          const double blend = 10.0 * u3 - 15.0 * u4 + 6.0 * u5;
          const double blend_velocity =
            (30.0 * u2 - 60.0 * u3 + 30.0 * u4) / startup_transition_duration_;
          const double wave_position = wavemaker_position_offset_ +
            actuator_amplitude_ * (1.0 - std::cos(omega_ * elapsed));
          const double wave_velocity = actuator_amplitude_ * omega_ *
            std::sin(omega_ * elapsed);

          position = trajectory_start_position_ +
            blend * (wave_position - trajectory_start_position_);
          velocity = blend_velocity * (wave_position - trajectory_start_position_) +
            blend * wave_velocity;
        } else {
          const double wave_elapsed = elapsed - startup_transition_duration_;
          position = wavemaker_position_offset_ +
            actuator_amplitude_ * (1.0 - std::cos(omega_ * wave_elapsed));
          velocity = actuator_amplitude_ * omega_ * std::sin(omega_ * wave_elapsed);
        }
        return false;
      }

      const double wave_elapsed = elapsed - startup_transition_duration_;
      const double wave_position = wavemaker_position_offset_ +
        actuator_amplitude_ * std::sin(omega_ * wave_elapsed);
      const double wave_velocity = actuator_amplitude_ * omega_ *
        std::cos(omega_ * wave_elapsed);

      if (elapsed < startup_transition_duration_) {
        const double u = elapsed / startup_transition_duration_;
        const double u2 = u * u;
        const double u3 = u2 * u;
        const double u4 = u3 * u;
        const double u5 = u4 * u;
        const double blend = 10.0 * u3 - 15.0 * u4 + 6.0 * u5;
        const double blend_velocity =
          (30.0 * u2 - 60.0 * u3 + 30.0 * u4) / startup_transition_duration_;

        position = trajectory_start_position_ +
          blend * (wave_position - trajectory_start_position_);
        velocity = blend_velocity * (wave_position - trajectory_start_position_) +
          blend * wave_velocity;
      } else {
        position = wave_position;
        velocity = wave_velocity;
      }
      return false;
    }

    if (elapsed >= trajectory_duration) {
      position = goal.positions.back();
      velocity = 0.0;
      return true;
    }

    const double sample = elapsed / goal.sample_interval;
    const auto index = static_cast<std::size_t>(sample);
    const double fraction = sample - static_cast<double>(index);
    const double delta = goal.positions[index + 1] - goal.positions[index];
    position = goal.positions[index] + fraction * delta;
    velocity = delta / goal.sample_interval;
    return false;
  }

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
    const auto current_state = get_current_state();
    
    if (!accepting_goals_ || current_state.id() != lifecycle_msgs::msg::State::PRIMARY_STATE_ACTIVE)  {
        RCLCPP_WARN(
    get_logger(),
    "Rejecting goal because the node is not accepting goals. State: %s",
    current_state.label().c_str());
      return rclcpp_action::GoalResponse::REJECT;
    }

    if (goal_pending_ || (goal_handle_ && goal_handle_->is_active())) {
      RCLCPP_WARN(get_logger(), "Rejecting goal because another goal is active");
      return rclcpp_action::GoalResponse::REJECT;
    }

    if (!prepare_goal(*goal)) {
      return rclcpp_action::GoalResponse::REJECT;
    }

    goal_pending_ = true;


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
    bool can_start = false;

    {
      std::lock_guard<std::mutex> lock(goal_mutex_);

      if (accepting_goals_ &&
          get_current_state().id() ==
            lifecycle_msgs::msg::State::PRIMARY_STATE_ACTIVE) {
        goal_handle_ = goal_handle;
        goal_pending_ = false;
        can_start = true;
      }
    }

    if (!can_start) {
      auto result = std::make_shared<MoveWavemaker::Result>();
      result->success = false;
      result->message = "Goal rejected because the node is no longer active";
      goal_handle->abort(result);
      return;
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
    const auto goal = goal_handle->get_goal();
    const double trajectory_duration = wavemaker_mode_pregenerated_ ?
      goal->sample_interval * static_cast<double>(goal->positions.size() - 1) : 0.0;

    goal_handle->execute();
    const auto start_time = std::chrono::steady_clock::now();
    rclcpp::Rate loop_rate(100);  // 100 Hz control loop

    while (rclcpp::ok() && !stop_execution_.load()) {

        // Check for cancellation
        if (goal_handle->is_canceling()) {
            actuator_->halt();

            {
                std::lock_guard<std::mutex> lock(goal_mutex_);
              if (goal_handle_ == goal_handle) {
                goal_handle_.reset();
                goal_pending_ = false;
              }
            }

            result->success = false;
            result->message = "Goal canceled; actuator halted and remains enabled";
            goal_handle->canceled(result);
            return;
        }

        const double t = std::chrono::duration<double>(std::chrono::steady_clock::now() - start_time).count();
        double x, v;
        const bool trajectory_complete = sample_goal(*goal, t, trajectory_duration, x, v);

        const auto setpoint = actuator_->to_actuator_setpoint(x, v);

        if (!publish_and_write_setpoint(setpoint.position, setpoint.velocity)) {
          RCLCPP_ERROR(get_logger(), "Failed to write actuator setpoint");
          auto failure = std::make_shared<MoveWavemaker::Result>();
          failure->success = false;
          const std::string reason = actuator_->fault_reason();
          failure->message = reason.empty() ?
            "Actuator rejected setpoint" : "Actuator rejected setpoint: " + reason;
          RCLCPP_ERROR(get_logger(), "%s", failure->message.c_str());
          goal_handle->abort(failure);
          actuator_->stop();
          {
            std::lock_guard<std::mutex> lock(goal_mutex_);
            if (goal_handle_ == goal_handle) {
              goal_handle_.reset();
              goal_pending_ = false;
            }
          }
          return;
        }

        feedback->desired_position = x;
        feedback->actual_position = actuator_->actual_position_m();
        feedback->elapsed_time = t;
        goal_handle->publish_feedback(feedback);

        if (trajectory_complete) {
          actuator_->halt();
          result->success = true;
          result->message = "Pregenerated trajectory completed";
          goal_handle->succeed(result);
          std::lock_guard<std::mutex> lock(goal_mutex_);
          if (goal_handle_ == goal_handle) {
            goal_handle_.reset();
            goal_pending_ = false;
          }
          return;
        }

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

  bool publish_and_write_setpoint(double position, double velocity)
  {
    auto velocity_msg = std_msgs::msg::Float64();
    velocity_msg.data = velocity;
    velocity_publisher_->publish(velocity_msg);
    return actuator_->write_setpoint({position, velocity});
  }

  void handle_driver_fault(const std::string & reason)
  {
    {
      std::lock_guard<std::mutex> lock(fault_mutex_);
      if (!fault_pending_.load()) {
        pending_fault_reason_ = reason;
      }
    }
    fault_pending_.store(true);
  }

  void process_driver_fault()
  {
    if (!fault_pending_.exchange(false)) {
      return;
    }

    std::string reason;
    {
      std::lock_guard<std::mutex> lock(fault_mutex_);
      reason = pending_fault_reason_;
      pending_fault_reason_.clear();
    }

    if (get_current_state().id() != lifecycle_msgs::msg::State::PRIMARY_STATE_ACTIVE) {
      return;
    }

    RCLCPP_ERROR(get_logger(), "Driver fault: %s", reason.c_str());
    abort_active_goal();
    fault_transition_pending_.store(true);
    trigger_transition(lifecycle_msgs::msg::Transition::TRANSITION_DEACTIVATE);
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
  double goal_position_minimum_;
  double goal_position_maximum_;
  bool upright_is_minimum_{false};
  double trajectory_start_position_{0.0};
  double startup_transition_duration_{0.0};
  double wavemaker_position_offset_;
  bool wavemaker_mode_pregenerated_;
  double actuator_amplitude_;
  double flap_attachment_height_;
  double water_depth_;
  double actuator_upright_angle_deg_;
  std::string actuator_drive_type_;
  double omega_;
  std::mutex goal_mutex_;
  std::shared_ptr<MoveWavemakerGoalHandle> goal_handle_;
  bool goal_pending_{false};
  rclcpp_action::Server<MoveWavemaker>::SharedPtr action_server_;
  rclcpp::CallbackGroup::SharedPtr cancel_service_callback_group_;
  rclcpp::CallbackGroup::SharedPtr cancel_client_callback_group_;
  rclcpp_action::Client<MoveWavemaker>::SharedPtr cancel_client_;
  rclcpp::Service<CancelAllGoals>::SharedPtr cancel_service_;  rclcpp::CallbackGroup::SharedPtr action_callback_group_;
  rclcpp::TimerBase::SharedPtr fault_timer_;
  std::atomic<bool> stop_execution_{false};
  std::atomic<bool> fault_pending_{false};
  std::atomic<bool> fault_transition_pending_{false};
  std::mutex fault_mutex_;
  std::string pending_fault_reason_;
  bool accepting_goals_{false};
  std::thread execution_thread_;
  ActuatorFactory actuator_factory_;
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


