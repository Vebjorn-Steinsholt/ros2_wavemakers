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

// Stroke at the still-water level needed for wave height target_height; type is "piston" or
// "flap". hinge_fraction is a flap's hinge height above the floor divided by the water depth
// (0 = hinged at the floor, must be < 1).
double compute_stroke(
  double mu, double target_height, const std::string & type, double hinge_fraction = 0.0);

}  // namespace wavemaker_controller
