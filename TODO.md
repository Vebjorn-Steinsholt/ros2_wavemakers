# Wavemaker TODO

Open software work for the wavemakers, by priority. Physical checks on the rig are in
[COMMISSIONING.md](COMMISSIONING.md); the checks before each lab session are in
[STARTUP.md](STARTUP.md).

Item numbers are shared with [COMMISSIONING.md](COMMISSIONING.md) (numbering restarted on
2026-10-06), so references such as "item 33" stay valid in both files; gaps in the numbering are
the items kept there.

- **(rig)** needs the real wavemaker; the unit tests use fakes for the drive and the controller.
- Paths are relative to the repository root.

## High

- [ ] **5. Write the outputs every cycle by default.** (`wavemaker_controller/src/indradrive_actuator.cpp`)
  Change the `write_only_dirty` default in `IndraDriveActuator` to `false` (it is `true`; only ladertanken sets `false`). The MGate fault value timeout (item 5 in [COMMISSIONING.md](COMMISSIONING.md)) relies on writes every cycle.

- [ ] **12. The bridge can get stuck in `stopping`.** (`wavemaker_controller/src/wavemaker_bridge.cpp`)
  `stop()` publishes `stopping` and waits for the goal's result. If the controller's action is unavailable, `stop()` returns early and nothing leaves `stopping`; if the result never arrives, the same happens, because the 2 s timeout is only armed for stop-service calls. Every later `start` is then refused.
  - Arm the timeout for every stop of our own goal. On timeout, or if the action server is unavailable during a stop, clear `goal_handle_`, `goal_requested_` and `cancel_on_accept_`, and publish `idle` with a warning ("stop not confirmed by the controller; check it"). Test with the fake controller killed mid-wave.

## Medium

- [ ] **16. Test and document the bridge.** (`wavemaker_controller/test/`, `README.md`)
  - A gtest with a fake `move_wavemaker` server: start and refused start, stop (topic and service), stop during start, lost heartbeat, controller rejection and abort, deactivation (`inactive`), and item 12. `test/fake_controller.py` shows the fake's behaviour.
  - Document the bridge interface in the README: topics, types, QoS, the start sequence (height, period, then `start`), heartbeat, `wavemaker_state` values.

- [x] **17. Decide the fault policy in bridge mode.** (`wavemaker_bringup/launch/`)
  After a controller fault the controller goes to unconfigured and breaks its bond, so the lifecycle manager also brings the bridge down; the web page shows `inactive` and someone must relaunch. Decide whether that is acceptable, and check on the rig that the 1 s bond timeout doesn't trigger falsely (no false trigger in the controller-only manager test, 2026-10-08; recheck with the bridge).
  Acceptable. 

- [x] **18. Decide whether a cancel should return to upright.**
  Cancels and the bridge's stop now stop and hold. The earlier plan to return to upright after every cancel conflicts with that. Confirm "hold" and drop the plan, or make it an option with tests. 
  Comment: It was decided that cancel should not return to upright. 

- [ ] **19. Linear drives (lilletanken, mclab).** (`wavemaker_controller/src/indradrive_actuator.cpp`)
  The drive's position tag (scaled as degrees) is used as metres unchanged, and velocity is converted with `/ 6` as if it were deg/s → rpm; the limits exist twice (`wavemaker_minimum/maximum` and `min_position_m/max_position_m`). Once the drive scaling is confirmed on the rig (item 19 in [COMMISSIONING.md](COMMISSIONING.md)), add a proper linear conversion and derive the drive limits from the node limits.

- [ ] **20. Data race on `IndraDrive::last_error_`.** (`drivers/mgate5101_driver/src/indradrive.cpp`)
  Written by `fail()` from the worker thread, read by `fault_reason()` / `last_error()` from executor threads. Guard it with a mutex and return a copy.

- [ ] **21. Unit-test the hold behaviour of `IndraDriveActuator::halt()`.**
  Check that `halt()` writes the last commanded target (clamped) with `hold_velocity_rpm` and leaves Drive Start set, e.g. by reading the MGate driver's staged outputs.

