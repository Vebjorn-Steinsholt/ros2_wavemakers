#include <gtest/gtest.h>

#include <algorithm>
#include <cmath>
#include <limits>
#include <vector>

#include "wave_math.hpp"
#include "wave_trajectory.hpp"

using wavemaker_controller::PregeneratedWaveTrajectory;
using wavemaker_controller::WavemakerGeometry;
using wavemaker_controller::WaveTrajectory;

namespace
{

// 0.05 m blend from 0.30 to 0.25 over 1 s, then three samples 0.1 s apart.
PregeneratedWaveTrajectory blended_trajectory()
{
  PregeneratedWaveTrajectory trajectory;
  trajectory.configure({0.25, 0.26, 0.27}, 0.1, 0.30, 1.0);
  return trajectory;
}

// Paddle amplitude at the attachment point for a symmetric regular wave.
double paddle_amplitude(const WavemakerGeometry & geometry, double wave_height, double period)
{
  WaveTrajectory trajectory;
  trajectory.configure(geometry, wave_height, period, geometry.upright_position);
  return trajectory.required_maximum() - geometry.upright_position;
}

}  // namespace

TEST(PregeneratedWaveTrajectoryTest, DurationIncludesBlend)
{
  EXPECT_NEAR(blended_trajectory().duration(), 1.2, 1e-12);
}

TEST(PregeneratedWaveTrajectoryTest, BlendStartsAtRestOnStartPosition)
{
  double position = 0.0;
  double velocity = 1.0;
  EXPECT_FALSE(blended_trajectory().sample(0.0, position, velocity));
  EXPECT_NEAR(position, 0.30, 1e-12);
  EXPECT_NEAR(velocity, 0.0, 1e-12);
}

TEST(PregeneratedWaveTrajectoryTest, BlendMidpointFollowsQuinticProfile)
{
  double position = 0.0;
  double velocity = 0.0;
  EXPECT_FALSE(blended_trajectory().sample(0.5, position, velocity));
  // s(0.5) = 0.5 and ds(0.5) = 1.875 / duration for the quintic blend.
  EXPECT_NEAR(position, 0.275, 1e-12);
  EXPECT_NEAR(velocity, 1.875 * -0.05, 1e-12);
}

TEST(PregeneratedWaveTrajectoryTest, SamplesStartWhenBlendEnds)
{
  double position = 0.0;
  double velocity = 0.0;
  EXPECT_FALSE(blended_trajectory().sample(1.0, position, velocity));
  EXPECT_NEAR(position, 0.25, 1e-12);
  EXPECT_NEAR(velocity, 0.1, 1e-9);

  EXPECT_FALSE(blended_trajectory().sample(1.15, position, velocity));
  EXPECT_NEAR(position, 0.265, 1e-9);
}

TEST(PregeneratedWaveTrajectoryTest, CompletesOnLastSampleAtRest)
{
  const auto trajectory = blended_trajectory();
  double position = 0.0;
  double velocity = 1.0;
  EXPECT_TRUE(trajectory.sample(trajectory.duration(), position, velocity));
  EXPECT_NEAR(position, 0.27, 1e-12);
  EXPECT_NEAR(velocity, 0.0, 1e-12);
}

TEST(PregeneratedWaveTrajectoryTest, ZeroBlendPlaysSamplesImmediately)
{
  PregeneratedWaveTrajectory trajectory;
  trajectory.configure({0.25, 0.26}, 0.1, 0.25, 0.0);
  double position = 0.0;
  double velocity = 0.0;
  EXPECT_FALSE(trajectory.sample(0.0, position, velocity));
  EXPECT_NEAR(position, 0.25, 1e-12);
  EXPECT_NEAR(velocity, 0.1, 1e-9);
}

TEST(PregeneratedWaveTrajectoryTest, SingleSampleCompletesImmediately)
{
  PregeneratedWaveTrajectory trajectory;
  trajectory.configure({0.25}, 0.1, 0.25, 0.0);
  double position = 0.0;
  double velocity = 1.0;
  EXPECT_TRUE(trajectory.sample(0.0, position, velocity));
  EXPECT_NEAR(position, 0.25, 1e-12);
  EXPECT_NEAR(velocity, 0.0, 1e-12);
}

