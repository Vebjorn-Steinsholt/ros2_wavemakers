#include "rclcpp/rclcpp.hpp"
#include "rclcpp_lifecycle/lifecycle_node.hpp"
#include "rclcpp_action/rclcpp_action.hpp"
#include "std_msgs/msg/float64.hpp"
#include "wavemaker_interfaces/action/move_wavemaker.hpp"

using rclcpp_lifecycle::LifecycleNode;
using MoveWavemaker = wavemaker_interfaces::action::MoveWavemaker;
using MoveWavemakerGoalHandle = rclcpp_action::ClientGoalHandle<MoveWavemaker>;
using CallbackReturn = rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn;

class WavemakerBridge : public LifecycleNode
{
public:
  explicit WavemakerBridge(const rclcpp::NodeOptions & options)
  : LifecycleNode("wavemaker_bridge", options)
  {
  }

  CallbackReturn on_configure(const rclcpp_lifecycle::State &) override
  {
    RCLCPP_INFO(get_logger(), "Configuring...");
    action_client_ = rclcpp_action::create_client<MoveWavemaker>(shared_from_this(), "move_wavemaker");
    position_publisher_ = create_publisher<std_msgs::msg::Float64>("position", 10);

    return CallbackReturn::SUCCESS;

  }

  CallbackReturn on_activate(const rclcpp_lifecycle::State &) override
  {
    RCLCPP_INFO(get_logger(), "Activating...");
    position_publisher_->on_activate();
    return CallbackReturn::SUCCESS;
  }

  CallbackReturn on_deactivate(const rclcpp_lifecycle::State &) override
  {
    RCLCPP_INFO(get_logger(), "Deactivating...");
    position_publisher_->on_deactivate();
    return CallbackReturn::SUCCESS;
  }

  CallbackReturn on_cleanup(const rclcpp_lifecycle::State &) override
  {
    RCLCPP_INFO(get_logger(), "Cleaning up...");
    action_client_.reset();
    position_publisher_.reset();
    return CallbackReturn::SUCCESS;
  }

  CallbackReturn on_shutdown(const rclcpp_lifecycle::State &) override
  {
    RCLCPP_INFO(get_logger(), "Shutting down...");
    return CallbackReturn::SUCCESS;
  }
  void send_goal(double amplitude, double period)
  {
    MoveWavemaker::Goal goal;
    goal.amplitude = amplitude;
    goal.period = period;
    rclcpp_action::Client<MoveWavemaker>::SendGoalOptions options;
    options.feedback_callback =
      [this](
        MoveWavemakerGoalHandle::SharedPtr,
        const std::shared_ptr<const MoveWavemaker::Feedback> feedback)
      {
        RCLCPP_INFO(
          get_logger(), "Desired: %.4f m, actual: %.4f m, elapsed: %.2f s",
          feedback->desired_position,
          feedback->actual_position,
          feedback->elapsed_time);

        if (position_publisher_->is_activated()) {
          std_msgs::msg::Float64 message;
          message.data = feedback->actual_position;
          position_publisher_->publish(message);
        }
      };

    action_client_->async_send_goal(goal, options);
  }

private:
  rclcpp_action::Client<MoveWavemaker>::SharedPtr action_client_;
  rclcpp_lifecycle::LifecyclePublisher<std_msgs::msg::Float64>::SharedPtr position_publisher_;
  rclcpp::Subscription<std_msgs::msg::Float64>::SharedPtr amplitude_subscription_;
  rclcpp::Subscription<std_msgs::msg::Float64>::SharedPtr period_subscription_;
  void amplitude_callback(const std_msgs::msg::Float64::SharedPtr msg);
  void period_callback(const std_msgs::msg::Float64::SharedPtr msg);
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  auto node = std::make_shared<WavemakerBridge>(rclcpp::NodeOptions());
  rclcpp::spin(node->get_node_base_interface());
  rclcpp::shutdown();
  return 0;
}