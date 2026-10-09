#include "rclcpp/rclcpp.hpp"
#include "rclcpp_lifecycle/lifecycle_node.hpp"
#include "rclcpp_action/rclcpp_action.hpp"
#include "bondcpp/bond.hpp"
#include <cstdio>
#include <functional>
#include <memory>
#include <string>
#include <chrono>
#include <utility>
#include <cmath>
#include "std_msgs/msg/float64.hpp"
#include "std_msgs/msg/bool.hpp"
#include "wavemaker_interfaces/action/move_wavemaker.hpp"
#include "std_srvs/srv/trigger.hpp"
#include "std_msgs/msg/string.hpp"

using rclcpp_lifecycle::LifecycleNode;
using MoveWavemaker = wavemaker_interfaces::action::MoveWavemaker;
using StopDone = std::function<void(bool, const std::string &)>;
using MoveWavemakerGoalHandle = rclcpp_action::ClientGoalHandle<MoveWavemaker>;
using CallbackReturn = rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn;

class WavemakerBridge : public LifecycleNode
{
private:
  enum class State { Inactive, Idle, Starting, Running, Stopping };

  static const char * state_name(State state)
  {
    switch (state) {
      case State::Inactive: return "inactive";
      case State::Idle: return "idle";
      case State::Starting: return "starting";
      case State::Running: return "running";
      case State::Stopping: return "stopping";
    }
    return "unknown";
  }

public:
  explicit WavemakerBridge(const rclcpp::NodeOptions & options)
  : LifecycleNode("wavemaker_bridge", options)
  {
    // Must match the lifecycle manager's bond_timeout; set by the launch file.
    declare_parameter<double>("bond_timeout", 4.0);
    declare_parameter<double>("heartbeat_timeout", 1.0);
  }

  CallbackReturn on_configure(const rclcpp_lifecycle::State &) override
  {
    RCLCPP_INFO(get_logger(), "Configuring...");
    action_client_ = rclcpp_action::create_client<MoveWavemaker>(
      shared_from_this(), "move_wavemaker");
    const auto status_qos = rclcpp::QoS(1).reliable().transient_local();
    state_publisher_ = rclcpp::create_publisher<std_msgs::msg::String>(
        *this, "wavemaker_state", status_qos);
    message_publisher_ = rclcpp::create_publisher<std_msgs::msg::String>(
        *this, "wavemaker_message", status_qos);
    publish_state(State::Inactive, "configured");
    return CallbackReturn::SUCCESS;

  }

  CallbackReturn on_activate(const rclcpp_lifecycle::State &) override
  {
    RCLCPP_INFO(get_logger(), "Activating...");
    bond_ = std::make_unique<bond::Bond>(
      "bond", get_name(), shared_from_this());
    bond_->setHeartbeatPeriod(0.10);
    bond_->setHeartbeatTimeout(get_parameter("bond_timeout").as_double());
    bond_->start();
    const auto command_qos = rclcpp::QoS(10).reliable();
    heartbeat_timeout_ = std::chrono::duration<double>(
      get_parameter("heartbeat_timeout").as_double());
    heartbeat_subscription_ = create_subscription<std_msgs::msg::Bool>(
      "heartbeat", rclcpp::SensorDataQoS(),
      [this](std_msgs::msg::Bool::ConstSharedPtr){
        last_heartbeat_ = std::chrono::steady_clock::now();
        heartbeat_seen_ = true;
      });
    watch_timer_ = create_wall_timer(std::chrono::milliseconds(100), [this]() {watch();});

    height_subscription_ = create_subscription<std_msgs::msg::Float64>(
      "height", 10,
      std::bind(&WavemakerBridge::height_callback, this, std::placeholders::_1));
    period_subscription_ = create_subscription<std_msgs::msg::Float64>(
      "period", 10,
      std::bind(&WavemakerBridge::period_callback, this, std::placeholders::_1));
    stop_subscription_ = create_subscription<std_msgs::msg::Bool>(
      "stop", 10,
      std::bind(&WavemakerBridge::stop_callback, this, std::placeholders::_1));
    start_subscription_ = create_subscription<std_msgs::msg::Bool>(
      "start", command_qos, [this](std_msgs::msg::Bool::ConstSharedPtr msg) {
        if (msg->data) {
          start();
        }
      });
    stop_service_ = create_service<std_srvs::srv::Trigger>(
      "stop",
      [this](
        std::shared_ptr<rclcpp::Service<std_srvs::srv::Trigger>> service,
        std::shared_ptr<rmw_request_id_t> request_id,
        std::shared_ptr<std_srvs::srv::Trigger::Request>)
      {
        // Reply later, from the cancel callback, so the executor is never blocked.
        stop("Stopped by stop service",
        [this, service, request_id](bool ok, const std::string & message) {
          std_srvs::srv::Trigger::Response response;
          response.success = ok;
          response.message = message;
          try {
            service->send_response(*request_id, response);
          } catch (const std::exception & error) {
            RCLCPP_WARN(get_logger(), "Could not send stop reply: %s", error.what());
          }
          });
      });
    pre_shutdown_callback_handle_ =
      get_node_base_interface()->get_context()->add_pre_shutdown_callback(
      [this]() {
        stop("Bridge is shutting down");
      });
    publish_state(State::Idle, "ready");
    return CallbackReturn::SUCCESS;
  }

