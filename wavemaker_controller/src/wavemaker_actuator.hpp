#pragma once

#include <string>

#include "rclcpp/rclcpp.hpp"

namespace wavemaker_controller {

struct ActuatorSetpoint
{
  double position;
  double velocity;
};

class WavemakerActuator
{
public:
  virtual ~WavemakerActuator() = default;

  virtual bool start(const rclcpp::Logger & logger) = 0;
  virtual void stop() = 0;
  virtual bool is_live() const = 0;
  virtual std::string status() const = 0;
  virtual double actual_position_m() const = 0;
  virtual ActuatorSetpoint to_actuator_setpoint(double position_m, double velocity_mps) const = 0;
  virtual bool write_setpoint(const ActuatorSetpoint & setpoint) = 0;
};

}  // namespace wavemaker_controller
