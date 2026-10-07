#include <chrono>
#include <atomic>
#include <memory>
#include <mutex>
#include <limits>
#include <string>
#include <thread>
#include <utility>
#include <vector>

#include <gtest/gtest.h>
#include <rclcpp/rclcpp.hpp>
#include <rclcpp_action/rclcpp_action.hpp>
#include <rclcpp_lifecycle/lifecycle_node.hpp>
#include "lifecycle_msgs/msg/state.hpp"
#include "lifecycle_msgs/msg/transition.hpp"
#include "std_msgs/msg/float64.hpp"
#include "wavemaker_interfaces/action/move_wavemaker.hpp"

#define main wavemaker_node_main
#include "../src/wavemaker_node.cpp"
#undef main

namespace
{

using MoveWavemaker = wavemaker_interfaces::action::MoveWavemaker;
using CallbackReturn = rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn;

class FakeActuator final : public wavemaker_controller::WavemakerActuator
{
public:
  ~FakeActuator() override
  {
    if (destructions_seen != nullptr) {
      ++*destructions_seen;
    }
  }

  void set_fault_callback(FaultCallback callback) override
  {
    fault_callback_ = std::move(callback);
  }

  bool start(const rclcpp::Logger &) override
  {
    started = true;
    return true;
  }

  void halt() override
  {
    ++halt_count;
  }

  void stop() override
  {
    ++stop_count;
    if (stops_seen != nullptr) {
      ++*stops_seen;
    }
    started = false;
  }

  bool is_live() const override {return started;}
  bool faulted() const override {return fault.load();}
  std::string fault_reason() const override {return fault.load() ? "fake fault" : "";}
  std::string status() const override {return "fake";}
  double actual_position_m() const override {return actual_position;}
  double max_velocity_mps() const override {return max_velocity;}
  int update_period_ms() const override {return update_period;}

  wavemaker_controller::ActuatorSetpoint to_actuator_setpoint(
    double position_m, double velocity_mps) const override
  {
    return {position_m * units_per_m, velocity_mps * units_per_m};
  }

  bool write_setpoint(const wavemaker_controller::ActuatorSetpoint & setpoint) override
  {
    if (reject_writes.load() || std::abs(setpoint.velocity) / units_per_m > max_velocity ||
      setpoint.position < min_position || setpoint.position > max_position)
    {
      return false;
    }
    std::lock_guard<std::mutex> lock(setpoint_mutex);
    if (write_count.load() == 0) {
      first_position = setpoint.position;
      first_velocity = setpoint.velocity;
    }
    if (write_count.load() > 0) {
      max_step = std::max(max_step, std::abs(setpoint.position - last_position));
    }
    last_position = setpoint.position;
    last_velocity = setpoint.velocity;
    if (follow_setpoints) {
      actual_position = setpoint.position / units_per_m;
    }
    written_positions.push_back(setpoint.position);
    minimum_position = std::min(minimum_position, setpoint.position);
    maximum_position = std::max(maximum_position, setpoint.position);
    ++write_count;
    return true;
  }

