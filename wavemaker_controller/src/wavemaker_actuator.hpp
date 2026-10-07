#pragma once

#include <functional>
#include <string>

#include "rclcpp/rclcpp.hpp"

namespace wavemaker_controller
{

struct ActuatorSetpoint
{
  double position;
  double velocity;
};

class WavemakerActuator
{
public:
  using FaultCallback = std::function<void(const std::string &)>;

  virtual ~WavemakerActuator() = default;

  virtual void set_fault_callback(FaultCallback callback) = 0;
  virtual bool start(const rclcpp::Logger & logger) = 0;
  // Stop moving and hold position, staying enabled so the next setpoint is followed.
  virtual void halt() = 0;
  virtual void stop() = 0;
  virtual bool is_live() const = 0;
  virtual bool faulted() const = 0;
  virtual std::string fault_reason() const = 0;
  virtual std::string status() const = 0;
  virtual double actual_position_m() const = 0;
  virtual ActuatorSetpoint to_actuator_setpoint(double position_m, double velocity_mps) const = 0;
  virtual bool write_setpoint(const ActuatorSetpoint & setpoint) = 0;
  // Fastest paddle speed the drive accepts, in m/s; write_setpoint() fails above it.
  virtual double max_velocity_mps() const = 0;
  // How often new setpoints actually reach the drive, in ms. The control loops run at this
  // period, because writing faster only overwrites setpoints before they are sent.
  virtual int update_period_ms() const = 0;
};

}  // namespace wavemaker_controller
