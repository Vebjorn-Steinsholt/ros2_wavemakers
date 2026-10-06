#include "wave_trajectory.hpp"

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <utility>
#include <vector>

#include "wave_math.hpp"

namespace wavemaker_controller
{

namespace
{

// Linear interpolation of positions; returns true once elapsed reaches duration.
// positions must not be empty.
bool sample_pregenerated(
  const std::vector<double> & positions, double sample_interval, double elapsed,
  double duration, double & position, double & velocity)
{
  if (elapsed >= duration) {
    position = positions.back();
    velocity = 0.0;
    return true;
  }

  const double sample = elapsed / sample_interval;
  // Rounding can put sample at n - 1 just below duration; keep index + 1 in range.
  const auto index = std::min(static_cast<std::size_t>(sample), positions.size() - 2);
  const double fraction = sample - static_cast<double>(index);
  const double delta = positions[index + 1] - positions[index];
  position = positions[index] + fraction * delta;
  velocity = delta / sample_interval;
  return false;
}

}  // namespace

void WaveTrajectory::configure(
  const WavemakerGeometry & geometry, double wave_amplitude, double period,
  double start_position)
{
  omega_ = 2.0 * M_PI / period;
  const double mu = solve_dispersion(omega_, geometry.water_depth);
  const double stroke = compute_stroke(mu, wave_amplitude * 2, geometry.type);
  amplitude_ = stroke / 2.0;
  if (geometry.type == "flap") {
    amplitude_ *= geometry.flap_attachment_height / geometry.water_depth;
  }
  upright_position_ = geometry.upright_position;
  upright_is_minimum_ = geometry.upright_is_minimum;
  start_position_ = start_position;
  startup_duration_ = period;
}

double WaveTrajectory::required_minimum() const
{
  return upright_is_minimum_ ? upright_position_ : upright_position_ - amplitude_;
}

double WaveTrajectory::required_maximum() const
{
  return upright_position_ + amplitude_ * (upright_is_minimum_ ? 2.0 : 1.0);
}

double WaveTrajectory::peak_velocity() const
{
  // In the blend, velocity = ds * (wave - start) + s * wave_velocity with s <= 1, so it is
  // bounded by the blend's peak ds times the largest wave-to-start distance plus the wave's.
  const double wave_peak = amplitude_ * omega_;
  const double distance = std::max(
    std::abs(required_maximum() - start_position_), std::abs(required_minimum() - start_position_));
  return wave_peak + kQuinticPeakVelocityFactor * distance / startup_duration_;
}

void WaveTrajectory::sample(double elapsed, double & position, double & velocity) const
{
  // Phase is continuous across the blend only because startup_duration_ equals the period.
  const double wave_time = (upright_is_minimum_ && elapsed < startup_duration_) ?
    elapsed : elapsed - startup_duration_;

  double wave_position;
  double wave_velocity;
  if (upright_is_minimum_) {
    wave_position = upright_position_ + amplitude_ * (1.0 - std::cos(omega_ * wave_time));
    wave_velocity = amplitude_ * omega_ * std::sin(omega_ * wave_time);
  } else {
    wave_position = upright_position_ + amplitude_ * std::sin(omega_ * wave_time);
    wave_velocity = amplitude_ * omega_ * std::cos(omega_ * wave_time);
  }

  if (elapsed < startup_duration_) {
    const Blend b = quintic_blend(elapsed, startup_duration_);
    position = start_position_ + b.s * (wave_position - start_position_);
    velocity = b.ds * (wave_position - start_position_) + b.s * wave_velocity;
  } else {
    position = wave_position;
    velocity = wave_velocity;
  }
}

void PregeneratedWaveTrajectory::configure(
  std::vector<double> positions, double sample_interval, double start_position,
  double blend_duration)
{
  positions_ = std::move(positions);
  sample_interval_ = sample_interval;
  start_position_ = start_position;
  blend_duration_ = blend_duration;
}

double PregeneratedWaveTrajectory::duration() const
{
  return blend_duration_ + sample_interval_ * static_cast<double>(positions_.size() - 1);
}

double PregeneratedWaveTrajectory::peak_velocity() const
{
  double peak = 0.0;
  if (blend_duration_ > 0.0) {
    peak = kQuinticPeakVelocityFactor * std::abs(positions_.front() - start_position_) /
      blend_duration_;
  }
  for (std::size_t i = 1; i < positions_.size(); ++i) {
    peak = std::max(peak, std::abs(positions_[i] - positions_[i - 1]) / sample_interval_);
  }
  return peak;
}

bool PregeneratedWaveTrajectory::sample(double elapsed, double & position, double & velocity) const
{
  if (elapsed < blend_duration_) {
    const Blend b = quintic_blend(elapsed, blend_duration_);
    const double delta = positions_.front() - start_position_;
    position = start_position_ + b.s * delta;
    velocity = b.ds * delta;
    return false;
  }
  return sample_pregenerated(
    positions_, sample_interval_, elapsed - blend_duration_, duration() - blend_duration_,
    position, velocity);
}

}  // namespace wavemaker_controller