  CallbackReturn on_deactivate(const rclcpp_lifecycle::State &) override
  {
    cleanup_helper("Bridge is deactivating");
    return CallbackReturn::SUCCESS;
  }

  CallbackReturn on_cleanup(const rclcpp_lifecycle::State &) override
  {
    RCLCPP_INFO(get_logger(), "Cleaning up...");
    action_client_.reset();
    state_publisher_.reset();
    message_publisher_.reset();

    return CallbackReturn::SUCCESS;
  }

  CallbackReturn on_shutdown(const rclcpp_lifecycle::State &) override
  {
    cleanup_helper("Bridge is shutting down");
    return CallbackReturn::SUCCESS;
  }

  CallbackReturn on_error(const rclcpp_lifecycle::State &) override
  {
    cleanup_helper("Bridge error");
    return CallbackReturn::SUCCESS;
  }
  bool heartbeat_alive() const
  {
    return heartbeat_seen_ &&
           (std::chrono::steady_clock::now() - last_heartbeat_ < heartbeat_timeout_);
  }
  void watch()
  {
    if ((state_ == State::Starting || state_ == State::Running) && !heartbeat_alive()) {
      stop("Stopped: client heartbeat (LabVIEW or web page) lost");
    }
    if (++watch_ticks_ % 10 == 0) {
      publish_state(state_, last_message_);
    }
  }

  void start()
  {
    if (state_ != State::Idle) {
      report(std::string("start refused: wave is ") + state_name(state_) + "; stop it first");
      return;
    }
    if (!(height_ > 0.0) || !(period_ > 0.0) || !std::isfinite(height_) ||
      !std::isfinite(period_))
    {
      report("start refused: height and period must be positive and finite");
      return;
    }
    if(!heartbeat_alive()) {
      report("start refused: no client heartbeat (LabVIEW or web page)");
      return;
    }
    if (!action_client_ || !action_client_->action_server_is_ready()) {
      report("start refused: action server not available");
      return;
    }


    MoveWavemaker::Goal goal;
    goal.height = height_;
    goal.period = period_;
    rclcpp_action::Client<MoveWavemaker>::SendGoalOptions options;
    options.goal_response_callback =
      [this](MoveWavemakerGoalHandle::SharedPtr handle) {on_goal_response(handle);};
    options.result_callback =
      [this](const MoveWavemakerGoalHandle::WrappedResult & result) {on_goal_result(result);};
    goal_requested_ = true;
    // The values are in the message so clients can check which ones were used: height,
    // period and start are separate topics, and DDS does not order them.
    char text[96];
    std::snprintf(
      text, sizeof(text), "Starting wave: height %.4f m, period %.3f s", height_, period_);
    publish_state(State::Starting, text);
    action_client_->async_send_goal(goal, options);
  }

  void cleanup_helper(const std::string & reason)
  {
    stop(reason);
    finish_stop(false, "Bridge is no longer active");
    get_node_base_interface()->get_context()->remove_pre_shutdown_callback(
      pre_shutdown_callback_handle_);
    watch_timer_.reset();
    stop_service_.reset();
    heartbeat_subscription_.reset();
    stop_subscription_.reset();
    start_subscription_.reset();
    period_subscription_.reset();
    height_subscription_.reset();
    if (bond_) {
      bond_->breakBond();
      bond_.reset();
    }
    publish_state(State::Inactive, reason);
  }

  void height_callback(const std_msgs::msg::Float64::SharedPtr msg)
  {
    height_ = msg->data;

  }


  void period_callback(const std_msgs::msg::Float64::SharedPtr msg)
  {
    period_ = msg->data;
  }

  void stop_callback(const std_msgs::msg::Bool::SharedPtr msg)
  {
    if (msg->data) {
      stop("Operator pressed stop");
    }
  }
  void stop(const std::string & reason, StopDone done = nullptr)
  {

    if (goal_requested_) {
      cancel_on_accept_ = true;
    }
    const bool ours = goal_requested_ || goal_handle_;
    if (ours) {
      publish_state(State::Stopping, reason);
    }
    if(!action_client_ || !action_client_->action_server_is_ready()) {
      report("Stop: Move Wavemaker action is not available");
      if (done) {
        done(false, "Move Wavemaker action is not available");
      }
      return;
    }
    action_client_->async_cancel_all_goals(
      [this, done, ours](rclcpp_action::Client<MoveWavemaker>::CancelResponse::SharedPtr response)
      {
        RCLCPP_INFO(get_logger(), "Stop: %zu goal(s) canceling", response->goals_canceling.size());
        if (!done) {
          return;
        }
        if (response->return_code != response->ERROR_NONE) {
          done(false, "Controller rejected the cancel request");
        } else if (!ours) {
          done(true, response->goals_canceling.empty() ? "Nothing running" : "Cancel accepted");

        } else {
          stop_done_ = done;
          stop_timeout_ = create_wall_timer(std::chrono::seconds(2), [this]() {
            if (stop_done_) {
              finish_stop(false, "No confirmation from controller");
            }
          });
        }
      });
  }

