#include "indradrive_actuator.hpp"

#include <chrono>
#include <cstdint>
#include <exception>
#include <stdexcept>
#include <thread>

namespace wavemaker_controller {

IndraDriveActuator::IndraDriveActuator(rclcpp_lifecycle::LifecycleNode & node)
: lead_m_per_degree_(0.0), actuator_upright_angle_deg_(0.0),
  wavemaker_position_offset_m_(0.0), poll_interval_ms_(50)
{
  node.declare_parameter<std::string>("driver_address", "");
  node.declare_parameter<int>("driver_port", 502);
  node.declare_parameter<int>("driver_unit_id", 1);
  node.declare_parameter<int>("input_base_reg", 0);
  node.declare_parameter<int>("input_word_count", 0);
  node.declare_parameter<int>("output_base_reg", 0x0800);
  node.declare_parameter<int>("output_word_count", 0);
  node.declare_parameter<bool>("input_uses_fc4", true);
  node.declare_parameter<int>("status_base_reg", 0);
  node.declare_parameter<int>("status_word_count", 0);
  node.declare_parameter<bool>("status_uses_fc4", true);
  node.declare_parameter<int>("poll_interval_ms", 50);
  node.declare_parameter<int>("timeout_ms", 1000);
  node.declare_parameter<int>("reconnect_ms", 2000);
  node.declare_parameter<bool>("write_only_dirty", true);
  node.declare_parameter<int>("profibus_address", 2);
  node.declare_parameter<double>("min_position_deg", -550.0);
  node.declare_parameter<double>("max_position_deg", 500.0);
  node.declare_parameter<double>("max_velocity_rpm", 100.0);
  node.declare_parameter<double>("in_position_tol_deg", 0.1);
  node.declare_parameter<int>("step_timeout_ms", 10000);
  node.declare_parameter<std::string>("actuator_drive_type", "linear");
  node.declare_parameter<double>("actuator_lead_m_per_degree", 0.0);

  lead_m_per_degree_ = node.get_parameter("actuator_lead_m_per_degree").as_double();
  actuator_upright_angle_deg_ = node.get_parameter("actuator_upright_angle_deg").as_double();
  wavemaker_position_offset_m_ = node.get_parameter("wavemaker_upright_position_m").as_double();
  poll_interval_ms_ = node.get_parameter("poll_interval_ms").as_int();
  actuator_drive_type_ = node.get_parameter("actuator_drive_type").as_string();
  if (actuator_drive_type_ != "linear" && actuator_drive_type_ != "angular") {
    throw std::invalid_argument("actuator_drive_type must be 'linear' or 'angular'");
  }
  if (actuator_drive_type_ == "angular" && lead_m_per_degree_ <= 0.0) {
    throw std::invalid_argument(
      "actuator_lead_m_per_degree must be positive for an angular actuator");
  }

  const std::string host = node.get_parameter("driver_address").as_string();
  const int port = node.get_parameter("driver_port").as_int();
  const int unit_id = node.get_parameter("driver_unit_id").as_int();
  const int input_base = node.get_parameter("input_base_reg").as_int();
  const int input_count = node.get_parameter("input_word_count").as_int();
  const int output_base = node.get_parameter("output_base_reg").as_int();
  const int output_count = node.get_parameter("output_word_count").as_int();
  const int status_base = node.get_parameter("status_base_reg").as_int();
  const int status_count = node.get_parameter("status_word_count").as_int();

  if (host.empty()) {
    throw std::invalid_argument("driver_address must be set before configuring the drive");
  }
  if (port < 1 || port > 65535 || unit_id < 0 || unit_id > 255 ||
      input_base < 0 || input_base > 65535 || input_count <= 0 || input_count > 65535 ||
      output_base < 0 || output_base > 65535 || output_count <= 0 || output_count > 65535 ||
      status_base < 0 || status_base > 65535 || status_count < 0 || status_count > 65535) {
    throw std::invalid_argument("invalid Modbus register or connection configuration");
  }

  const int profibus_address = node.get_parameter("profibus_address").as_int();
  if (profibus_address < 0 || profibus_address > 125) {
    throw std::invalid_argument("profibus_address must be between 0 and 125");
  }

  mgate::GatewayConfig gateway_config;
  gateway_config.host = host;
  gateway_config.port = static_cast<std::uint16_t>(port);
  gateway_config.unit_id = static_cast<std::uint8_t>(unit_id);
  gateway_config.input_base_reg = static_cast<std::uint16_t>(input_base);
  gateway_config.input_word_count = static_cast<std::uint16_t>(input_count);
  gateway_config.output_base_reg = static_cast<std::uint16_t>(output_base);
  gateway_config.output_word_count = static_cast<std::uint16_t>(output_count);
  gateway_config.input_uses_fc4 = node.get_parameter("input_uses_fc4").as_bool();
  gateway_config.status_base_reg = static_cast<std::uint16_t>(status_base);
  gateway_config.status_word_count = static_cast<std::uint16_t>(status_count);
  gateway_config.status_uses_fc4 = node.get_parameter("status_uses_fc4").as_bool();
  gateway_config.poll_interval_ms = node.get_parameter("poll_interval_ms").as_int();
  gateway_config.timeout_ms = node.get_parameter("timeout_ms").as_int();
  gateway_config.reconnect_ms = node.get_parameter("reconnect_ms").as_int();
  gateway_config.write_only_dirty = node.get_parameter("write_only_dirty").as_bool();

  if (gateway_config.poll_interval_ms <= 0 || gateway_config.timeout_ms <= 0 ||
      gateway_config.reconnect_ms <= 0) {
    throw std::invalid_argument("poll_interval_ms, timeout_ms, and reconnect_ms must be positive");
  }

  mgate::IndraDriveConfig drive_config;
  drive_config.profibus_address = profibus_address;
  drive_config.min_position_deg = node.get_parameter("min_position_deg").as_double();
  drive_config.max_position_deg = node.get_parameter("max_position_deg").as_double();
  drive_config.max_velocity_rpm = node.get_parameter("max_velocity_rpm").as_double();
  drive_config.in_position_tol_deg = node.get_parameter("in_position_tol_deg").as_double();
  drive_config.step_timeout_ms = node.get_parameter("step_timeout_ms").as_int();

  if (drive_config.min_position_deg >= drive_config.max_position_deg ||
      drive_config.max_velocity_rpm <= 0.0 || drive_config.in_position_tol_deg < 0.0 ||
      drive_config.step_timeout_ms <= 0) {
    throw std::invalid_argument("invalid IndraDrive limits or timeout");
  }

  mgate_driver_ = std::make_unique<mgate::MGateDriver>(gateway_config);
  indradrive_ = std::make_unique<mgate::IndraDrive>(*mgate_driver_, drive_config);
}

bool IndraDriveActuator::start(const rclcpp::Logger & logger)
{
  mgate_driver_->start();
  if (!mgate_driver_->wait_for_data(5000)) {
    mgate_driver_->stop();
    RCLCPP_ERROR(logger, "Failed to receive data from the drive within 5 seconds.");
    return false;
  }

  const auto first_stats = mgate_driver_->stats();
  std::this_thread::sleep_for(std::chrono::milliseconds(200));
  const auto second_stats = mgate_driver_->stats();

  if (second_stats.cycles <= first_stats.cycles) {
    RCLCPP_ERROR(logger, "MGate polling is not progressing");
    mgate_driver_->stop();
    return false;
  }
  if (second_stats.read_errors > first_stats.read_errors ||
      second_stats.write_errors > first_stats.write_errors) {
    RCLCPP_ERROR(logger, "MGate communication errors increased");
    mgate_driver_->stop();
    return false;
  }
  if (!second_stats.last_error.empty()) {
    RCLCPP_ERROR(logger, "MGate reported an error: %s", second_stats.last_error.c_str());
    mgate_driver_->stop();
    return false;
  }

  const auto poll_interval_us = static_cast<long long>(poll_interval_ms_) * 1000;
  if (second_stats.last_cycle_us >= poll_interval_us) {
    RCLCPP_ERROR(logger, "MGate cycle time is too long");
    mgate_driver_->stop();
    return false;
  }

  if (!indradrive_->slave_live()) {
    RCLCPP_ERROR(logger, "Drive is not live on PROFIBUS");
    mgate_driver_->stop();
    return false;
  }

  RCLCPP_INFO(logger, "Drive status: %s", indradrive_->describe().c_str());
  return true;
}

void IndraDriveActuator::stop()
{
  if (indradrive_) {
    indradrive_->disable();
  }
  if (mgate_driver_) {
    mgate_driver_->stop();
  }
}

bool IndraDriveActuator::is_live() const
{
  return indradrive_ && indradrive_->slave_live();
}

std::string IndraDriveActuator::status() const
{
  return indradrive_ ? indradrive_->describe() : std::string{};
}

double IndraDriveActuator::actual_position_m() const
{
  const double position_deg = indradrive_->position_deg();
  return actuator_drive_type_ == "angular" ?
    wavemaker_position_offset_m_ +
    (position_deg - actuator_upright_angle_deg_) * lead_m_per_degree_ : position_deg;
}

ActuatorSetpoint IndraDriveActuator::to_actuator_setpoint(
  double position_m, double velocity_mps) const
{
  if (actuator_drive_type_ == "angular") {
    const double position_deg = actuator_upright_angle_deg_ +
      (position_m - wavemaker_position_offset_m_) / lead_m_per_degree_;
    return {position_deg, velocity_mps / lead_m_per_degree_};
  }
  return {position_m, velocity_mps};
}

}  // namespace wavemaker_controller
