#pragma once

#include <string>

namespace wavemaker_controller
{

// Peak of ds for the quintic blend: v_max = 1.875 * distance / duration.
constexpr double kQuinticPeakVelocityFactor = 1.875;

struct Blend
{
  double s;   // blend factor, 0 -> 1
  double ds;  // d(s)/dt
};

// Quintic smoothstep; duration must be > 0.
Blend quintic_blend(double elapsed, double duration);

// Beji 2013 dispersion relation; returns the dimensionless wavenumber k*h.
double solve_dispersion(double omega, double depth, double g = 9.81);

// Stroke needed for wave height target_height; type is "piston" or "flap".
double compute_stroke(double mu, double target_height, const std::string & type);

}  // namespace wavemaker_controller