  void publish_state(State state, const std::string & message)
  {
    state_ = state;
    last_message_ = message;
    if (state_publisher_) {
      std_msgs::msg::String msg;
      msg.data = state_name(state);
      state_publisher_->publish(msg);
    }
    if (message_publisher_) {
      std_msgs::msg::String msg;
      msg.data = message;
      message_publisher_->publish(msg);
    }
  }

  // Reports a refused command without changing the state.
  void report(const std::string & message)
  {
    RCLCPP_WARN(get_logger(), "%s", message.c_str());
    publish_state(state_, message);
  }
  void on_goal_response(MoveWavemakerGoalHandle::SharedPtr handle_)
  {
    goal_requested_ = false;
    if (!handle_) {
      cancel_on_accept_ = false;
      // Back to idle so a new start is possible; stay inactive if the bridge left meanwhile.
      publish_state(idle_unless_inactive(), "Goal rejected by controller (see its log)");
      finish_stop(true, "stopped");
      return;
    }
    goal_handle_ = handle_;
    if (cancel_on_accept_) {
      cancel_on_accept_ = false;
      action_client_->async_cancel_goal(handle_);
      return;
    }
    publish_state(State::Running, "Wave running");
  }

  void on_goal_result(const MoveWavemakerGoalHandle::WrappedResult & result)
  {
    goal_handle_.reset();
    std::string message;
    switch (result.code) {
      case rclcpp_action::ResultCode::CANCELED:
        message = "Stopped, holding position.";
        break;
      case rclcpp_action::ResultCode::SUCCEEDED:
        message = "Wave finished successfully";
        break;
      case rclcpp_action::ResultCode::ABORTED:
        message = "Wave aborted: " + result.result->message;
        break;
      default:
        message = "Wave ended";
        break;
    }
    // The result can arrive after the bridge was deactivated; then it must stay inactive.
    publish_state(idle_unless_inactive(), message);
    finish_stop(true, message);
  }

  State idle_unless_inactive() const
  {
    return state_ == State::Inactive ? State::Inactive : State::Idle;
  }

  void finish_stop(bool ok, const std::string & message)
  {
    if (stop_timeout_) {
      stop_timeout_->cancel();
      stop_timeout_.reset();
    }
    if (auto done = std::exchange(stop_done_, nullptr)) {
      done(ok, message);
    }
  }
  rclcpp_action::Client<MoveWavemaker>::SharedPtr action_client_;
  rclcpp::Subscription<std_msgs::msg::Float64>::SharedPtr height_subscription_;
  rclcpp::Subscription<std_msgs::msg::Float64>::SharedPtr period_subscription_;
  rclcpp::Subscription<std_msgs::msg::Bool>::SharedPtr stop_subscription_;
  rclcpp::Subscription<std_msgs::msg::Bool>::SharedPtr start_subscription_;
  rclcpp::Subscription<std_msgs::msg::Bool>::SharedPtr heartbeat_subscription_;
  rclcpp::Service<std_srvs::srv::Trigger>::SharedPtr stop_service_;
  rclcpp::Publisher<std_msgs::msg::String>::SharedPtr state_publisher_;
  rclcpp::Publisher<std_msgs::msg::String>::SharedPtr message_publisher_;
  MoveWavemakerGoalHandle::SharedPtr goal_handle_;
  rclcpp::TimerBase::SharedPtr watch_timer_;
  bool heartbeat_seen_ = {false};
  bool goal_requested_ = {false};
  bool cancel_on_accept_ = {false};
  std::chrono::steady_clock::time_point last_heartbeat_{};
  std::chrono::duration<double> heartbeat_timeout_{1.0};
  unsigned int watch_ticks_{0};
  StopDone stop_done_;
  rclcpp::TimerBase::SharedPtr stop_timeout_;
  State state_ = State::Inactive;
  std::string last_message_;
  std::unique_ptr<bond::Bond> bond_;
  double height_ = 0.0;
  double period_ = 0.0;
  rclcpp::PreShutdownCallbackHandle pre_shutdown_callback_handle_;

};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  auto node = std::make_shared<WavemakerBridge>(rclcpp::NodeOptions());
  rclcpp::spin(node->get_node_base_interface());
  rclcpp::shutdown();
  return 0;
}
