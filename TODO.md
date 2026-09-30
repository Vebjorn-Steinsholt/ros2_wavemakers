# Physical Wavemaker Checklist

Do not enable motion until these are verified:

- [x] Emergency stop is accessible, tested, and known to remove actuator power.
- [x] Mechanical guards, couplings, linkage, flap/piston attachment, and fasteners are secure.
- [ x] Full travel is physically clear, including the wavemaker, actuator, tank walls, and personnel area.
- [ ] Mechanical end stops are present and cannot be exceeded by software commands.
- [x] The actuator is mounted rigidly with no backlash or unexpected free play.
- [ x] Actual wavemaker type matches configuration: piston or flap.
- [x] Flap attachment height and water depth match the YAML values.
- [ ] Actuator conversion is verified:
  - [ ] Angular drive conversion matches the real mechanism.
  - [ ] Linear drive feedback units are confirmed to be meters or correctly converted.
- [ ] Positive command direction produces the expected physical direction.
- [ ] Encoder/position feedback changes correctly and has the expected sign.
- [ ] Establish the physical upright reference, reset the encoder so it reads 0 degrees there, then verify and record the zero.
- [ ] Software limits correspond to real safe limits:
  - [ ] Linear-drive wavemaker travel: 0.0 to 0.5 m; for angular drives, verify the configured degree limits map to the physical safe travel.
  - [x] Angular drive limits: `-550°` minimum and `500°` maximum are configured in IndraWorks.
  - [ ] Linear drive limits: configured minimum and maximum meters.
  - [ ] Maximum velocity: 50 rpm where applicable.
- [x] The configured `wavemaker_upright_position_m` matches the physical upright/mean position: `0.0 m` for ladertanken.
- [ ] Drive has no active faults and reports live on PROFIBUS.
- [ ] MGate communication is stable with no read/write errors.
- [ ] IP address, Modbus port, unit ID, and PROFIBUS address are correct and unique.
- [ ] Cable shielding, grounding, power, and network connections are secure.
- [ ] Communication is tested without motion first.
- [ ] Lifecycle transitions are tested without a goal: configure, activate, deactivate, cleanup.
- [x] Activation caused no observed physical motion; drive velocity feedback oscillates at standstill and must be resolved before motion testing.
- [x] Before the first motor movement test, mechanically disconnect the flap from the motor/actuator and secure the disconnected flap so it cannot move unexpectedly.
- [ ] With the flap disconnected, perform the first motor movement test at very small amplitude and low frequency.
- [ ] Actual position and velocity feedback are confirmed during first motion.
- [ ] Goal cancellation and lifecycle deactivation are tested while moving.
- [ ] Emergency stop and network/driver disconnection behavior are tested.
- [ ] The actuator disables safely after deactivation or communication loss.
- [ ] Reconnect the flap only after motor-only tests pass, then recheck mechanical attachment, zero position, direction, travel limits, and emergency-stop behavior before moving the flap.
- [ ] A known-good parameter backup is kept, and tested zero position and limits are recorded.

## Actuator Conversion Calibration

Calibrate both the scale and zero offset before setting `actuator_lead_m_per_degree`.

For several safe actuator angles, record the actuator angle and the corresponding physical
flap position in the same coordinate system used by `wavemaker_minimum` and
`wavemaker_maximum`:

| Actuator angle (degrees) | Physical flap position (m) |
| ---: | ---: |
| theta_1 | x_1 |
| theta_2 | x_2 |
| ... | ... |

Fit the relationship:

```text
x = a + b * theta
```

- [x] Use at least 5 to 10 positions across the full safe travel.
- [x] Calculate `b` as the meters-per-degree scale value: `0.053 / 100 = 0.00053 m/deg`.
- [x] Record `a`, the physical position when the actuator reports zero degrees: `0.0 m` for ladertanken.
- [x] Treat actuator zero as the upright flap reference: `actuator_upright_angle_deg: 0.0`.
- [ ] Record the measured angle/position pairs used to fit the scale and offset.
- [ ] Measure while moving in both directions to detect backlash.
- [ ] Repeat each position to check repeatability.
- [ ] Check that the relationship is approximately linear.
- [ ] Validate the fitted relationship with positions not used for fitting.
- [ ] Confirm that positive actuator angle produces positive wavemaker motion.
- [ ] Confirm the resulting drive limits cannot move the flap beyond its physical stops.
- [ ] If the fit has significant residual error, use a geometry-based conversion or a calibrated lookup table/interpolation instead of one constant scale value.