TEST(PregeneratedWaveTrajectoryTest, IndexStaysInRangeJustBeforeEnd)
{
  // With 10 samples 0.07 s apart, the largest time below duration() divides to exactly 9.0,
  // which without the clamp reads positions[10].
  std::vector<double> positions;
  for (int i = 0; i < 10; ++i) {
    positions.push_back(0.2 + 0.01 * i);
  }
  PregeneratedWaveTrajectory trajectory;
  trajectory.configure(positions, 0.07, positions.front(), 0.0);

  const double elapsed = std::nextafter(trajectory.duration(), 0.0);
  ASSERT_GE(elapsed / 0.07, 9.0);  // the rounding case this test is about

  double position = 0.0;
  double velocity = 0.0;
  EXPECT_FALSE(trajectory.sample(elapsed, position, velocity));
  EXPECT_NEAR(position, positions.back(), 1e-9);
  EXPECT_NEAR(velocity, 0.01 / 0.07, 1e-9);
}

TEST(PregeneratedWaveTrajectoryTest, PeakVelocityCoversBlendAndSamples)
{
  // Blend: 1.875 * 0.05 / 1.0 = 0.09375 m/s. Samples: 0.01 / 0.1 = 0.1 m/s.
  EXPECT_NEAR(blended_trajectory().peak_velocity(), 0.1, 1e-12);

  PregeneratedWaveTrajectory steep_blend;
  steep_blend.configure({0.25, 0.26}, 0.1, 0.45, 1.0);  // 0.2 m blend in 1 s
  EXPECT_NEAR(steep_blend.peak_velocity(), 1.875 * 0.2, 1e-12);
}

TEST(WaveTrajectoryTest, PeakVelocityBoundsEverySample)
{
  for (const bool upright_is_minimum : {false, true}) {
    for (const double start : {0.25, 0.20, 0.32}) {
      WaveTrajectory trajectory;
      trajectory.configure({"piston", 1.0, 0.0, 0.25, upright_is_minimum}, 0.04, 2.0, start);
      const double bound = trajectory.peak_velocity();

      double largest = 0.0;
      for (double t = 0.0; t < 6.0; t += 0.001) {
        double position = 0.0;
        double velocity = 0.0;
        trajectory.sample(t, position, velocity);
        largest = std::max(largest, std::abs(velocity));
      }
      EXPECT_GT(largest, 0.0);
      EXPECT_LE(largest, bound + 1e-12) << "start " << start;
    }
  }
}

TEST(WaveTrajectoryTest, StartBlendBeginsAtRestOnStartPosition)
{
  WaveTrajectory trajectory;
  trajectory.configure({"piston", 1.0, 0.0, 0.25, false}, 0.04, 2.0, 0.30);
  double position = 0.0;
  double velocity = 1.0;
  trajectory.sample(0.0, position, velocity);
  EXPECT_NEAR(position, 0.30, 1e-12);
  EXPECT_NEAR(velocity, 0.0, 1e-12);
}

TEST(WaveTrajectoryTest, TransferGainDividesStroke)
{
  for (const char * type : {"piston", "flap"}) {
    const double nominal = paddle_amplitude({type, 0.73, 1.3, 0.0, false, 1.0}, 0.02, 2.0);
    const double corrected = paddle_amplitude({type, 0.73, 1.3, 0.0, false, 2.5}, 0.02, 2.0);
    EXPECT_GT(nominal, 0.0);
    EXPECT_NEAR(corrected, nominal / 2.5, 1e-12) << type;
  }
}

TEST(WaveTrajectoryTest, TransferGainScalesOneSidedRange)
{
  WaveTrajectory nominal;
  nominal.configure({"piston", 1.0, 0.0, 0.25, true, 1.0}, 0.02, 2.0, 0.25);
  WaveTrajectory corrected;
  corrected.configure({"piston", 1.0, 0.0, 0.25, true, 2.0}, 0.02, 2.0, 0.25);
  EXPECT_NEAR(corrected.required_minimum(), 0.25, 1e-12);
  EXPECT_NEAR(
    corrected.required_maximum() - 0.25, (nominal.required_maximum() - 0.25) / 2.0, 1e-12);
}

TEST(WaveTrajectoryTest, FlapAmplitudeFollowsLinearTheoryAtCurrentDepth)
{
  // The flap stroke is the one at the waterline, scaled up to the attachment height.
  for (const double depth : {0.5, 0.72, 0.73, 1.0}) {
    const double omega = 2.0 * M_PI / 2.0;
    const double mu = wavemaker_controller::solve_dispersion(omega, depth);
    const double expected =
      wavemaker_controller::compute_stroke(mu, 0.02, "flap") / 2.0 * 1.3 / depth;
    EXPECT_NEAR(
      paddle_amplitude({"flap", depth, 1.3, 0.0, false, 1.0}, 0.02, 2.0), expected, 1e-12)
      << "depth " << depth;
  }
}

