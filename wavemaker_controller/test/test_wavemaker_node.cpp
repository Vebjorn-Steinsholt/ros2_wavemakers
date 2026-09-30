#include <chrono>
#include <atomic>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <utility>

#include <gtest/gtest.h>
#include <rclcpp/rclcpp.hpp>
#include <rclcpp_action/rclcpp_action.hpp>
#include <rclcpp_lifecycle/lifecycle_node.hpp>
#include "lifecycle_msgs/msg/state.hpp"
#include "lifecycle_msgs/msg/transition.hpp"
#include "wavemaker_interfaces/action/move_wavemaker.hpp"

#define main wavemaker_node_main
#include "../src/wavemaker_node.cpp"
#undef main

namespace {

using MoveWavemaker = wavemaker_interfaces::action::MoveWavemaker;
using CallbackReturn = rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn;

class FakeActuator final : public wavemaker_controller::WavemakerActuator
{
public:
  void set_fault_callback(FaultCallback callback) override
  {
    fault_callback_ = std::move(callback);
  }

  bool start(const rclcpp::Logger &) override
  {
    started = true;
    return true;
  }

  void stop() override
  {
    ++stop_count;
    started = false;
  }

  bool is_live() const override { return started; }
  bool faulted() const override { return false; }
  std::string fault_reason() const override { return {}; }
  std::string status() const override { return "fake"; }
  double actual_position_m() const override { return actual_position; }

  wavemaker_controller::ActuatorSetpoint to_actuator_setpoint(
    double position_m, double velocity_mps) const override
  {
    return {position_m, velocity_mps};
  }

  bool write_setpoint(const wavemaker_controller::ActuatorSetpoint & setpoint) override
  {
    std::lock_guard<std::mutex> lock(setpoint_mutex);
    if (write_count.load() == 0) {
      first_position = setpoint.position;
      first_velocity = setpoint.velocity;
    }
    last_position = setpoint.position;
    last_velocity = setpoint.velocity;
    ++write_count;
    return true;
  }

  bool started{false};
  int stop_count{0};
  std::atomic<int> write_count{0};
  double actual_position{0.25};
  double first_position{0.0};
  double first_velocity{0.0};
  double last_position{0.0};
  double last_velocity{0.0};
  std::mutex setpoint_mutex;
  FaultCallback fault_callback_;
};

rclcpp::NodeOptions node_options(const std::string & ns, bool pregenerated)
{
  return rclcpp::NodeOptions()
         .arguments({"--ros-args", "-r", "__ns:=" + ns})
         .parameter_overrides({
      rclcpp::Parameter("wavemaker_type", "piston"),
      rclcpp::Parameter("wavemaker_minimum", 0.0),
      rclcpp::Parameter("wavemaker_maximum", 0.5),
      rclcpp::Parameter("wavemaker_upright_position_m", 0.25),
      rclcpp::Parameter("wavemaker_mode_pregenerated", pregenerated),
      rclcpp::Parameter("water_depth", 1.0),
      rclcpp::Parameter("flap_attachment_height", 0.0),
      rclcpp::Parameter("actuator_upright_angle_deg", 0.0),
    });
}

rclcpp::NodeOptions angular_node_options(const std::string & ns)
{
  return rclcpp::NodeOptions()
    .arguments({"--ros-args", "-r", "__ns:=" + ns})
    .parameter_overrides({
      rclcpp::Parameter("wavemaker_type", "piston"),
      rclcpp::Parameter("wavemaker_minimum", 0.2),
      rclcpp::Parameter("wavemaker_maximum", 0.3),
      rclcpp::Parameter("wavemaker_upright_position_m", 0.25),
      rclcpp::Parameter("wavemaker_mode_pregenerated", true),
      rclcpp::Parameter("water_depth", 1.0),
      rclcpp::Parameter("flap_attachment_height", 0.0),
      rclcpp::Parameter("actuator_upright_angle_deg", 0.0),
      rclcpp::Parameter("actuator_drive_type", "angular"),
      rclcpp::Parameter("actuator_lead_m_per_degree", 0.001),
      rclcpp::Parameter("min_position_deg", -100.0),
      rclcpp::Parameter("max_position_deg", 100.0),
    });
}

template<typename FutureT>
bool spin_until(
  rclcpp::executors::MultiThreadedExecutor & executor, FutureT & future,
  std::chrono::milliseconds timeout)
{
  const auto deadline = std::chrono::steady_clock::now() + timeout;
  while (future.wait_for(std::chrono::milliseconds(0)) != std::future_status::ready) {
    executor.spin_some();
    if (std::chrono::steady_clock::now() >= deadline) {
      return false;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(5));
  }
  return true;
}

class WavemakerNodeTest : public ::testing::Test
{
protected:
  static void SetUpTestSuite()
  {
    rclcpp::init(0, nullptr);
  }

  static void TearDownTestSuite()
  {
    rclcpp::shutdown();
  }
};

}  // namespace