## Goal and Motion Test Sequence

Do not begin physical motion testing until the safety and calibration sections above are complete.

- [x] Launch one selected wavemaker with automatic lifecycle management disabled:
  ```bash
  ros2 launch wavemaker_bringup wavemakers.launch.py \
    wavemaker:=ladertanken enable_lifecycle_manager:=false
  ```
- [x] Set the controller and action names for the selected wavemaker:
  ```bash
  NS=/wavemakers/ladertanken
  NODE=/wavemakers/ladertanken/controller
  ACTION=$NS/move_wavemaker
  ```
- [x] Inspect interfaces and loaded limits/calibration before motion:
  ```bash
  ros2 node info $NODE
  ros2 action info $ACTION
  ros2 param dump $NODE
  ```
- [x] For linear drives, confirm `min_position_m` and `max_position_m`; for angular drives, confirm `min_position_deg` and `max_position_deg`.
- [x] Confirm the controller is unconfigured; no action server is expected before configure.
- [x] Configure without activating, then verify the inactive state and action server:
  ```bash
  ros2 lifecycle get $NODE
  ros2 lifecycle set $NODE configure
  ros2 lifecycle get $NODE
  ros2 action info $ACTION
  ```
- [x] While inactive, verify that a goal is rejected without enabling the drive:
  ```bash
  ros2 action send_goal --feedback \
    $ACTION \
    wavemaker_interfaces/action/MoveWavemaker \
    "{amplitude: 0.001, period: 10.0}"
  ```
- [ ] After the safety and calibration checks are complete, activate without an action goal:
  ```bash
  ros2 lifecycle set $NODE activate
  ros2 lifecycle get $NODE
  ```
- [ ] Confirm activation produces no unexpected motion or command.
- [ ] Monitor feedback and velocity during testing:
  ```bash
  ros2 topic echo $NS/wavemaker_velocity
  ros2 topic hz $NS/wavemaker_velocity
  ```
- [ ] With the flap disconnected and secured, send a very small, low-frequency goal (`amplitude: 0.00005` is about 1.2 degrees of actuator amplitude for the current ladertanken settings):
  ```bash
  ros2 action send_goal --feedback \
    $ACTION \
    wavemaker_interfaces/action/MoveWavemaker \
    "{amplitude: 0.00005, period: 10.0}"
  ```
- [ ] With `wavemaker_mode_pregenerated` enabled, test an in-range timed position sequence and confirm it completes at the final sample:
  ```bash
  ros2 action send_goal --feedback \
    $ACTION \
    wavemaker_interfaces/action/MoveWavemaker \
    "{positions: [0.0, 0.00053, 0.0], sample_interval: 1.0}"
  ```
- [ ] Confirm feedback, direction, limits, second-goal rejection, cancellation, and stop behavior.
- [ ] Verify oversized goals are rejected against configured actuator limits (angular degree limits converted to wavemaker position, or linear-drive meter limits).
- [ ] While a goal is active, test lifecycle deactivation and confirm the goal is aborted:
  ```bash
  ros2 lifecycle set $NODE deactivate
  ros2 lifecycle get $NODE
  ```
- [ ] Test the emergency stop and network/driver disconnection; confirm the actuator disables safely.
- [ ] Reconnect the flap only after motor-only tests pass, then recheck attachment, zero, direction, limits, and emergency stop.
- [ ] Clean up and repeat configuration to verify resources restart correctly:
  ```bash
  ros2 lifecycle set $NODE cleanup
  ros2 lifecycle set $NODE configure
  ros2 lifecycle set $NODE activate
  ```