  bool started{false};
  int stop_count{0};
  int halt_count{0};
  std::atomic<int> write_count{0};
  std::atomic<bool> fault{false};
  std::atomic<bool> reject_writes{false};  // write_setpoint returns false
  // Like IndraDrive::move_to(): writes above this speed fail. Unlimited by default.
  double max_velocity{std::numeric_limits<double>::infinity()};
  int update_period{10};  // ms; sets the controller's loop rate
  // Like IndraDrive::move_to(): writes outside these positions fail. Unlimited by default.
  double min_position{-std::numeric_limits<double>::infinity()};
  double max_position{std::numeric_limits<double>::infinity()};
  // Counts stop() calls outside the fake, which release_resources() destroys.
  std::atomic<int> * stops_seen{nullptr};
  std::atomic<int> * destructions_seen{nullptr};
  // Actuator units per metre in to_actuator_setpoint(); the recorded setpoints are in these units.
  double units_per_m{1.0};
  std::vector<double> written_positions;  // every position passed to write_setpoint()
  bool follow_setpoints{false};  // actual position tracks the last written setpoint
  double actual_position{0.25};
  double first_position{0.0};
  double first_velocity{0.0};
  double last_position{0.0};
  double last_velocity{0.0};
  double max_step{0.0};  // largest change between consecutive setpoints
  double minimum_position{std::numeric_limits<double>::infinity()};
  double maximum_position{-std::numeric_limits<double>::infinity()};
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

rclcpp::NodeOptions one_sided_node_options(const std::string & ns)
{
  return rclcpp::NodeOptions()
         .arguments({"--ros-args", "-r", "__ns:=" + ns})
         .parameter_overrides({
      rclcpp::Parameter("wavemaker_type", "flap"),
      rclcpp::Parameter("wavemaker_minimum", 0.25),
      rclcpp::Parameter("wavemaker_maximum", 0.5),
      rclcpp::Parameter("wavemaker_upright_position_m", 0.25),
      rclcpp::Parameter("wavemaker_upright_is_minimum", true),
      rclcpp::Parameter("wavemaker_mode_pregenerated", false),
      rclcpp::Parameter("water_depth", 1.0),
      rclcpp::Parameter("flap_attachment_height", 1.3),
      rclcpp::Parameter("actuator_upright_angle_deg", 0.0),
      rclcpp::Parameter("actuator_drive_type", "linear"),
      rclcpp::Parameter("min_position_m", 0.25),
      rclcpp::Parameter("max_position_m", 0.5),
    });
}

bool spin_until_state(
  rclcpp::Executor & executor, rclcpp_lifecycle::LifecycleNode & node, uint8_t state_id,
  std::chrono::milliseconds timeout)
{
  const auto deadline = std::chrono::steady_clock::now() + timeout;
  while (node.get_current_state().id() != state_id) {
    executor.spin_some();
    if (std::chrono::steady_clock::now() >= deadline) {
      return false;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(5));
  }
  return true;
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

// Creates a controller whose fake drive accepts at most max_velocity_mps.
std::shared_ptr<WavemakerNode> speed_limited_node(
  const std::string & ns, bool pregenerated, double max_velocity_mps,
  const std::vector<rclcpp::Parameter> & overrides = {})
{
  auto factory = [max_velocity_mps](rclcpp_lifecycle::LifecycleNode &) {
      auto actuator = std::make_unique<FakeActuator>();
      actuator->max_velocity = max_velocity_mps;
      return actuator;
    };
  auto options = node_options(ns, pregenerated);
  options.append_parameter_override("return_to_upright_max_velocity_mps", 0.05);
  for (const auto & parameter : overrides) {
    options.append_parameter_override(parameter.get_name(), parameter.get_parameter_value());
  }
  return std::make_shared<WavemakerNode>(options, factory);
}

// Configures and activates node, sends one goal, cancels it if accepted, deactivates the node
// and reports whether the controller accepted the goal.
bool goal_accepted(
  const std::shared_ptr<WavemakerNode> & node, const std::string & ns, bool pregenerated,
  const MoveWavemaker::Goal & goal)
{
  EXPECT_EQ(
    node->trigger_transition(lifecycle_msgs::msg::Transition::TRANSITION_CONFIGURE).id(),
    lifecycle_msgs::msg::State::PRIMARY_STATE_INACTIVE);
  EXPECT_EQ(
    node->trigger_transition(lifecycle_msgs::msg::Transition::TRANSITION_ACTIVATE).id(),
    lifecycle_msgs::msg::State::PRIMARY_STATE_ACTIVE);

  auto client_node = std::make_shared<rclcpp::Node>(
    "speed_limit_client", node_options(ns, pregenerated));
  auto client = rclcpp_action::create_client<MoveWavemaker>(client_node, "move_wavemaker");
  rclcpp::executors::MultiThreadedExecutor executor;
  executor.add_node(node->get_node_base_interface());
  executor.add_node(client_node);
  EXPECT_TRUE(client->wait_for_action_server(std::chrono::seconds(1)));

  auto goal_future = client->async_send_goal(goal);
  EXPECT_TRUE(spin_until(executor, goal_future, std::chrono::seconds(1)));
  auto goal_handle = goal_future.get();
  if (goal_handle) {
    auto cancel_future = client->async_cancel_goal(goal_handle);
    spin_until(executor, cancel_future, std::chrono::seconds(1));
  }
  node->trigger_transition(lifecycle_msgs::msg::Transition::TRANSITION_DEACTIVATE);
  return goal_handle != nullptr;
}

// Like goal_accepted() on a fresh speed_limited_node().
bool goal_accepted_with_speed_limit(
  const std::string & ns, bool pregenerated, double max_velocity_mps,
  const MoveWavemaker::Goal & goal, const std::vector<rclcpp::Parameter> & overrides = {})
{
  return goal_accepted(
    speed_limited_node(ns, pregenerated, max_velocity_mps, overrides), ns, pregenerated, goal);
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

struct ReturnToUprightRig
{
  FakeActuator * fake{nullptr};
  std::shared_ptr<WavemakerNode> node;
  std::shared_ptr<rclcpp::Node> client_node;
  rclcpp::Client<ReturnToUpright>::SharedPtr client;
  rclcpp_action::Client<MoveWavemaker>::SharedPtr action_client;
  rclcpp::executors::MultiThreadedExecutor executor;

  void setup(
    const std::string & ns, double start_position, bool activate,
    double max_velocity_mps = 0.1, int update_period_ms = 10)
  {
    auto factory = [this, start_position, update_period_ms](rclcpp_lifecycle::LifecycleNode &) {
        auto actuator = std::make_unique<FakeActuator>();
        fake = actuator.get();
        fake->actual_position = start_position;
        fake->update_period = update_period_ms;
        return actuator;
      };
    auto options = node_options(ns, false);
    options.append_parameter_override("return_to_upright_timeout_margin_s", 0.2);
    options.append_parameter_override("return_to_upright_max_velocity_mps", max_velocity_mps);
    node = std::make_shared<WavemakerNode>(options, factory);
    ASSERT_EQ(
      node->trigger_transition(lifecycle_msgs::msg::Transition::TRANSITION_CONFIGURE).id(),
      lifecycle_msgs::msg::State::PRIMARY_STATE_INACTIVE);
    if (activate) {
      ASSERT_EQ(
        node->trigger_transition(lifecycle_msgs::msg::Transition::TRANSITION_ACTIVATE).id(),
        lifecycle_msgs::msg::State::PRIMARY_STATE_ACTIVE);
    }

    client_node = std::make_shared<rclcpp::Node>("return_client", node_options(ns, false));
    client = client_node->create_client<ReturnToUpright>("return_to_upright");
    action_client = rclcpp_action::create_client<MoveWavemaker>(client_node, "move_wavemaker");
    executor.add_node(node->get_node_base_interface());
    executor.add_node(client_node);
    ASSERT_TRUE(client->wait_for_service(std::chrono::seconds(1)));
  }

  ReturnToUpright::Response call(double tolerance)
  {
    auto request = std::make_shared<ReturnToUpright::Request>();
    request->requester = "test";
    request->tolerance = tolerance;
    auto future = client->async_send_request(request);
    if (!spin_until(executor, future, std::chrono::seconds(5))) {
      ADD_FAILURE() << "return_to_upright did not reply";
      return ReturnToUpright::Response();
    }
    return *future.get();
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

TEST_F(WavemakerNodeTest, PregeneratedActionCompletesAndKeepsActuatorEnabled)
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

  auto client_node = std::make_shared<rclcpp::Node>("completion_client",
    node_options("/completion_test", true));
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
  EXPECT_EQ(fake->halt_count, 1);
  EXPECT_TRUE(fake->started);
}

TEST_F(WavemakerNodeTest, PregeneratedGoalBlendsFromMeasuredPosition)
{
  FakeActuator * fake = nullptr;
  auto factory = [&fake](rclcpp_lifecycle::LifecycleNode &) {
      auto actuator = std::make_unique<FakeActuator>();
      fake = actuator.get();
      fake->actual_position = 0.30;
      return actuator;
    };
  auto node = std::make_shared<WavemakerNode>(node_options("/pregenerated_blend_test", true),
    factory);
  ASSERT_EQ(
    node->trigger_transition(lifecycle_msgs::msg::Transition::TRANSITION_CONFIGURE).id(),
    lifecycle_msgs::msg::State::PRIMARY_STATE_INACTIVE);
  ASSERT_EQ(
    node->trigger_transition(lifecycle_msgs::msg::Transition::TRANSITION_ACTIVATE).id(),
    lifecycle_msgs::msg::State::PRIMARY_STATE_ACTIVE);

  auto client_node = std::make_shared<rclcpp::Node>(
    "pregenerated_blend_client", node_options("/pregenerated_blend_test", true));
  auto client = rclcpp_action::create_client<MoveWavemaker>(client_node, "move_wavemaker");
  rclcpp::executors::MultiThreadedExecutor executor;
  executor.add_node(node->get_node_base_interface());
  executor.add_node(client_node);

  ASSERT_TRUE(client->wait_for_action_server(std::chrono::seconds(1)));
  // 0.05 m from the paddle to positions[0]; the samples themselves move at 0.1 m/s.
  MoveWavemaker::Goal goal;
  goal.positions = {0.25, 0.26};
  goal.sample_interval = 0.1;
  auto goal_future = client->async_send_goal(goal);
  ASSERT_TRUE(spin_until(executor, goal_future, std::chrono::seconds(1)));
  auto goal_handle = goal_future.get();
  ASSERT_NE(goal_handle, nullptr);

  // Blend lasts max(1.875 * 0.05 / 0.1, 1.0) = 1.0 s, then 0.1 s of samples.
  auto result_future = client->async_get_result(goal_handle);
  ASSERT_TRUE(spin_until(executor, result_future, std::chrono::seconds(4)));
  EXPECT_EQ(result_future.get().code, rclcpp_action::ResultCode::SUCCEEDED);

  std::lock_guard<std::mutex> lock(fake->setpoint_mutex);
  EXPECT_NEAR(fake->first_position, 0.30, 1e-4);
  EXPECT_NEAR(fake->last_position, 0.26, 1e-9);
  // Without the blend the first sample would be a 0.05 m step. Peak blend speed is
  // 0.094 m/s, so a 10 ms cycle moves about 1 mm; allow 3 mm for loop jitter.
  EXPECT_LT(fake->max_step, 0.003);
}

TEST_F(WavemakerNodeTest, CancelledActionHaltsAndAcceptsNextGoal)
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

  auto client_node = std::make_shared<rclcpp::Node>("cancel_client",
    node_options("/cancel_test", false));
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
  EXPECT_EQ(fake->halt_count, 1);
  EXPECT_TRUE(fake->started);

  const int writes_after_cancel = fake->write_count.load();
  auto second_goal_future = client->async_send_goal(goal);
  ASSERT_TRUE(spin_until(executor, second_goal_future, std::chrono::seconds(1)));
  auto second_goal_handle = second_goal_future.get();
  ASSERT_NE(second_goal_handle, nullptr);

  const auto second_write_deadline =
    std::chrono::steady_clock::now() + std::chrono::seconds(1);
  while (fake->write_count.load() <= writes_after_cancel &&
    std::chrono::steady_clock::now() < second_write_deadline)
  {
    executor.spin_some();
    std::this_thread::sleep_for(std::chrono::milliseconds(5));
  }
  EXPECT_GT(fake->write_count.load(), writes_after_cancel);

  client->async_cancel_goal(second_goal_handle);
  auto second_result_future = client->async_get_result(second_goal_handle);
  ASSERT_TRUE(spin_until(executor, second_result_future, std::chrono::seconds(2)));
  EXPECT_EQ(second_result_future.get().code, rclcpp_action::ResultCode::CANCELED);
  EXPECT_EQ(fake->halt_count, 2);
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
    // Starting at rest is checked in test_wave_trajectory; the drive gets the speed cap.
    EXPECT_NEAR(fake->first_position, fake->actual_position, 1e-5);
  }

  client->async_cancel_goal(goal_handle);
  auto result_future = client->async_get_result(goal_handle);
  ASSERT_TRUE(spin_until(executor, result_future, std::chrono::seconds(2)));
  EXPECT_EQ(result_future.get().code, rclcpp_action::ResultCode::CANCELED);
  EXPECT_EQ(fake->halt_count, 1);
}

TEST_F(WavemakerNodeTest, UprightMinimumWaveformNeverCommandsBehindUpright)
{
  FakeActuator * fake = nullptr;
  auto factory = [&fake](rclcpp_lifecycle::LifecycleNode &) {
      auto actuator = std::make_unique<FakeActuator>();
      fake = actuator.get();
      fake->actual_position = 0.25;
      return actuator;
    };
  auto node = std::make_shared<WavemakerNode>(
    one_sided_node_options("/upright_minimum_test"), factory);
  ASSERT_EQ(
    node->trigger_transition(lifecycle_msgs::msg::Transition::TRANSITION_CONFIGURE).id(),
    lifecycle_msgs::msg::State::PRIMARY_STATE_INACTIVE);
  ASSERT_EQ(
    node->trigger_transition(lifecycle_msgs::msg::Transition::TRANSITION_ACTIVATE).id(),
    lifecycle_msgs::msg::State::PRIMARY_STATE_ACTIVE);

  auto client_node = std::make_shared<rclcpp::Node>(
    "upright_minimum_client", node_options("/upright_minimum_test", false));
  auto client = rclcpp_action::create_client<MoveWavemaker>(client_node, "move_wavemaker");
  rclcpp::executors::MultiThreadedExecutor executor;
  executor.add_node(node->get_node_base_interface());
  executor.add_node(client_node);

  ASSERT_TRUE(client->wait_for_action_server(std::chrono::seconds(1)));
  MoveWavemaker::Goal goal;
  goal.amplitude = 0.00001;
  goal.period = 0.5;
  auto goal_future = client->async_send_goal(goal);
  ASSERT_TRUE(spin_until(executor, goal_future, std::chrono::seconds(1)));
  auto goal_handle = goal_future.get();
  ASSERT_NE(goal_handle, nullptr);

  const auto write_deadline = std::chrono::steady_clock::now() + std::chrono::seconds(2);
  while (fake->write_count.load() < 120 && std::chrono::steady_clock::now() < write_deadline) {
    executor.spin_some();
    std::this_thread::sleep_for(std::chrono::milliseconds(5));
  }
  ASSERT_GE(fake->write_count.load(), 120);
  {
    std::lock_guard<std::mutex> lock(fake->setpoint_mutex);
    EXPECT_GE(fake->minimum_position, 0.25 - 1e-9);
    EXPECT_GT(fake->maximum_position, 0.25);
    EXPECT_LE(fake->maximum_position, 0.5);
  }

  client->async_cancel_goal(goal_handle);
  auto result_future = client->async_get_result(goal_handle);
  ASSERT_TRUE(spin_until(executor, result_future, std::chrono::seconds(2)));
  EXPECT_EQ(result_future.get().code, rclcpp_action::ResultCode::CANCELED);
}

TEST_F(WavemakerNodeTest, SymmetricGoalOscillatesAroundUpright)
{
  FakeActuator * fake = nullptr;
  auto factory = [&fake](rclcpp_lifecycle::LifecycleNode &) {
      auto actuator = std::make_unique<FakeActuator>();
      fake = actuator.get();
      fake->actual_position = 0.25;
      return actuator;
    };
  auto node = std::make_shared<WavemakerNode>(node_options("/symmetric_test", false), factory);
  ASSERT_EQ(
    node->trigger_transition(lifecycle_msgs::msg::Transition::TRANSITION_CONFIGURE).id(),
    lifecycle_msgs::msg::State::PRIMARY_STATE_INACTIVE);
  ASSERT_EQ(
    node->trigger_transition(lifecycle_msgs::msg::Transition::TRANSITION_ACTIVATE).id(),
    lifecycle_msgs::msg::State::PRIMARY_STATE_ACTIVE);

  auto client_node = std::make_shared<rclcpp::Node>(
    "symmetric_client", node_options("/symmetric_test", false));
  auto client = rclcpp_action::create_client<MoveWavemaker>(client_node, "move_wavemaker");
  rclcpp::executors::MultiThreadedExecutor executor;
  executor.add_node(node->get_node_base_interface());
  executor.add_node(client_node);

  ASSERT_TRUE(client->wait_for_action_server(std::chrono::seconds(1)));
  MoveWavemaker::Goal goal;
  goal.amplitude = 0.00001;
  goal.period = 0.5;
  auto goal_future = client->async_send_goal(goal);
  ASSERT_TRUE(spin_until(executor, goal_future, std::chrono::seconds(1)));
  auto goal_handle = goal_future.get();
  ASSERT_NE(goal_handle, nullptr);

  const auto write_deadline = std::chrono::steady_clock::now() + std::chrono::seconds(2);
  while (fake->write_count.load() < 120 && std::chrono::steady_clock::now() < write_deadline) {
    executor.spin_some();
    std::this_thread::sleep_for(std::chrono::milliseconds(5));
  }
  ASSERT_GE(fake->write_count.load(), 120);
  {
    std::lock_guard<std::mutex> lock(fake->setpoint_mutex);
    EXPECT_LT(fake->minimum_position, fake->actual_position);
    EXPECT_GT(fake->maximum_position, fake->actual_position);
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

TEST_F(WavemakerNodeTest, ReturnToUprightRejectedWhileInactive)
{
  ReturnToUprightRig rig;
  ASSERT_NO_FATAL_FAILURE(rig.setup("/return_inactive_test", 0.30, false));

  const auto response = rig.call(0.0);
  EXPECT_EQ(response.status, ReturnToUpright::Response::CONTROLLER_INACTIVE);
}

TEST_F(WavemakerNodeTest, ReturnToUprightRejectedWhileGoalRunning)
{
  ReturnToUprightRig rig;
  ASSERT_NO_FATAL_FAILURE(rig.setup("/return_busy_test", 0.25, true));
  ASSERT_TRUE(rig.action_client->wait_for_action_server(std::chrono::seconds(1)));

  MoveWavemaker::Goal goal;
  goal.amplitude = 0.001;
  goal.period = 10.0;
  auto goal_future = rig.action_client->async_send_goal(goal);
  ASSERT_TRUE(spin_until(rig.executor, goal_future, std::chrono::seconds(1)));
  auto goal_handle = goal_future.get();
  ASSERT_NE(goal_handle, nullptr);

  const auto write_deadline = std::chrono::steady_clock::now() + std::chrono::seconds(1);
  while (rig.fake->write_count.load() == 0 && std::chrono::steady_clock::now() < write_deadline) {
    rig.executor.spin_some();
    std::this_thread::sleep_for(std::chrono::milliseconds(5));
  }
  ASSERT_GT(rig.fake->write_count.load(), 0);

  EXPECT_EQ(rig.call(0.0).status, ReturnToUpright::Response::BUSY);

  rig.action_client->async_cancel_goal(goal_handle);
  auto result_future = rig.action_client->async_get_result(goal_handle);
  ASSERT_TRUE(spin_until(rig.executor, result_future, std::chrono::seconds(2)));
  EXPECT_EQ(result_future.get().code, rclcpp_action::ResultCode::CANCELED);
}

TEST_F(WavemakerNodeTest, ReturnToUprightMovesToUprightPosition)
{
  ReturnToUprightRig rig;
  ASSERT_NO_FATAL_FAILURE(rig.setup("/return_success_test", 0.30, true));
  rig.fake->follow_setpoints = true;

  const auto response = rig.call(0.001);
  EXPECT_EQ(response.status, ReturnToUpright::Response::SUCCESS);
  EXPECT_NEAR(response.final_position, 0.25, 0.001);
  EXPECT_GE(rig.fake->halt_count, 1);
}

TEST_F(WavemakerNodeTest, ReturnToUprightAcceptsPaddleJustBelowMinimum)
{
  // Limits are [0.0, 0.5]; the default tolerance (2 mm) is allowed outside them.
  // 1 m/s keeps the 0.251 m return at the 1 s minimum duration.
  ReturnToUprightRig rig;
  ASSERT_NO_FATAL_FAILURE(rig.setup("/return_below_minimum_test", -0.001, true, 1.0));
  rig.fake->follow_setpoints = true;
  // The drive rejects targets outside its limits, so the first setpoint must be clamped.
  rig.fake->min_position = 0.0;
  rig.fake->max_position = 0.5;

  const auto response = rig.call(0.001);
  EXPECT_EQ(response.status, ReturnToUpright::Response::SUCCESS);
  EXPECT_NEAR(response.final_position, 0.25, 0.001);
}

TEST_F(WavemakerNodeTest, ReturnPublishesSetpointVelocityAndPosition)
{
  ReturnToUprightRig rig;
  ASSERT_NO_FATAL_FAILURE(rig.setup("/return_topics_test", 0.30, true));
  rig.fake->follow_setpoints = true;

  std::mutex mutex;
  std::vector<double> setpoints;
  int velocities = 0;
  int positions = 0;
  auto setpoint_sub = rig.client_node->create_subscription<std_msgs::msg::Float64>(
    "wavemaker_setpoint", 100, [&](std_msgs::msg::Float64::ConstSharedPtr msg) {
      std::lock_guard<std::mutex> lock(mutex);
      setpoints.push_back(msg->data);
    });
  auto velocity_sub = rig.client_node->create_subscription<std_msgs::msg::Float64>(
    "wavemaker_velocity", 100, [&](std_msgs::msg::Float64::ConstSharedPtr) {
      std::lock_guard<std::mutex> lock(mutex);
      ++velocities;
    });
  auto position_sub = rig.client_node->create_subscription<std_msgs::msg::Float64>(
    "wavemaker_position", 100, [&](std_msgs::msg::Float64::ConstSharedPtr) {
      std::lock_guard<std::mutex> lock(mutex);
      ++positions;
    });
  // Let the subscriptions match the node's publishers before the return starts.
  const auto discovery = std::chrono::steady_clock::now() + std::chrono::milliseconds(500);
  while (std::chrono::steady_clock::now() < discovery) {
    rig.executor.spin_some();
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }

  const auto response = rig.call(0.001);
  EXPECT_EQ(response.status, ReturnToUpright::Response::SUCCESS);
  for (int i = 0; i < 20; ++i) {  // deliver the last messages
    rig.executor.spin_some();
    std::this_thread::sleep_for(std::chrono::milliseconds(5));
  }

  std::lock_guard<std::mutex> lock(mutex);
  ASSERT_FALSE(setpoints.empty());
  EXPECT_GT(velocities, 0);
  EXPECT_GT(positions, 0);
  EXPECT_NEAR(setpoints.back(), 0.25, 0.001);  // metres, the upright position
}

TEST_F(WavemakerNodeTest, ReturnSendsPeakSpeedWithMarginAsDriveSpeedCap)
{
  // 0.05 m in the 1 s minimum duration: peak 1.875 * 0.05 = 0.09375 m/s, cap 1.2x that.
  ReturnToUprightRig rig;
  ASSERT_NO_FATAL_FAILURE(rig.setup("/return_cap_test", 0.30, true));
  rig.fake->follow_setpoints = true;

  const auto response = rig.call(0.001);
  EXPECT_EQ(response.status, ReturnToUpright::Response::SUCCESS);
  std::lock_guard<std::mutex> lock(rig.fake->setpoint_mutex);
  // The trajectory starts and ends at rest, but the drive gets the constant cap.
  EXPECT_NEAR(rig.fake->first_velocity, 1.2 * 0.09375, 1e-9);
  EXPECT_NEAR(rig.fake->last_velocity, 1.2 * 0.09375, 1e-9);
}

TEST_F(WavemakerNodeTest, DriveSpeedCapNeverExceedsDriveMaximum)
{
  // Peak 0.09375 m/s * 1.2 = 0.1125 m/s, above the drive's 0.105 m/s, so the cap is 0.105.
  ReturnToUprightRig rig;
  ASSERT_NO_FATAL_FAILURE(rig.setup("/return_cap_limit_test", 0.30, false));
  rig.fake->max_velocity = 0.105;
  ASSERT_EQ(
    rig.node->trigger_transition(lifecycle_msgs::msg::Transition::TRANSITION_ACTIVATE).id(),
    lifecycle_msgs::msg::State::PRIMARY_STATE_ACTIVE);
  rig.fake->follow_setpoints = true;

  const auto response = rig.call(0.001);
  EXPECT_EQ(response.status, ReturnToUpright::Response::SUCCESS);
  std::lock_guard<std::mutex> lock(rig.fake->setpoint_mutex);
  EXPECT_NEAR(rig.fake->first_velocity, 0.105, 1e-9);
}

TEST_F(WavemakerNodeTest, ControlLoopRunsAtActuatorUpdatePeriod)
{
  // A 50 ms actuator period means 20 setpoints per second instead of 100.
  // The return from 0.30 to 0.25 at 0.1 m/s takes the 1 s minimum duration.
  ReturnToUprightRig rig;
  ASSERT_NO_FATAL_FAILURE(rig.setup("/update_period_test", 0.30, true, 0.1, 50));
  rig.fake->follow_setpoints = true;

  const auto start = std::chrono::steady_clock::now();
  const auto response = rig.call(0.001);
  const double seconds =
    std::chrono::duration<double>(std::chrono::steady_clock::now() - start).count();
  EXPECT_EQ(response.status, ReturnToUpright::Response::SUCCESS);

  // About 20 writes per second of motion; at 100 Hz it would be about 100.
  const double writes_per_second = rig.fake->write_count.load() / seconds;
  EXPECT_GT(writes_per_second, 12.0);
  EXPECT_LT(writes_per_second, 28.0);
}

TEST_F(WavemakerNodeTest, ReturnToUprightRejectsPaddleBeyondLimitTolerance)
{
  ReturnToUprightRig rig;
  ASSERT_NO_FATAL_FAILURE(rig.setup("/return_beyond_tolerance_test", -0.003, true));

  const auto response = rig.call(0.0);
  EXPECT_EQ(response.status, ReturnToUpright::Response::POSITION_INVALID);
  EXPECT_EQ(rig.fake->write_count.load(), 0);
}

TEST_F(WavemakerNodeTest, ReturnToUprightTimesOutWhenActuatorDoesNotMove)
{
  ReturnToUprightRig rig;
  ASSERT_NO_FATAL_FAILURE(rig.setup("/return_timeout_test", 0.30, true));

  const auto response = rig.call(0.001);
  EXPECT_EQ(response.status, ReturnToUpright::Response::TIMEOUT);
  EXPECT_NEAR(response.final_position, 0.30, 1e-9);
  EXPECT_EQ(rig.fake->halt_count, 1);
}

TEST_F(WavemakerNodeTest, ReturnToUprightRejectedWhenActuatorFaulted)
{
  ReturnToUprightRig rig;
  ASSERT_NO_FATAL_FAILURE(rig.setup("/return_fault_start_test", 0.30, true));
  rig.fake->fault = true;

  const auto response = rig.call(0.0);
  EXPECT_EQ(response.status, ReturnToUpright::Response::ACTUATOR_FAULT);
  EXPECT_EQ(rig.fake->write_count.load(), 0);
}

TEST_F(WavemakerNodeTest, ReturnToUprightReportsFaultDuringMove)
{
  ReturnToUprightRig rig;
  ASSERT_NO_FATAL_FAILURE(rig.setup("/return_fault_move_test", 0.30, true));

  std::thread trigger([fake = rig.fake]() {
      std::this_thread::sleep_for(std::chrono::milliseconds(200));
      fake->fault = true;
    });
  const auto response = rig.call(0.001);
  trigger.join();

  EXPECT_EQ(response.status, ReturnToUpright::Response::ACTUATOR_FAULT);
  EXPECT_GT(rig.fake->write_count.load(), 0);
  EXPECT_EQ(rig.fake->halt_count, 1);
}

TEST_F(WavemakerNodeTest, ReturnToUprightRejectsPositionOutsideLimits)
{
  ReturnToUprightRig rig;
  ASSERT_NO_FATAL_FAILURE(rig.setup("/return_invalid_position_test", 0.30, true));
  rig.fake->actual_position = 0.9;

  const auto response = rig.call(0.0);
  EXPECT_EQ(response.status, ReturnToUpright::Response::POSITION_INVALID);
  EXPECT_EQ(rig.fake->write_count.load(), 0);
}

TEST_F(WavemakerNodeTest, DriverFaultReleasesResourcesAndAllowsReconfigure)
{
  FakeActuator * fake = nullptr;
  std::atomic<int> stops{0};
  std::atomic<int> destructions{0};
  int factory_calls = 0;
  auto factory = [&](rclcpp_lifecycle::LifecycleNode &) {
      auto actuator = std::make_unique<FakeActuator>();
      fake = actuator.get();
      fake->stops_seen = &stops;
      fake->destructions_seen = &destructions;
      ++factory_calls;
      return actuator;
    };
  auto node = std::make_shared<WavemakerNode>(node_options("/driver_fault_test", false), factory);
  ASSERT_EQ(
    node->trigger_transition(lifecycle_msgs::msg::Transition::TRANSITION_CONFIGURE).id(),
    lifecycle_msgs::msg::State::PRIMARY_STATE_INACTIVE);
  ASSERT_EQ(
    node->trigger_transition(lifecycle_msgs::msg::Transition::TRANSITION_ACTIVATE).id(),
    lifecycle_msgs::msg::State::PRIMARY_STATE_ACTIVE);
  rclcpp::executors::MultiThreadedExecutor executor;
  executor.add_node(node->get_node_base_interface());

  // The fault timer deactivates, on_deactivate returns ERROR, and on_error releases everything.
  fake->fault_callback_("test fault");
  fake = nullptr;  // destroyed by release_resources()
  ASSERT_TRUE(
    spin_until_state(
      executor, *node, lifecycle_msgs::msg::State::PRIMARY_STATE_UNCONFIGURED,
      std::chrono::seconds(2)));
  EXPECT_GE(stops.load(), 1);
  EXPECT_EQ(destructions.load(), 1);  // on_error freed the actuator
  EXPECT_EQ(factory_calls, 1);

  // Configuring again creates a new actuator and the node can be activated.
  ASSERT_EQ(
    node->trigger_transition(lifecycle_msgs::msg::Transition::TRANSITION_CONFIGURE).id(),
    lifecycle_msgs::msg::State::PRIMARY_STATE_INACTIVE);
  EXPECT_EQ(factory_calls, 2);
  ASSERT_EQ(
    node->trigger_transition(lifecycle_msgs::msg::Transition::TRANSITION_ACTIVATE).id(),
    lifecycle_msgs::msg::State::PRIMARY_STATE_ACTIVE);
  ASSERT_NE(fake, nullptr);
  EXPECT_TRUE(fake->started);
}

TEST_F(WavemakerNodeTest, SetpointWriteFailureAbortsGoalAndDeactivates)
{
  FakeActuator * fake = nullptr;
  std::atomic<int> stops{0};
  auto factory = [&](rclcpp_lifecycle::LifecycleNode &) {
      auto actuator = std::make_unique<FakeActuator>();
      fake = actuator.get();
      fake->stops_seen = &stops;
      return actuator;
    };
  auto node = std::make_shared<WavemakerNode>(node_options("/write_failure_test", true), factory);
  ASSERT_EQ(
    node->trigger_transition(lifecycle_msgs::msg::Transition::TRANSITION_CONFIGURE).id(),
    lifecycle_msgs::msg::State::PRIMARY_STATE_INACTIVE);
  ASSERT_EQ(
    node->trigger_transition(lifecycle_msgs::msg::Transition::TRANSITION_ACTIVATE).id(),
    lifecycle_msgs::msg::State::PRIMARY_STATE_ACTIVE);

  auto client_node = std::make_shared<rclcpp::Node>(
    "write_failure_client", node_options("/write_failure_test", true));
  auto client = rclcpp_action::create_client<MoveWavemaker>(client_node, "move_wavemaker");
  rclcpp::executors::MultiThreadedExecutor executor;
  executor.add_node(node->get_node_base_interface());
  executor.add_node(client_node);
  ASSERT_TRUE(client->wait_for_action_server(std::chrono::seconds(1)));

  fake->reject_writes = true;
  MoveWavemaker::Goal goal;
  goal.positions = {0.25, 0.26};
  goal.sample_interval = 0.1;
  auto goal_future = client->async_send_goal(goal);
  ASSERT_TRUE(spin_until(executor, goal_future, std::chrono::seconds(1)));
  auto goal_handle = goal_future.get();
  ASSERT_NE(goal_handle, nullptr);

  auto result_future = client->async_get_result(goal_handle);
  ASSERT_TRUE(spin_until(executor, result_future, std::chrono::seconds(2)));
  const auto result = result_future.get();
  EXPECT_EQ(result.code, rclcpp_action::ResultCode::ABORTED);
  EXPECT_NE(result.result->message.find("Actuator rejected setpoint"), std::string::npos);

  // The failure is routed through the fault path, so the node leaves the active state.
  fake = nullptr;  // destroyed by release_resources()
  ASSERT_TRUE(
    spin_until_state(
      executor, *node, lifecycle_msgs::msg::State::PRIMARY_STATE_UNCONFIGURED,
      std::chrono::seconds(2)));
  EXPECT_GE(stops.load(), 1);
}

TEST_F(WavemakerNodeTest, ConfigureFailsWhenReturnSpeedExceedsDriveMaximum)
{
  auto factory = [](rclcpp_lifecycle::LifecycleNode &) {
      auto actuator = std::make_unique<FakeActuator>();
      actuator->max_velocity = 0.05;  // below the default return speed of 0.1 m/s
      return actuator;
    };
  auto node = std::make_shared<WavemakerNode>(node_options("/return_speed_test", false), factory);
  EXPECT_EQ(
    node->trigger_transition(lifecycle_msgs::msg::Transition::TRANSITION_CONFIGURE).id(),
    lifecycle_msgs::msg::State::PRIMARY_STATE_UNCONFIGURED);
}

TEST_F(WavemakerNodeTest, RegularWaveAboveDriveSpeedIsRejected)
{
  // Piston in 1 m water: a 0.05 m wave at 1 s needs about 0.16 m/s.
  MoveWavemaker::Goal fast;
  fast.amplitude = 0.05;
  fast.period = 1.0;
  EXPECT_FALSE(goal_accepted_with_speed_limit("/fast_wave_test", false, 0.06, fast));

  MoveWavemaker::Goal slow;
  slow.amplitude = 0.001;
  slow.period = 10.0;
  EXPECT_TRUE(goal_accepted_with_speed_limit("/slow_wave_test", false, 0.06, slow));
}

TEST_F(WavemakerNodeTest, PregeneratedGoalAboveDriveSpeedIsRejected)
{
  MoveWavemaker::Goal fast;  // 0.01 m per 0.1 s = 0.1 m/s
  fast.positions = {0.25, 0.26};
  fast.sample_interval = 0.1;
  EXPECT_FALSE(goal_accepted_with_speed_limit("/fast_samples_test", true, 0.06, fast));

  MoveWavemaker::Goal slow;  // 0.01 m per 0.5 s = 0.02 m/s
  slow.positions = {0.25, 0.26};
  slow.sample_interval = 0.5;
  EXPECT_TRUE(goal_accepted_with_speed_limit("/slow_samples_test", true, 0.06, slow));
}

TEST_F(WavemakerNodeTest, ConfigureFailsForInvalidTransferGain)
{
  for (const double gain : {0.0, -1.0, std::numeric_limits<double>::quiet_NaN(),
      std::numeric_limits<double>::infinity()})
  {
    auto options = node_options("/transfer_gain_test", false);
    options.append_parameter_override("wavemaker_transfer_gain", gain);
    auto node = std::make_shared<WavemakerNode>(
      options, [](rclcpp_lifecycle::LifecycleNode &) {return std::make_unique<FakeActuator>();});
    EXPECT_EQ(
      node->trigger_transition(lifecycle_msgs::msg::Transition::TRANSITION_CONFIGURE).id(),
      lifecycle_msgs::msg::State::PRIMARY_STATE_UNCONFIGURED) << "gain " << gain;
  }
}

TEST_F(WavemakerNodeTest, TransferGainScalesRequiredStroke)
{
  // The fast wave of RegularWaveAboveDriveSpeedIsRejected needs about 0.16 m/s; a gain of 4
  // divides the stroke, and with it the speed, by 4.
  MoveWavemaker::Goal fast;
  fast.amplitude = 0.05;
  fast.period = 1.0;
  EXPECT_FALSE(
    goal_accepted_with_speed_limit(
      "/gain_low_test", false, 0.06, fast, {rclcpp::Parameter("wavemaker_transfer_gain", 1.0)}));
  EXPECT_TRUE(
    goal_accepted_with_speed_limit(
      "/gain_high_test", false, 0.06, fast, {rclcpp::Parameter("wavemaker_transfer_gain", 4.0)}));
}

TEST_F(WavemakerNodeTest, WaterDepthChangeTakesEffectAtNextConfigure)
{
  // A 0.01 m, 10 s wave needs about 0.04 m/s in 1 m of water and about 0.08 m/s in 0.2 m.
  MoveWavemaker::Goal goal;
  goal.amplitude = 0.01;
  goal.period = 10.0;
  auto node = speed_limited_node("/water_depth_test", false, 0.06);
  EXPECT_TRUE(goal_accepted(node, "/water_depth_test", false, goal));

  ASSERT_EQ(
    node->trigger_transition(lifecycle_msgs::msg::Transition::TRANSITION_CLEANUP).id(),
    lifecycle_msgs::msg::State::PRIMARY_STATE_UNCONFIGURED);
  ASSERT_TRUE(node->set_parameter(rclcpp::Parameter("water_depth", 0.2)).successful);
  EXPECT_FALSE(goal_accepted(node, "/water_depth_test", false, goal));
}

TEST_F(WavemakerNodeTest, PublishesSetpointInActuatorUnits)
{
  ReturnToUprightRig rig;
  ASSERT_NO_FATAL_FAILURE(rig.setup("/actuator_setpoint_test", 0.30, true));
  rig.fake->follow_setpoints = true;
  rig.fake->units_per_m = 1000.0;  // distinguishes actuator units from metres

  std::mutex mutex;
  std::vector<double> setpoints_m;
  std::vector<double> actuator_setpoints;
  auto setpoint_sub = rig.client_node->create_subscription<std_msgs::msg::Float64>(
    "wavemaker_setpoint", 1000, [&](std_msgs::msg::Float64::ConstSharedPtr msg) {
      std::lock_guard<std::mutex> lock(mutex);
      setpoints_m.push_back(msg->data);
    });
  auto actuator_sub = rig.client_node->create_subscription<std_msgs::msg::Float64>(
    "wavemaker_actuator_setpoint", 1000, [&](std_msgs::msg::Float64::ConstSharedPtr msg) {
      std::lock_guard<std::mutex> lock(mutex);
      actuator_setpoints.push_back(msg->data);
    });
  const auto discovery = std::chrono::steady_clock::now() + std::chrono::milliseconds(500);
  while (std::chrono::steady_clock::now() < discovery) {
    rig.executor.spin_some();
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }

  const auto response = rig.call(0.001);
  EXPECT_EQ(response.status, ReturnToUpright::Response::SUCCESS);
  for (int i = 0; i < 20; ++i) {  // deliver the last messages
    rig.executor.spin_some();
    std::this_thread::sleep_for(std::chrono::milliseconds(5));
  }

  std::lock_guard<std::mutex> lock(mutex);
  std::lock_guard<std::mutex> fake_lock(rig.fake->setpoint_mutex);
  ASSERT_FALSE(actuator_setpoints.empty());
  // Exactly what the drive was sent, and the metre setpoint converted.
  EXPECT_EQ(actuator_setpoints, rig.fake->written_positions);
  ASSERT_EQ(actuator_setpoints.size(), setpoints_m.size());
  for (std::size_t i = 0; i < actuator_setpoints.size(); ++i) {
    EXPECT_NEAR(actuator_setpoints[i], setpoints_m[i] * 1000.0, 1e-9) << "sample " << i;
  }
  EXPECT_NEAR(actuator_setpoints.back(), 250.0, 1.0);  // upright, 0.25 m
}
