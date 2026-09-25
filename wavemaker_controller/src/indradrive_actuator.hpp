#pragma once

#include <memory>
#include <string>

#include "wavemaker_actuator.hpp"
#include "rclcpp_lifecycle/lifecycle_node.hpp"
#include "mgate/indradrive.h"
#include "mgate/mgate_driver.h"

namespace wavemaker_controller {

class IndraDriveActuator final : public WavemakerActuator
{
public:
  explicit IndraDriveActuator(rclcpp_lifecycle::LifecycleNode & node);

  bool start(const rclcpp::Logger & logger) override;
  void stop() override;
  bool is_live() const override;
  std::string status() const override;
  double actual_position_m() const override;
  ActuatorSetpoint to_actuator_setpoint(double position_m, double velocity_mps) const override;
  bool write_setpoint(const ActuatorSetpoint & setpoint) override;

private:
  std::unique_ptr<mgate::MGateDriver> mgate_driver_;
  std::unique_ptr<mgate::IndraDrive> indradrive_;
  std::string actuator_drive_type_;
  double lead_m_per_degree_;
  double actuator_upright_angle_deg_;
  double wavemaker_position_offset_m_;
  int poll_interval_ms_;
};

}  // namespace wavemaker_controller
