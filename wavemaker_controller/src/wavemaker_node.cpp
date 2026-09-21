#include <memory>
#include <string>

#include "lifecycle_msgs/msg/transition.hpp"
#include "rclcpp/rclcpp.hpp"
#include "rclcpp_lifecycle/lifecycle_node.hpp"
#include "bondcpp/bond.hpp"



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
    
  }
 

  CallbackReturn on_configure(const rclcpp_lifecycle::State & previous_state) override
  {
    wavemaker_type_ = get_parameter("wavemaker_type").as_string();
    driver_address_ = get_parameter("driver_address").as_string();
    wavemaker_id_ = get_parameter("wavemaker_id").as_string();
    wavemaker_maximum_ = get_parameter("wavemaker_maximum").as_double();
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

  std::string wavemaker_type_;
  std::string driver_address_;
  std::string wavemaker_id_;
  double wavemaker_maximum_;
  // std::unique_ptr<WavemakerDriver> driver_;
  // rclcpp::TimerBase::SharedPtr command_timer_;
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  auto node = std::make_shared<WavemakerNode>();

  rclcpp::executors::SingleThreadedExecutor executor;
  executor.add_node(node->get_node_base_interface());
  executor.spin();

  rclcpp::shutdown();
  return 0;
}