TEST_F(WavemakerNodeTest, LifecycleConfigureActivateDeactivate)
{
  FakeActuator * fake = nullptr;
  auto factory = [&fake](rclcpp_lifecycle::LifecycleNode &) {
    auto actuator = std::make_unique<FakeActuator>();
    fake = actuator.get();
    return actuator;
  };
  auto node = std::make_shared<WavemakerNode>(node_options("/lifecycle_test", false), factory);

  auto state = node->trigger_transition(lifecycle_msgs::msg::Transition::TRANSITION_CONFIGURE);
  ASSERT_EQ(state.id(), lifecycle_msgs::msg::State::PRIMARY_STATE_INACTIVE);
  ASSERT_NE(fake, nullptr);

  state = node->trigger_transition(lifecycle_msgs::msg::Transition::TRANSITION_ACTIVATE);
  ASSERT_EQ(state.id(), lifecycle_msgs::msg::State::PRIMARY_STATE_ACTIVE);
  EXPECT_TRUE(fake->started);

  state = node->trigger_transition(lifecycle_msgs::msg::Transition::TRANSITION_DEACTIVATE);
  EXPECT_EQ(state.id(), lifecycle_msgs::msg::State::PRIMARY_STATE_INACTIVE);
  EXPECT_FALSE(fake->started);
  EXPECT_GE(fake->stop_count, 1);
}

TEST_F(WavemakerNodeTest, PregeneratedActionCompletesAndStopsActuator)
{
  FakeActuator * fake = nullptr;
  auto factory = [&fake](rclcpp_lifecycle::LifecycleNode &) {
    auto actuator = std::make_unique<FakeActuator>();
    fake = actuator.get();
    return actuator;
  };
  auto node = std::make_shared<WavemakerNode>(node_options("/completion_test", true), factory);
  ASSERT_EQ(
    node->trigger_transition(lifecycle_msgs::msg::Transition::TRANSITION_CONFIGURE).id(),
    lifecycle_msgs::msg::State::PRIMARY_STATE_INACTIVE);
  ASSERT_EQ(
    node->trigger_transition(lifecycle_msgs::msg::Transition::TRANSITION_ACTIVATE).id(),
    lifecycle_msgs::msg::State::PRIMARY_STATE_ACTIVE);

  auto client_node = std::make_shared<rclcpp::Node>("completion_client", node_options("/completion_test", true));
  auto client = rclcpp_action::create_client<MoveWavemaker>(client_node, "move_wavemaker");
  rclcpp::executors::MultiThreadedExecutor executor;
  executor.add_node(node->get_node_base_interface());
  executor.add_node(client_node);

  ASSERT_TRUE(client->wait_for_action_server(std::chrono::seconds(1)));
  MoveWavemaker::Goal goal;
  goal.positions = {0.25, 0.26};
  goal.sample_interval = 0.02;
  auto goal_future = client->async_send_goal(goal);
  ASSERT_TRUE(spin_until(executor, goal_future, std::chrono::seconds(1)));
  auto goal_handle = goal_future.get();
  ASSERT_NE(goal_handle, nullptr);

  auto result_future = client->async_get_result(goal_handle);
  ASSERT_TRUE(spin_until(executor, result_future, std::chrono::seconds(2)));
  EXPECT_EQ(result_future.get().code, rclcpp_action::ResultCode::SUCCEEDED);
  EXPECT_GE(fake->stop_count, 1);
}

TEST_F(WavemakerNodeTest, CancelledActionStopsActuator)
{
  FakeActuator * fake = nullptr;
  auto factory = [&fake](rclcpp_lifecycle::LifecycleNode &) {
    auto actuator = std::make_unique<FakeActuator>();
    fake = actuator.get();
    return actuator;
  };
  auto node = std::make_shared<WavemakerNode>(node_options("/cancel_test", false), factory);
  ASSERT_EQ(
    node->trigger_transition(lifecycle_msgs::msg::Transition::TRANSITION_CONFIGURE).id(),
    lifecycle_msgs::msg::State::PRIMARY_STATE_INACTIVE);
  ASSERT_EQ(
    node->trigger_transition(lifecycle_msgs::msg::Transition::TRANSITION_ACTIVATE).id(),
    lifecycle_msgs::msg::State::PRIMARY_STATE_ACTIVE);

  auto client_node = std::make_shared<rclcpp::Node>("cancel_client", node_options("/cancel_test", false));
  auto client = rclcpp_action::create_client<MoveWavemaker>(client_node, "move_wavemaker");
  rclcpp::executors::MultiThreadedExecutor executor;
  executor.add_node(node->get_node_base_interface());
  executor.add_node(client_node);

  ASSERT_TRUE(client->wait_for_action_server(std::chrono::seconds(1)));
  MoveWavemaker::Goal goal;
  goal.amplitude = 0.001;
  goal.period = 10.0;
  auto goal_future = client->async_send_goal(goal);
  ASSERT_TRUE(spin_until(executor, goal_future, std::chrono::seconds(1)));
  auto goal_handle = goal_future.get();
  ASSERT_NE(goal_handle, nullptr);

  const auto write_deadline = std::chrono::steady_clock::now() + std::chrono::seconds(1);
  while (fake->write_count == 0 && std::chrono::steady_clock::now() < write_deadline) {
    executor.spin_some();
    std::this_thread::sleep_for(std::chrono::milliseconds(5));
  }
  ASSERT_GT(fake->write_count, 0);

  client->async_cancel_goal(goal_handle);
  auto result_future = client->async_get_result(goal_handle);
  ASSERT_TRUE(spin_until(executor, result_future, std::chrono::seconds(2)));
  EXPECT_EQ(result_future.get().code, rclcpp_action::ResultCode::CANCELED);
  EXPECT_GE(fake->stop_count, 1);
}

