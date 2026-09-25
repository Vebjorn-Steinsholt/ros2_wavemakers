# Physical Wavemaker Checklist

Do not enable motion until these are verified:

- [ ] Emergency stop is accessible, tested, and known to remove actuator power.
- [ ] Mechanical guards, couplings, linkage, flap/piston attachment, and fasteners are secure.
- [ ] Full travel is physically clear, including the wavemaker, actuator, tank walls, and personnel area.
- [ ] Mechanical end stops are present and cannot be exceeded by software commands.
- [ ] The actuator is mounted rigidly with no backlash or unexpected free play.
- [ ] Actual wavemaker type matches configuration: piston or flap.
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
- [ ] The centered position is physically correct, since the controller uses the midpoint of minimum and maximum travel.
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