TEST(WaveTrajectoryTest, ShallowerWaterNeedsLargerStroke)
{
  // Long waves (10 s) are near the shallow-water limit, where the height-to-stroke ratio
  // grows with depth.
  for (const char * type : {"piston", "flap"}) {
    double previous = std::numeric_limits<double>::infinity();
    for (const double depth : {0.5, 0.72, 0.73, 1.0}) {
      const double stroke_at_waterline =
        paddle_amplitude({type, depth, depth, 0.0, false, 1.0}, 0.02, 10.0);
      EXPECT_LT(stroke_at_waterline, previous) << type << " depth " << depth;
      previous = stroke_at_waterline;
    }
  }
}

TEST(WaveTrajectoryTest, PeakVelocityIsTheTruePeak)
{
  // Largest lab wave: 1/15 at 1.3 s in 1 m of water (H = 17.1 cm), flap top at 1.3 m.
  // Starting anywhere in the stroke, the blend never moves faster than the wave itself.
  for (const bool upright_is_minimum : {false, true}) {
    WaveTrajectory reference;
    reference.configure({"flap", 1.0, 1.3, 0.0, upright_is_minimum, 1.0}, 0.171, 1.3, 0.0);
    const double low = reference.required_minimum();
    const double high = reference.required_maximum();
    for (const double start : {low, 0.5 * (low + high), high}) {
      WaveTrajectory trajectory;
      trajectory.configure(
        {"flap", 1.0, 1.3, 0.0, upright_is_minimum, 1.0}, 0.171, 1.3, start);

      double largest = 0.0;
      for (double t = 0.0; t < 4.0; t += 1e-4) {
        double position = 0.0;
        double velocity = 0.0;
        trajectory.sample(t, position, velocity);
        largest = std::max(largest, std::abs(velocity));
      }
      EXPECT_GE(trajectory.peak_velocity(), largest) << "start " << start;
      EXPECT_LE(trajectory.peak_velocity(), largest * 1.001) << "start " << start;
    }
  }
}

TEST(WaveTrajectoryTest, FlapHingedAtFloorUsesBottomHingedFormula)
{
  // hinge_height 0 must give exactly the bottom-hinged flap formula used before it existed.
  for (const double mu : {0.3, 1.0, 2.5}) {
    const double bottom_hinged = 4.0 * std::sinh(mu) *
      (mu * std::sinh(mu) - std::cosh(mu) + 1.0) / (mu * (std::sinh(2.0 * mu) + 2.0 * mu));
    EXPECT_NEAR(
      wavemaker_controller::compute_stroke(mu, 0.1, "flap", 0.0), 0.1 / bottom_hinged, 1e-12)
      << "mu " << mu;
  }
}

TEST(WaveTrajectoryTest, RaisedHingeMatchesDeanAndDalrymple)
{
  // mc_lab: 1.5 m water, hinge 0.5 m and attachment 1.95 m above the floor; H 0.05 m, T 1.5 s.
  // Reference computed independently (Python) from H/S = 4 sinh(kh) / (sinh 2kh + 2kh) *
  // [sinh(kh) + (cosh(kl) - cosh(kh)) / (k (h - l))], scaled by (1.95 - 0.5) / (1.5 - 0.5).
  WavemakerGeometry geometry{"flap", 1.5, 1.95, 0.0, false, 1.0, 0.5};
  EXPECT_NEAR(paddle_amplitude(geometry, 0.05, 1.5), 0.03501458303518656, 1e-9);

  // The same flap hinged at the floor needs less stroke at the attachment point.
  geometry.hinge_height = 0.0;
  EXPECT_NEAR(paddle_amplitude(geometry, 0.05, 1.5), 0.025396605521757824, 1e-9);
}

TEST(WaveTrajectoryTest, HigherHingeNeedsLargerStroke)
{
  double previous = 0.0;
  for (const double hinge : {0.0, 0.2, 0.5, 0.8, 1.2}) {
    const double amplitude =
      paddle_amplitude({"flap", 1.5, 1.95, 0.0, false, 1.0, hinge}, 0.05, 1.5);
    EXPECT_GT(amplitude, previous) << "hinge " << hinge;
    previous = amplitude;
  }
}

TEST(WaveTrajectoryTest, PistonIgnoresHingeHeight)
{
  EXPECT_NEAR(
    paddle_amplitude({"piston", 1.0, 0.0, 0.0, false, 1.0, 0.5}, 0.05, 1.5),
    paddle_amplitude({"piston", 1.0, 0.0, 0.0, false, 1.0, 0.0}, 0.05, 1.5), 1e-15);
}
