# Sloshing oscillator

Plan and record for running the LinMot sloshing oscillator from the lab Pi with the same
controller, bridge and web page as the wavemakers. Work happens on the branch `feat/oscillator`
and stays off master and the lab Pi until it has run on its rig.

- **(rig)** needs the real oscillator; the unit tests use a simulated Arduino and drive.
- Paths are relative to the repository root.
- Decisions are dated; change them here when they change.

## The rig

A LinMot linear motor moves a sloshing tank back and forth. The physical limits of the tank and
the water are fixed:

| | Limit |
| --- | --- |
| Travel | 5–195 mm, centre 100 mm (hard end stops at 0 and 200 mm) |
| Speed | 2 m/s |
| Acceleration | 9.81 m/s² (1 g, the professor's specification, tested) |
| Frequency | 5 Hz |

Goals give a height H, the peak-to-peak stroke, and a period T. There is no transfer function and
no water depth: the water level changes between experiments but does not affect the control.

## How it fits together

```
 web page / ros2 CLI ──▶ bridge_node ──▶ controller_node ──USB serial──▶ Arduino Uno ──CAN──▶ LinMot drive ──▶ slide
                                         (LinMotActuator)    115200 baud    + MCP2515     500 kbit/s
                                                             22/14-byte     shield        CANopen node 12
                                                             frames, CRC
```

| Layer | Where | Does |
| --- | --- | --- |
| Firmware | `firmware/linmot_serial_bridge/` | Serial frames to CAN and back; rejects commands outside 5–195 mm, 3 m/s, 20 m/s²; aborts the motion when no frame arrives for 200 ms (watchdog). `link_test.py` tests it without ROS. |
| Driver | `drivers/linmot_serial_driver/` | `SerialBridge` sends one frame every 20 ms (a command, the streamed target or a status request), so the watchdog only trips if the Pi stops. `LinMotDrive` brings the drive up from its status word, streams VAI go-to-position commands and latches faults. No ROS. |
| Actuator | `rig_controller/src/linmot_actuator.*` | Implements `Actuator` for the controller, selected with `actuator_type: linmot`. |
| Controller | `rig_controller/src/controller_node.cpp` | Lifecycle node, trajectories, limits, goals; shared with the wavemakers. |

Three layers of limits, the tightest first: the controller (5–195 mm, 2 m/s, 9.81 m/s², 5 Hz;
step D), the firmware (5–195 mm, 3 m/s, 20 m/s²) and the drive's own position limits in
LinMot-Talk (3 / 197 mm). The hardware e-stop is the last.

## Decisions

- **Link (2026-10-19):** USB serial from the Pi to the Arduino instead of the earlier Wi-Fi/UDP.
  An Ethernet shield clashed with the CAN shield on SPI and interrupts.
- **No drive-internal sine (2026-10-19):** the drive's built-in sine (old command 8) keeps
  running when the Pi stops sending, so it was removed. The Pi streams every position.
- **VAI limits (2026-10-21):** each go-to-position command carries the sine's peak velocity and
  peak acceleration. From experience, lower limits make the motion jitter.
- **Start-up (2026-10-21):** at activation the actuator reads the status word and acts on it:
  acknowledges an error, switches on, homes if needed (`home_if_needed`, default true). A fatal
  error, a silent Arduino or drive, or a drive that does not switch on fails the activation.
- **Board (2026-10-21):** an Arduino Uno R3 for now; an Uno R4 WiFi also runs the firmware.
- **Names (2026-10-21):** `wavemaker_controller` became `rig_controller`; the node, bridge and
  actuator interface became `controller_node`, `bridge_node` and `Actuator`. Actuator classes
  are named after the drive (`IndraDriveActuator`, `LinMotActuator`); the rig's behaviour
  comes from `wavemaker_type` (`oscillator`, step D).
- **Topics and actions name the rig (2026-10-21):** a `rig` parameter (`wavemaker` |
  `oscillator`, default `wavemaker`) on the controller, bridge and web page; for the oscillator
  `move_oscillator`, `oscillator_setpoint` and so on (step R). Proposed; scheme to be confirmed.
- **Pregenerated goals (2026-10-21):** allowed for the oscillator, very low priority.
- **Bringup (2026-10-21):** a separate package `oscillator_bringup` with its own
  `oscillator.launch.py`, namespace `/oscillators/sloshing`.
- **Pi (2026-10-19):** the oscillator and the wavemaker never run at the same time on the lab Pi;
  separate `oscillator@.service`, shared soft stop button on GPIO17.

## Plan

### Done

- [x] Firmware: framing, CRC, READY, watchdog (2026-10-19); switch-on after power-up and error
  acknowledge (2026-10-20); node 12 and INT on D2 (2026-10-21).
- [x] `link_test.py --move` passes on the rig: link, CAN path, homing, streamed sine, rejected
  command, watchdog (2026-10-21).
- [x] Drive limits in LinMot-Talk and the abort fix (see [Found on the rig](#found-on-the-rig)).
- [x] Step A, `linmot_serial_driver` with `linmot_probe` (2026-10-21; tested against the
  simulator only).
- [x] Renames to `rig_controller`, `controller_node`, `bridge_node`, `Actuator` (2026-10-21).
- [x] Step B code: `LinMotActuator`, `actuator_type` in the controller, tests (2026-10-21; not
  committed yet).

### Now: step B on the rig

- [ ] Commit step B.
- [ ] Firmware: upload the command-count change (motion command count instead of an empty header
  and an extra SYNC; PDO1 last), run `link_test.py --move`, expect about one feedback per
  command, then commit it. (rig)
- [ ] Run `linmot_probe` against the Uno and drive (no motion). (rig)
- [ ] First controller run with a temporary parameter file (`actuator_type: linmot`,
  `wavemaker_type: piston` as a placeholder), lifecycle by hand: configure, activate (homing),
  `return_to_upright`, faults (USB unplugged, e-stop, LinMot-Talk switching off), deactivate.
  No `height`/`period` goals before step D: they would be turned into a stroke by wave theory.
  (rig)

### C + D: the oscillator in the controller

- [ ] `wavemaker_type: "oscillator"`: amplitude H/2 around `wavemaker_upright_position_m`; no
  dispersion relation, transfer function or gain (`wave_trajectory.cpp`, `controller_node.cpp`).
- [ ] `on_configure`: accept `oscillator`; water depth, flap, hinge and gain checks only for
  `flap` / `piston`; `wavemaker_upright_is_minimum` must be false.
- [ ] `peak_acceleration()` for regular waves (sampled over the start blend, like
  `peak_velocity()`), pregenerated goals and the return to upright.
- [ ] Goal checks for every type, off by default: peak acceleration (9.81 m/s² for the
  oscillator) and shortest period (0.2 s).
- [ ] Step C: the setpoint carries peak acceleration × margin to the actuator; remove
  `linmot_default_acceleration_mps2`.
- [ ] Tests (`test_wave_trajectory.cpp`, `test_controller_node.cpp`) and the README parameter
  table.

### R: topics and actions name the rig

- [ ] `rig` parameter on `controller_node`, `bridge_node` and the web page; names built from it.
- [ ] The controller fails configure if `wavemaker_type: oscillator` and `rig` do not match.
- [ ] `--rig` for `fake_controller.py` and `play_dat.py`.
- [ ] Defaults keep every wavemaker name as it is.

### E: bringup

- [ ] Package `oscillator_bringup`: `config/oscillator.yaml` under
  `/oscillators/sloshing/controller` and `launch/oscillator.launch.py` (controller, bridge,
  lifecycle manager, soft stop, web page with `rig:=oscillator`).
- [ ] `serial_port` set to the Uno's `/dev/serial/by-id/...` path on the Pi.

### F: service on the Pi

- [ ] `rpi/oscillator@.service`, copied from `wavemaker@.service` and, like it, without an
  `[Install]` section, so it never starts (or homes) at boot.
- [ ] Each service refuses to start while the other runs (`ExecStartPre`; `Conflicts=` would
  stop the running one instead).
- [ ] The service user in `dialout`; `rpi/README.md` updated.

### G: commissioning (rig)

- [ ] Without water: activation with homing, `return_to_upright`, small sines (for example 10 mm
  at 2 s), then steps towards 2 m/s, 9.81 m/s² and 5 Hz. Record setpoint and position, and
  check whether VAI at the sine's maxima runs without jitter.
- [ ] Fault paths: soft stop button, e-stop, USB, LinMot-Talk.
- [ ] With water: the same steps; watch the tracking error and the drive's following-error
  limit (20 mm).
- [ ] Pregenerated goals: check the peak acceleration taken from the samples.

### Merge

- [ ] Merge `feat/oscillator` into master once G is far enough along.
- [ ] On the lab Pi, remove `build/` and `install/` of `wavemaker_controller` and rebuild. The
  wavemaker runs unchanged: `actuator_type` defaults to `indradrive`.

## Found on the rig

What the bench and rig tests showed, 2026-10-21, Uno R3 with the MCP2515 shield and the drive:

- The drive is CANopen node 12 (0x0C); the UDP version's node 4 was wrong. The configuration
  has Node-ID Source Select 1 and a parameter value of 63, so 12 most likely comes from the
  drive's address switches (not checked). The shield's INT line is on D2, not D5. Both were found by polling the MCP2515
  directly and printing every frame.
- `mcp_can` prints about 71 bytes of debug text at start-up (`DEBUG_MODE 1` in its
  `mcp_can_dfs.h`); the framing skips it. At port open the Uno resets and answers after about
  0.5 s.
- Round trip Pi → drive → Pi: median 8.5 ms, maximum 10.6 ms. 50 Hz has margin; 100 Hz does not.
- At standstill the slide stays a few 0.1 mm from its target because of friction (more with
  water); `link_test.py` accepts 0.5 mm.
- The slide lags a streamed sine by up to about 2.3 mm at 16 mm/s.
- Homing drives the slide into the 200 mm end stop (Home Position 200 mm), then to the Initial
  Position 100 mm. Positions are only meaningful after homing; the drive loses homing when it
  is powered off and after some errors.
- LinMot-Talk drive configuration (E1200-GP-UC, firmware 6.10; backup `current.lmc`):
  - Minimal / Maximal Position set to 3 / 197 mm, and both checks switched on in the Error
    Detection Mask (they were off, with limits at −57 / 273 mm, the whole motor stroke). The
    drive makes a quick stop if the slide passes them. Homing still works with them on.
  - `/Abort Motion Command` was forced in the Ctrl Word Parameter Force Mask, so the bridge
    could not clear it and the watchdog's abort did nothing. Now not forced; the watchdog
    stops the slide.
  - Quick stop deceleration is 10 m/s²: from 2 m/s that takes 0.2 m. Raising it shortens a
    quick stop at speed but stops harder.
  - The drive has no general velocity or acceleration limit; the following-error limit (warning
    10 mm, error 20 mm) and the 32 A current limit are its only motion checks.
  - While LinMot-Talk's control panel has control, the drive ignores the control word from the
    bus. Release it before running from the Pi.
- Save the drive configuration as a new `.lmc` after changes: the limits and the abort fix exist
  only on the drive.
