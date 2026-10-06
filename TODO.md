# Wavemaker TODO

All open work for the wavemakers, physical checks and software together, by priority.
Numbering restarted on 2026-10-06 when the physical checklist and the code TODO were merged.

- **(rig)** needs the real wavemaker; the unit tests use fakes for the drive and the controller.
- Paths are relative to the repository root.
- Do not enable motion until every **High** item marked *before motion* is done.
- How to run the tests is in [Rig test procedure](#rig-test-procedure) at the end.

## High

### Before any motion

- [ ] **1. Mechanical end stops are present and software cannot drive past them.** (rig)

- [ ] **2. Software limits match the real safe travel.** (rig)
  - Ladertanken: controller limits are −500° to 500° (`min_position_deg` / `max_position_deg`); the drive is set to −550° / 500° in IndraWorks. Verify the controller range against the physical safe travel, and that the resulting limits cannot move the flap past its stops.
  - Linear drives: confirm the configured minimum and maximum (0.0–0.5 m in `wavemakers.yaml`).
  - Confirm the 50 rpm speed limit (`max_velocity_rpm`).

- [ ] **3. Direction, feedback and zero are correct.** (rig)
  - Positive command gives the expected physical direction; position feedback changes with the expected sign.
  - Set the physical upright reference, reset the encoder to 0° there, then verify and record the zero.
  - Angular conversion (`actuator_lead_m_per_degree: 0.00053`) matches the real mechanism; see [Actuator conversion calibration](#actuator-conversion-calibration).

- [ ] **4. Communication is correct and stable.** (rig)
  - IP address, Modbus port, unit ID and PROFIBUS address are correct and unique.
  - Cable shielding, grounding, power and network connections are secure. The PC must be on the gateway's wired network (it was on Wi-Fi only on 2026-10-06 and could not reach 192.168.1.70).
  - The drive has no active faults and reports live on PROFIBUS; the MGate shows no read/write errors (`mgate_example` in monitor mode).

- [ ] **5. Make the gateway switch the drive off if the PC or the node dies.** (rig; MGate configuration)
  The MGate keeps sending the last outputs (Drive On, Drive Start, last target) if Modbus writes stop. Our driver only sets control word 0 while its process is alive.
  - On the MGate's output module, set a fault value timeout of a few poll intervals (e.g. 200–300 ms) with fault value 0 (MGate manual p. 28). Test by killing the controller while the drive is enabled; the drive must switch off.
  - Change the `write_only_dirty` default in `IndraDriveActuator` to `false` (it is `true`; only ladertanken sets `false`). The fault timeout relies on writes every cycle.

- [ ] **6. Choose the poll interval.** (rig)
  Run `ros2 run mgate5101_driver poll_rate_probe` (controller stopped) and set `poll_interval_ms` in `wavemakers.yaml` to the shortest usable interval; the control loop follows it. Keep the MGate fault timeout (item 5) at several times it.

- [ ] **7. Lifecycle without motion.** (rig)
  Configure, activate, deactivate, cleanup and configure again with no goal; activation must cause no motion.

### First motion

- [ ] **8. First motor test with the flap disconnected.** (rig)
  The flap is already disconnected and secured. Run a very small, slow wave (`amplitude: 0.00005`, `period: 10.0`, about ±1.2° on ladertanken) and confirm position and velocity feedback.
  - Measure the position oscillation while the drive holds still, e.g. with the IndraWorks oscilloscope, or from `wavemaker_position` at the end of a `return_to_upright` (the topic is only published during motion). It must stay well below the 2 mm return-to-upright tolerance (`return_to_upright_default_tolerance_m`).

- [ ] **9. Confirm the drive keeps following after a stop.** (rig)
  `IndraDriveActuator::halt()` holds the last commanded target instead of using Drive Halt, which made the drive ignore later setpoints. Every stop, cancel and return relies on this.
  - Run a wave, stop it, start a second wave and check the paddle moves; run `return_to_upright` after a wave; stop at speed and check the paddle stops without moving back.

- [ ] **10. Stopping and failures while moving.** (rig)
  Test while a wave runs: cancel / bridge stop, lifecycle deactivation (goal aborted, drive disabled), emergency stop, network and gateway disconnection. The actuator must disable safely after deactivation or communication loss.

- [ ] **11. Reconnect the flap.** (rig)
  Only after items 8–10 pass. Then recheck the attachment, zero, direction, travel limits and emergency stop before moving the flap.

### Software

- [ ] **12. The bridge can get stuck in `stopping`.** (`wavemaker_controller/src/wavemaker_bridge.cpp`)
  `stop()` publishes `stopping` and waits for the goal's result. If the controller's action is unavailable, `stop()` returns early and nothing leaves `stopping`; if the result never arrives, the same happens, because the 2 s timeout is only armed for stop-service calls. Every later `start` is then refused.
  - Arm the timeout for every stop of our own goal. On timeout, or if the action server is unavailable during a stop, clear `goal_handle_`, `goal_requested_` and `cancel_on_accept_`, and publish `idle` with a warning ("stop not confirmed by the controller; check it"). Test with the fake controller killed mid-wave.

## Medium

- [ ] **13. Tune the drive's speed cap.** (rig)
  Plot `wavemaker_setpoint` against `wavemaker_position`. If the paddle lags at its fastest point, raise `positioning_velocity_margin` (default 1.2); if it moves in steps, lower it or the drive's acceleration (`S-0-0260`).

- [ ] **14. Complete and record the conversion calibration.** (rig)
  Record the angle/position pairs, measure in both directions (backlash), repeat positions, check linearity, validate with positions not used for the fit. See [Actuator conversion calibration](#actuator-conversion-calibration).

- [ ] **15. Keep a known-good parameter backup** of the drive and gateway, with the tested zero position and limits recorded. (rig)

- [ ] **16. Test and document the bridge.** (`wavemaker_controller/test/`, `README.md`)
  - A gtest with a fake `move_wavemaker` server: start and refused start, stop (topic and service), stop during start, lost heartbeat, controller rejection and abort, deactivation (`inactive`), and item 12. `test/fake_controller.py` shows the fake's behaviour.
  - Document the LabVIEW interface in the README: topics, types, QoS, the start sequence (amplitude, period, then `start`), heartbeat, `wavemaker_state` values, DDS names (`rt/...`, `std_msgs::msg::dds_::Bool_` etc.).

- [ ] **17. Decide the fault policy in bridge mode.** (`wavemaker_bringup/launch/`)
  After a controller fault the controller goes to unconfigured and breaks its bond, so the lifecycle manager also brings the bridge down; LabVIEW sees `inactive` and someone must relaunch. Decide whether that is acceptable, and check on the rig that the 1 s bond timeout doesn't trigger falsely.

- [ ] **18. Decide whether a cancel should return to upright.**
  Cancels and the bridge's stop now stop and hold. The earlier plan to return to upright after every cancel conflicts with that. Confirm "hold" and drop the plan, or make it an option with tests.

- [ ] **19. Linear drives (lilletanken, mc_lab).** (rig; `wavemaker_controller/src/indradrive_actuator.cpp`)
  The drive's position tag (scaled as degrees) is used as metres unchanged, and velocity is converted with `/ 6` as if it were deg/s → rpm; the limits exist twice (`wavemaker_minimum/maximum` and `min_position_m/max_position_m`). Confirm the drive scaling, add a proper linear conversion, derive the drive limits from the node limits.

- [ ] **20. Data race on `IndraDrive::last_error_`.** (`drivers/mgate5101_driver/src/indradrive.cpp`)
  Written by `fail()` from the worker thread, read by `fault_reason()` / `last_error()` from executor threads. Guard it with a mutex and return a copy.

- [ ] **21. Unit-test the hold behaviour of `IndraDriveActuator::halt()`.**
  Check that `halt()` writes the last commanded target (clamped) with `hold_velocity_rpm` and leaves Drive Start set, e.g. by reading the MGate driver's staged outputs.

- [ ] **22. `cancel_all_goals` can report `SERVER_UNAVAILABLE` right after configure.** (`wavemaker_controller/src/wavemaker_node.cpp`)
  `action_server_is_ready()` is false until discovery completes; wait up to about 1 s before replying.

## Low

- [ ] **23. A return to upright cannot be stopped by a cancel or the bridge's stop.** (`wavemaker_node.cpp`)
  Only a lifecycle transition or a fault stops it. Decide; if needed, let a cancel set `stop_execution_` while `returning_to_upright_` is true.

- [ ] **24. Return-to-upright replies with the wrong reason in two cases.** (`wavemaker_node.cpp`)
  An exception gives `ACTUATOR_FAULT` with an empty message (use the exception text); a fault that deactivates the node first gives `CONTROLLER_INACTIVE` (check `actuator_->faulted()` in the `Stopped` case).

- [ ] **25. The start blend always lasts one wave period.** (`wavemaker_controller/src/wave_trajectory.cpp`)
  Far from the first wave position with a short period, the start is too fast and the goal is rejected. Use enough whole periods to stay under `return_to_upright_max_velocity_mps`.

- [ ] **26. Goal type is chosen by a parameter, and the bridge only starts regular waves.**
  Choose per goal (non-empty `positions` → pregenerated) or name the configured mode in the rejection. Decide whether LabVIEW needs pregenerated waves.

- [ ] **27. A regular wave from the terminal runs until it is cancelled.**
  In bridge mode the heartbeat covers a lost client; from the terminal nothing does. Consider an optional `duration` in `MoveWavemaker.action`.

- [ ] **28. Action feedback is sent every control cycle.** (`wavemaker_node.cpp`)
  Measurements are on `wavemaker_setpoint` / `wavemaker_position`; publish feedback at about 10 Hz or lower.

- [ ] **29. `IndraDrive::disable()` can block a lifecycle callback for up to 20 s.** (`drivers/mgate5101_driver/src/indradrive.cpp`)
  Accept, or use a shorter timeout for `disable()`.

- [ ] **30. `wavemaker_minimum` / `wavemaker_maximum` are declared only for linear drives at construction.** (`wavemaker_node.cpp`)
  Always declare both.

- [ ] **31. Bridge interface details.** (`wavemaker_bridge.cpp`)
  `amplitude`, `period` and `stop` use depth 10 instead of `command_qos`; the `stop` topic and `stop` service share a name (consider renaming the service, e.g. `stop_wave`).

- [ ] **32. Clean-up.**
  Remove the unused `driver_address_` / `wavemaker_id_` members and the commented-out members at the end of `WavemakerNode`; run `ament_uncrustify --reformat` on `wavemaker_bridge.cpp`, `indradrive_actuator.cpp/.hpp`, `wavemaker_actuator.hpp` and both test files; fix the whitespace on lines 14 and 24 of `wavemaker_controller/CMakeLists.txt`; set the license in `wavemaker_controller/package.xml`.

## Verified so far

Kept as a record of physical checks already done.

- Emergency stop is accessible, tested, and removes actuator power.
- Mechanical guards, couplings, linkage, attachment and fasteners are secure.
- Full travel is physically clear (wavemaker, actuator, tank walls, personnel area).
- The actuator is mounted rigidly, without backlash or free play.
- Wavemaker type, flap attachment height and water depth match `wavemakers.yaml`.
- Drive limits −550° / 500° are configured in IndraWorks.
- `wavemaker_upright_position_m` matches the physical upright: 0.0 m for ladertanken.
- Activation caused no physical motion.
- Small position oscillation at standstill (the servo holding position) is accepted; it shows as larger velocity oscillation, but the controller only uses position. Its size is checked in item 8.
- The flap is mechanically disconnected and secured for the first motor tests.
- Calibration: 5–10 positions used; scale `b = 0.053 / 100 = 0.00053 m/deg`; offset `a = 0.0 m`; actuator zero = upright (`actuator_upright_angle_deg: 0.0`).
- Terminal-mode launch, interface inspection, configure without activate, and goal rejection while inactive.

## Actuator conversion calibration

Calibrate both the scale and the zero offset before setting `actuator_lead_m_per_degree`. For several safe
actuator angles, record the angle and the physical flap position in the same coordinate system as the
wavemaker limits, and fit `x = a + b * theta`:

| Actuator angle (degrees) | Physical flap position (m) |
| ---: | ---: |
| theta_1 | x_1 |
| theta_2 | x_2 |
| ... | ... |

If the fit has significant residual error, use a geometry-based conversion or a calibrated lookup table
instead of one constant scale.

## Rig test procedure

Terminal mode, ladertanken; replace the name for another wavemaker. Do not start motion until the
*before motion* items are done.

```bash
cd ~/ros_ws && source install/setup.bash
NS=/wavemakers/ladertanken

# communication only, no motion (controller stopped)
ros2 run mgate5101_driver poll_rate_probe --seconds 10

# launch (controller only, lifecycle by hand)
ros2 launch wavemaker_bringup wavemakers.launch.py wavemaker:=ladertanken
ros2 lifecycle set $NS/controller configure
ros2 lifecycle set $NS/controller activate          # enables the drive

# watch
ros2 run rqt_plot rqt_plot $NS/wavemaker_setpoint/data $NS/wavemaker_position/data

# small slow wave, then stop
ros2 action send_goal --feedback $NS/move_wavemaker wavemaker_interfaces/action/MoveWavemaker \
  "{amplitude: 0.00005, period: 10.0}"            # Ctrl-C cancels
ros2 service call $NS/return_to_upright wavemaker_interfaces/srv/ReturnToUpright "{requester: 'cli', tolerance: 0.0}"

# finish
ros2 lifecycle set $NS/controller deactivate        # disables the drive
```

Bridge (LabVIEW role played from the terminal), with the controller active:

```bash
ros2 launch wavemaker_bringup wavemakers.launch.py wavemaker:=ladertanken with_bridge:=true
# configure + activate the controller, then the bridge ($NS/wavemaker_bridge)
ros2 topic pub -r 5 $NS/heartbeat std_msgs/msg/Bool "{data: true}"
ros2 topic echo $NS/wavemaker_message
ros2 topic pub --once -w 1 $NS/amplitude std_msgs/msg/Float64 "{data: 0.00005}"
ros2 topic pub --once -w 1 $NS/period std_msgs/msg/Float64 "{data: 10.0}"
ros2 topic pub --once -w 1 $NS/start std_msgs/msg/Bool "{data: true}"
ros2 topic pub --once -w 1 $NS/stop std_msgs/msg/Bool "{data: true}"
```

Without hardware, run `python3 wavemaker_controller/test/fake_controller.py --ros-args -r __ns:=$NS`
instead of the controller (options `--reject`, `--abort-after SECONDS`).
