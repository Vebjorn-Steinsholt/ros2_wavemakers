# Physical Wavemaker Checklist

Do not enable motion until these are verified:

- [x] Emergency stop is accessible, tested, and known to remove actuator power.
- [x] Mechanical guards, couplings, linkage, flap/piston attachment, and fasteners are secure.
- [ x] Full travel is physically clear, including the wavemaker, actuator, tank walls, and personnel area.
- [ ] Mechanical end stops are present and cannot be exceeded by software commands.
- [x] The actuator is mounted rigidly with no backlash or unexpected free play.
- [ x] Actual wavemaker type matches configuration: piston or flap.
- [ ] Flap attachment height and water depth match the YAML values.
- [ ] Actuator conversion is verified:
  - [ ] Angular drive conversion matches the real mechanism.
  - [ ] Linear drive feedback units are confirmed to be meters or correctly converted.
- [ ] Positive command direction produces the expected physical direction.
- [ ] Encoder/position feedback changes correctly and has the expected sign.
- [ ] Physical zero/reference position is established and recorded.
- [ ] Software limits correspond to real safe limits:
  - [ ] Wavemaker travel: 0.0 to 0.5 m.
  - [ ] Drive limits: configured minimum and maximum degrees.
  - [ ] Maximum velocity: 50 rpm where applicable.
- [ ] The configured `wavemaker_upright_position_m` matches the physical upright/mean position.
- [ ] Drive has no active faults and reports live on PROFIBUS.
- [ ] MGate communication is stable with no read/write errors.
- [ ] IP address, Modbus port, unit ID, and PROFIBUS address are correct and unique.
- [ ] Cable shielding, grounding, power, and network connections are secure.
- [ ] Communication is tested without motion first.
- [ ] Lifecycle transitions are tested without a goal: configure, activate, deactivate, cleanup.
- [ ] Activation does not cause unexpected motion.
- [ ] First motion is performed with very small amplitude and low frequency.
- [ ] Actual position and velocity feedback are confirmed during first motion.
- [ ] Goal cancellation and lifecycle deactivation are tested while moving.
- [ ] Emergency stop and network/driver disconnection behavior are tested.
- [ ] The actuator disables safely after deactivation or communication loss.
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

- [ ] Use at least 5 to 10 positions across the full safe travel.
- [ ] Calculate `b` as the meters-per-degree scale value.
- [ ] Record `a`, the physical position when the actuator reports zero degrees.
- [ ] Treat actuator zero as the upright flap reference, record its corresponding wavemaker position, and plan a separate angular zero-offset parameter.
- [ ] Measure while moving in both directions to detect backlash.
- [ ] Repeat each position to check repeatability.
- [ ] Check that the relationship is approximately linear.
- [ ] Validate the fitted relationship with positions not used for fitting.
- [ ] Confirm that positive actuator angle produces positive wavemaker motion.
- [ ] Confirm the resulting drive limits cannot move the flap beyond its physical stops.
- [ ] If the fit has significant residual error, use a geometry-based conversion or a calibrated lookup table/interpolation instead of one constant scale value.

## Goal and Motion Test Sequence

Do not begin physical motion testing until the safety and calibration sections above are complete.

- [ ] Confirm the controller lifecycle state before testing:
  ```bash
  ros2 lifecycle get /wavemakers/ladertanken/controller
  ```
- [ ] Verify that a goal is rejected while the controller is inactive.
- [ ] Configure and activate the controller without an action goal.
- [ ] Confirm activation produces no unexpected motion or command.
- [ ] Monitor the velocity topic during testing:
  ```bash
  ros2 topic echo /wavemakers/ladertanken/controller/wavemaker_velocity
  ```
- [ ] Send a very small, low-frequency goal with the action client:
  ```bash
  ros2 action send_goal --feedback \
    /wavemakers/ladertanken/controller/move_wavemaker \
    wavemaker_interfaces/action/MoveWavemaker \
    "{amplitude: 0.001, period: 10.0}"
  ```
- [ ] Confirm desired position oscillates around `wavemaker_upright_position_m`.
- [ ] Confirm actual position and velocity feedback are sensible.
- [ ] Confirm positive actuator angle produces the expected physical direction.
- [ ] Send a second goal while the first is active and verify that it is rejected.
- [ ] Send a goal whose amplitude exceeds either travel inequality and verify rejection:
  - [ ] `x_upright - A >= x_minimum`.
  - [ ] `x_upright + A <= x_maximum`.
- [ ] Test goal cancellation while the actuator is moving.
- [ ] Test lifecycle deactivation while a goal is active and confirm the goal is aborted.
- [ ] Confirm deactivation stops the execution thread and disables the actuator.
- [ ] Test emergency stop behavior while moving with an operator at the emergency stop.
- [ ] Test network or driver disconnection and confirm the actuator disables safely.
- [ ] Repeat the test after cleanup and reconfiguration to verify resources restart correctly.