TEST_F(WavemakerNodeTest, SinusoidalGoalBlendsFromMeasuredPosition)
{
  FakeActuator * fake = nullptr;
  auto factory = [&fake](rclcpp_lifecycle::LifecycleNode &) {
    auto actuator = std::make_unique<FakeActuator>();
    fake = actuator.get();
    fake->actual_position = 0.30;
    return actuator;
  };
  auto node = std::make_shared<WavemakerNode>(node_options("/smooth_start_test", false), factory);
  ASSERT_EQ(
    node->trigger_transition(lifecycle_msgs::msg::Transition::TRANSITION_CONFIGURE).id(),
    lifecycle_msgs::msg::State::PRIMARY_STATE_INACTIVE);
  ASSERT_EQ(
    node->trigger_transition(lifecycle_msgs::msg::Transition::TRANSITION_ACTIVATE).id(),
    lifecycle_msgs::msg::State::PRIMARY_STATE_ACTIVE);

  auto client_node = std::make_shared<rclcpp::Node>(
    "smooth_start_client", node_options("/smooth_start_test", false));
  auto client = rclcpp_action::create_client<MoveWavemaker>(client_node, "move_wavemaker");
  rclcpp::executors::MultiThreadedExecutor executor;
  executor.add_node(node->get_node_base_interface());
  executor.add_node(client_node);

  ASSERT_TRUE(client->wait_for_action_server(std::chrono::seconds(1)));
  MoveWavemaker::Goal goal;
  goal.amplitude = 0.001;
  goal.period = 10.0;
  auto goal_future = client->async_send_goal(goal);
  ASSERT_TRUE(spin_until(executor, goal_future, std::chrono::seconds(1)));
  auto goal_handle = goal_future.get();
  ASSERT_NE(goal_handle, nullptr);

  const auto write_deadline = std::chrono::steady_clock::now() + std::chrono::seconds(1);
  while (fake->write_count.load() == 0 && std::chrono::steady_clock::now() < write_deadline) {
    executor.spin_some();
    std::this_thread::sleep_for(std::chrono::milliseconds(5));
  }
  ASSERT_GT(fake->write_count.load(), 0);
  {
    std::lock_guard<std::mutex> lock(fake->setpoint_mutex);
    EXPECT_NEAR(fake->first_position, fake->actual_position, 1e-5);
    EXPECT_NEAR(fake->first_velocity, 0.0, 1e-5);
  }

  client->async_cancel_goal(goal_handle);
  auto result_future = client->async_get_result(goal_handle);
  ASSERT_TRUE(spin_until(executor, result_future, std::chrono::seconds(2)));
  EXPECT_EQ(result_future.get().code, rclcpp_action::ResultCode::CANCELED);
}

TEST_F(WavemakerNodeTest, AngularDriveLimitsOverrideWavemakerMeterBounds)
{
  auto factory = [](rclcpp_lifecycle::LifecycleNode &) {
      return std::make_unique<FakeActuator>();
    };
  auto node = std::make_shared<WavemakerNode>(
    angular_node_options("/angular_limits_test"), factory);
  ASSERT_EQ(
    node->trigger_transition(lifecycle_msgs::msg::Transition::TRANSITION_CONFIGURE).id(),
    lifecycle_msgs::msg::State::PRIMARY_STATE_INACTIVE);
  ASSERT_EQ(
    node->trigger_transition(lifecycle_msgs::msg::Transition::TRANSITION_ACTIVATE).id(),
    lifecycle_msgs::msg::State::PRIMARY_STATE_ACTIVE);

  auto client_node = std::make_shared<rclcpp::Node>(
    "angular_limits_client", node_options("/angular_limits_test", true));
  auto client = rclcpp_action::create_client<MoveWavemaker>(client_node, "move_wavemaker");
  rclcpp::executors::MultiThreadedExecutor executor;
  executor.add_node(node->get_node_base_interface());
  executor.add_node(client_node);

  ASSERT_TRUE(client->wait_for_action_server(std::chrono::seconds(1)));
  MoveWavemaker::Goal goal;
  goal.positions = {0.15, 0.16};
  goal.sample_interval = 0.02;
  auto goal_future = client->async_send_goal(goal);
  ASSERT_TRUE(spin_until(executor, goal_future, std::chrono::seconds(1)));
  auto goal_handle = goal_future.get();
  ASSERT_NE(goal_handle, nullptr);

  auto result_future = client->async_get_result(goal_handle);
  ASSERT_TRUE(spin_until(executor, result_future, std::chrono::seconds(2)));
  EXPECT_EQ(result_future.get().code, rclcpp_action::ResultCode::SUCCEEDED);
}