- [ ] **22. `cancel_all_goals` can report `SERVER_UNAVAILABLE` right after configure.** (`wavemaker_controller/src/wavemaker_node.cpp`)
  `action_server_is_ready()` is false until discovery completes; wait up to about 1 s before replying.

- [x] **35. Run `lab.launch.py` on the lab Pi as a systemd service.** (`rpi/`)
  Done 2026-10-16: template unit `wavemaker@.service` (`User=%i`, never started at boot), `run.sh`, `stop.sh` (ordered shutdown through the lifecycle manager before SIGINT), settings in `/etc/default/wavemaker`, sudoers rule; install and use in [rpi/README.md](rpi/README.md). The web server stops the service after 2 h without a wave. Rig-tested on ladertanken: start, `systemctl stop` mid-wave, soft stop mid-wave, stop with `stop.sh` failing (SIGINT alone stopped the drive), reboot. Not yet tested on the rig: the idle auto-stop (tested in `demo.launch.py`).

## Low

- [X] **23. A return to upright cannot be stopped by a cancel or the bridge's stop.** (`wavemaker_node.cpp`)
  Only a lifecycle transition or a fault stops it. Decide; if needed, let a cancel set `stop_execution_` while `returning_to_upright_` is true.
  Handled by future soft stop button functionality.

- [ ] **24. Return-to-upright replies with the wrong reason in two cases.** (`wavemaker_node.cpp`)
  An exception gives `ACTUATOR_FAULT` with an empty message (use the exception text); a fault that deactivates the node first gives `CONTROLLER_INACTIVE` (check `actuator_->faulted()` in the `Stopped` case).

- [ ] **25. The start blend always lasts one wave period.** (`wavemaker_controller/src/wave_trajectory.cpp`)
  Far from the first wave position with a short period, the start is too fast and the goal is rejected. Use enough whole periods to stay under `return_to_upright_max_velocity_mps`.

- [ ] **26. Goal type is chosen by a parameter, and the bridge only starts regular waves.**
  Choose per goal (non-empty `positions` → pregenerated) or name the configured mode in the rejection. Decide whether the web page needs pregenerated waves.


- [ ] **27. A regular wave from the terminal runs until it is cancelled.**
  In bridge mode the heartbeat covers a lost client; from the terminal nothing does. Consider an optional `duration` in `MoveWavemaker.action`.

- [x] **28. Action feedback is sent every control cycle.** (`wavemaker_node.cpp`)
  Measurements are on `wavemaker_setpoint` / `wavemaker_position`; publish feedback at about 10 Hz or lower.

This is fine

- [ ] **29. `IndraDrive::disable()` can block a lifecycle callback for up to 20 s.** (`drivers/mgate5101_driver/src/indradrive.cpp`)
  Accept, or use a shorter timeout for `disable()`.

- [ ] **30. `wavemaker_minimum` / `wavemaker_maximum` are declared only for linear drives at construction.** (`wavemaker_node.cpp`)
  Always declare both.

- [ ] **31. Bridge interface details.** (`wavemaker_bridge.cpp`)
  `height`, `period` and `stop` use depth 10 instead of `command_qos`; the `stop` topic and `stop` service share a name (consider renaming the service, e.g. `stop_wave`).

- [ ] **32. Clean-up.**
  Remove the unused `driver_address_` / `wavemaker_id_` members and the commented-out members at the end of `WavemakerNode`; set the license in `wavemaker_controller/package.xml`. (Formatting and CMake lint fixed 2026-10-07.)

- [ ] **36. Soft stop timeout logged once at startup.** (`wavemaker_node.cpp`, `soft_stop.py`; rig)
  On the Pi, about 1 s after `Managed nodes are active`, the controller logs `No Soft stop message received for over 200 ms; treating as stopped` once (2026-10-16), then works normally. Most likely load while all nodes start; harmless before the first wave, but it is logged as an error on every normal start. If it ever appears mid-session, `soft_stop_timeout_s` (0.2 s) is too tight for the Pi. Decide: raise the timeout, or skip the check until the soft stop has published steadily for a moment.
