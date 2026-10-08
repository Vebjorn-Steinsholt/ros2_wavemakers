# Wavemaker TODO

All open work for the wavemakers, physical checks and software together, by priority.
Numbering restarted on 2026-10-06 when the physical checklist and the code TODO were merged.

- **(rig)** needs the real wavemaker; the unit tests use fakes for the drive and the controller.
- Paths are relative to the repository root.
- Do not enable motion until every **High** item marked *before motion* is done.
- How to run the tests is in [Rig test procedure](#rig-test-procedure) at the end.

## High

### Before any motion

- [ ] **1. Narrow the ladertanken limits once the transfer gain is known.** (rig)
  The limits are ±200° (`min_position_deg` / `max_position_deg`, ±10.6 cm at the flap top) and 170 rpm (`max_velocity_rpm`), set 2026-10-08 for the lab waves and verified with them (see *Verified so far*). There are no mechanical end stops, so these are the only travel limits.
  - Once the transfer gain is measured (item 33), recompute the required angles and speeds (theory / gain) and narrow the limits to the largest lab wave plus 10–20 %; keep `max_velocity_rpm` at 1.2 × the largest wave's peak speed.
  - If the old system's settings or logs exist, compare its largest commanded stroke and speed with these numbers.
  - Optional: confirm the drive's degrees are motor degrees by counting the motor pulley's teeth and measuring the belt pitch (teeth × pitch ≈ 190 mm), and check the drive's velocity limit (S-0-0091) is at least 170 rpm.
  - (Mechanical, for whoever owns the mechanics.) The belt can jump a tooth under hard turns, and the motor encoder does not see it: the flap would then be offset from the zero and the travel limits without any warning. Check belt tension and condition regularly; after long lab sessions run `return_to_upright` and check the flap is visually upright.

- [ ] **2. Linear drive limits.** (rig)
  Confirm the configured minimum and maximum (0.0–0.5 m in `wavemakers.yaml`) for lilletanken and mc_lab.

- [ ] **3. Direction, feedback and zero are correct.** (rig)
  - Positive command gives the expected physical direction. The drive moves positive in degrees first, as commanded (2026-10-07); which way the paddle moves has not been seen yet. Check it with someone at the paddle or a phone camera, at the latest when the flap is reconnected (item 11).
  - Done 2026-10-07: position feedback follows the setpoint with the same sign; `return_to_upright` settles at 0° (±0.002° in the drive software), the upright reference.
  - Angular conversion (`actuator_lead_m_per_degree: 0.00053`) matches the real mechanism; see [Actuator conversion calibration](#actuator-conversion-calibration).

- [ ] **4. Communication is correct and stable.** (rig)
  - IP address, Modbus port, unit ID and PROFIBUS address are correct and unique.
  - Cable shielding, grounding, power and network connections are secure.
  - The drive has no active faults and reports live on PROFIBUS (`mgate_example` in monitor mode).

- [ ] **5. Make the gateway switch the drive off if the PC or the node dies.** (rig; MGate configuration)
  The MGate keeps sending the last outputs (Drive On, Drive Start, last target) if Modbus writes stop. Our driver only sets control word 0 while its process is alive.
  - On the MGate's output module, set a fault value timeout of about 500 ms (it must ride out the ~210 ms gateway stalls; see *Verified so far*) with fault value 0 (MGate manual p. 28). Test by killing the controller while the drive is enabled; the drive must switch off.
  - Change the `write_only_dirty` default in `IndraDriveActuator` to `false` (it is `true`; only ladertanken sets `false`). The fault timeout relies on writes every cycle.

- [ ] **34. Spring balance matches the water depth.** (rig; mechanical, for whoever owns the mechanics)
  The blue ropes run over a pulley to large springs whose tension counteracts the water pressure on the flap, so the motor is stable at upright. The water's moment on the flap grows roughly with depth cubed (1.0 m ≈ 2.5 × 0.73 m), so the tension must be reset whenever the water level changes, together with `water_depth` in `wavemakers.yaml`.
  - Check: with the drive holding upright, motor torque or current in IndraWorks is close to zero. A steady holding torque means the balance is off; it heats the motor and takes from the torque the waves need.
  - The ropes are frayed (fuzzy surface along their length) and carry the preload all the time. If one breaks, the full water moment lands on the motor (overcurrent or following-error fault) or swings the flap toward the dry side. Inspect them now and replace if worn; check them again before the larger lab waves.
  - Release the tension before draining the tank, or the springs pull the flap the other way.

- [ ] **7. Lifecycle without motion.** (rig)
  Configure, activate, deactivate, cleanup and configure again with no goal; activation must cause no motion.

### First motion

- [ ] **9. Confirm the drive keeps following after a stop.** (rig)
  `IndraDriveActuator::halt()` holds the last commanded target instead of using Drive Halt, which made the drive ignore later setpoints. Every stop, cancel and return relies on this.
  - Done 2026-10-07: after a Ctrl-C cancel, a new wave starts and the paddle moves, and `return_to_upright` brings it back to 0° (drive reading −0.0020° to +0.0017° at rest).
  - Done 2026-10-08, flap connected: the `cancel_all_goals` service stops a running wave, and new waves start afterwards.
  - Still to check: stop at speed (cancel as the paddle passes the middle) and check it stops without moving back.

- [ ] **10. Stopping and failures while moving.** (rig)
  Test while a wave runs: bridge stop, emergency stop, network and gateway disconnection. The actuator must disable safely after deactivation or communication loss. Cancel by Ctrl-C (2026-10-07), by the `cancel_all_goals` service and lifecycle deactivation through the lifecycle manager (2026-10-08) are done.

- [ ] **11. Rechecks with the flap reconnected.** (rig)
  The flap was reconnected and run in water on 2026-10-08 (lab waves, see *Verified so far*), before items 9 and 10 were finished. Still to recheck with the flap connected: which way the flap moves for a positive command (item 3), and the emergency stop while a wave runs (item 10).

### Software

- [ ] **12. The bridge can get stuck in `stopping`.** (`wavemaker_controller/src/wavemaker_bridge.cpp`)
  `stop()` publishes `stopping` and waits for the goal's result. If the controller's action is unavailable, `stop()` returns early and nothing leaves `stopping`; if the result never arrives, the same happens, because the 2 s timeout is only armed for stop-service calls. Every later `start` is then refused.
  - Arm the timeout for every stop of our own goal. On timeout, or if the action server is unavailable during a stop, clear `goal_handle_`, `goal_requested_` and `cancel_on_accept_`, and publish `idle` with a warning ("stop not confirmed by the controller; check it"). Test with the fake controller killed mid-wave.

## Medium

- [ ] **13. Tune the drive's speed cap.** (rig)
  Plot `wavemaker_setpoint` against `wavemaker_position`. If the paddle lags at its fastest point, raise `positioning_velocity_margin` (default 1.2); if it moves in steps, lower it or the drive's acceleration (`S-0-0260`).
  - Also look for setpoint freezes of about 0.2 s (gateway stalls; see *Verified so far*). If they occur, check `nstat -az TcpRetransSegs` before and after (it prints the total since boot) and the cable and switch; if they persist, raise `poll_interval_ms`.
  - Activation measures one gateway cycle and fails if it is slow; retry once before suspecting anything else.

- [ ] **14. Complete and record the conversion calibration.** (rig)
  Record the angle/position pairs, measure in both directions (backlash), repeat positions, check linearity, validate with positions not used for the fit. See [Actuator conversion calibration](#actuator-conversion-calibration).

- [ ] **15. Keep a known-good parameter backup** of the drive and gateway, with the tested zero position and limits recorded. (rig)

- [ ] **16. Test and document the bridge.** (`wavemaker_controller/test/`, `README.md`)
  - A gtest with a fake `move_wavemaker` server: start and refused start, stop (topic and service), stop during start, lost heartbeat, controller rejection and abort, deactivation (`inactive`), and item 12. `test/fake_controller.py` shows the fake's behaviour.
  - Document the LabVIEW interface in the README: topics, types, QoS, the start sequence (height, period, then `start`), heartbeat, `wavemaker_state` values, DDS names (`rt/...`, `std_msgs::msg::dds_::Bool_` etc.).

- [ ] **17. Decide the fault policy in bridge mode.** (`wavemaker_bringup/launch/`)
  After a controller fault the controller goes to unconfigured and breaks its bond, so the lifecycle manager also brings the bridge down; LabVIEW sees `inactive` and someone must relaunch. Decide whether that is acceptable, and check on the rig that the 1 s bond timeout doesn't trigger falsely (no false trigger in the controller-only manager test, 2026-10-08; recheck with the bridge).

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

- [ ] **33. Calibrate the wave-height transfer gain.** (rig; wave probes, flap connected)
  `wavemaker_transfer_gain` (default 1.0 = linear theory, in `wavemakers.yaml`) divides the stroke of regular waves to correct the generated height. Run regular waves over the periods and heights in use, measure the height with wave probes away from the paddle, and set new gain = old gain × measured / target.
  - If the ratio varies with period, one gain is not enough; record the ratios and decide whether the gain should depend on period.
  - Recheck after changing the water depth or the calibration from item 14.
  - Not applied to pregenerated waves; decide whether it should be.

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
  `height`, `period` and `stop` use depth 10 instead of `command_qos`; the `stop` topic and `stop` service share a name (consider renaming the service, e.g. `stop_wave`).

- [ ] **32. Clean-up.**
  Remove the unused `driver_address_` / `wavemaker_id_` members and the commented-out members at the end of `WavemakerNode`; set the license in `wavemaker_controller/package.xml`. (Formatting and CMake lint fixed 2026-10-07.)

## Verified so far

Kept as a record of physical checks already done.

- Emergency stop is accessible, tested, and removes actuator power.
- Mechanical guards, couplings, linkage, attachment and fasteners are secure.
- Full travel is physically clear (wavemaker, actuator, tank walls, personnel area).
- The actuator is mounted rigidly, without backlash or free play.
- Wavemaker type, flap attachment height and water depth match `wavemakers.yaml`.
- Drive limits −550° / 500° are configured in IndraWorks and are kept there by decision (2026-10-07); the controller's ±200° is the travel limit.
- No mechanical end stops. The lab waves above have been run on this hardware at 1 m depth (Pål Lader, 2026-10-08), which by linear theory needs about ±9.3 cm at the flap top; the earlier ±5 cm figure (2026-10-07) was too low.
- `wavemaker_upright_position_m` matches the physical upright: 0.0 m for ladertanken.
- Activation caused no physical motion.
- Small position oscillation at standstill (the servo holding position) is accepted; it shows as larger velocity oscillation, but the controller only uses position. Measured below ±0.5° (≈ ±0.27 mm on ladertanken), well inside the 2 mm return-to-upright tolerance (≈ 3.8°).
- The flap was disconnected for the first motor tests (2026-10-07) and reconnected for the lab waves (2026-10-08).
- Calibration: 5–10 positions used, measured at the flap top, which is at `flap_attachment_height`; scale `b = 0.053 / 100 = 0.00053 m/deg`; offset `a = 0.0 m`; actuator zero = upright (`actuator_upright_angle_deg: 0.0`).
- The PC reaches the gateway at 192.168.1.70 over the wired network.
- Motor nameplate (ladertanken): Rexroth MSK071E-0450-NN-M1-UG0-NNNN, 3-phase permanent magnet; n max 6000 min⁻¹ (4500 rated), MdN 23.0 Nm / IdN 20.0 A natural convection (34.5 Nm / 30.0 A surface cooled), Km 1.29 Nm/A, KE 82.7 V/1000 min⁻¹, 23.5 kg.
- Poll interval (2026-10-07): `poll_interval_ms` is 20 ms (50 before). Normal gateway cycles take 4–8 ms with no read/write errors or reconnects. The 10 s sweep had single cycles of about 210 ms at 40, 10, 5 and 2 ms; at 2 ms nearly every cycle stalled, so the stalls grow with polling rate (the gateway, or packets lost under load). A 60 s run at 20 ms had none (3000 cycles, max 8.1 ms). Stalls during waves are watched in item 13.
- First motion, flap disconnected (2026-10-07): wave `amplitude: 0.0005` (the action's old field, H/2; now `height: 0.001`), `period: 10.0` (about ±1 cm at the flap top, ±19.5°); `wavemaker_position` follows `wavemaker_setpoint` with the same sign and size.
- Larger wave, flap disconnected (2026-10-07): `amplitude: 0.002` (now `height: 0.004`), `period: 10.0` runs as calculated (first movement positive, peaks about ±79°, ±4.1 cm at the flap top); Ctrl-C stops and holds.
- Terminal-mode launch, interface inspection, configure without activate, and goal rejection while inactive.
- Lab waves, flap connected, 1.01 m water depth, limits ±200° and 170 rpm (2026-10-08). All eight steps ran, each followed by `return_to_upright` with the flap upright afterwards. In order of rising acceleration, as `height` [m] / `period` [s] (run with the action's old `amplitude` field, H/2; lab steepness; expected drive angle and peak speed by linear theory): 0.026 / 1.0 (1/60; ±21°, 22 rpm), 0.049 / 1.2 (1/45; ±45°, 40 rpm), 0.074 / 1.2 (1/30; ±68°, 60 rpm), 0.108 / 1.5 (1/30; ±132°, 92 rpm), 0.051 / 0.7 (1/15; ±35°, 53 rpm), 0.104 / 1.0 (1/15; ±83°, 87 rpm), 0.148 / 1.2 (1/15; ±137°, 119 rpm), 0.171 / 1.3 (1/15, the largest lab wave; ±173°, 139 rpm). Wave heights were not measured (item 33).
- `cancel_all_goals` stops a running wave, flap connected (2026-10-08).
- Lifecycle manager, controller only (`enable_lifecycle_manager:=true bond_timeout:=1.0`), flap connected (2026-10-08): the manager configures and activates the controller by itself; a wave runs; PAUSE (`manage_nodes` command 1) during a wave aborts the goal, deactivates the controller and disables the drive; RESUME (2) reactivates it, `return_to_upright` and a new wave work; SHUTDOWN (4) finalizes the controller with the drive disabled. The 1 s bond timeout did not trigger falsely. Not tested: a crashed controller (needs the MGate fault timeout first, item 5).

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
  "{height: 0.0001, period: 10.0}"                # Ctrl-C cancels
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
ros2 topic pub --once -w 1 $NS/height std_msgs/msg/Float64 "{data: 0.0001}"
ros2 topic pub --once -w 1 $NS/period std_msgs/msg/Float64 "{data: 10.0}"
ros2 topic pub --once -w 1 $NS/start std_msgs/msg/Bool "{data: true}"
ros2 topic pub --once -w 1 $NS/stop std_msgs/msg/Bool "{data: true}"
```

Without hardware, run `python3 wavemaker_controller/test/fake_controller.py --ros-args -r __ns:=$NS`
instead of the controller (options `--reject`, `--abort-after SECONDS`).
