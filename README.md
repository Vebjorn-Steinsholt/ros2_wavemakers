# ROS 2 Wavemakers

ROS 2 (Jazzy) packages for controlling the Hydrolab wavemakers. A lifecycle controller node generates
the paddle motion and sends it to a Rexroth IndraDrive through a Moxa MGate 5101-PBM-MN
Modbus TCP / PROFIBUS gateway.

```
 ROS 2 client ──action/services──▶ controller node ──Modbus TCP──▶ MGate 5101 ──PROFIBUS──▶ IndraDrive ──▶ paddle
 (ros2 CLI, bridge)                (wavemaker_controller)           (gateway)                (servo drive)
```

> **Safety first.** Do not start physical motion until the checks in [TODO.md](TODO.md) are complete:
> emergency stop, mechanical travel, actuator conversion, software limits, feedback direction,
> communication, and the physical zero position. [TODO.md](TODO.md) also tracks the open software
> issues; several affect behaviour on the real drive.

## Contents

- [Packages](#packages)
- [Build](#build)
- [Configuration](#configuration)
- [Running a wavemaker](#running-a-wavemaker)
- [Interfaces](#interfaces)
- [How a motion runs](#how-a-motion-runs)
- [Faults and stopping](#faults-and-stopping)
- [Choosing the poll interval](#choosing-the-poll-interval)
- [Tests](#tests)
- [Recommended test order on the rig](#recommended-test-order-on-the-rig)

## Packages

| Package | Contents |
|---|---|
| `drivers/mgate5101_driver` | Standalone C++ driver: cyclic Modbus TCP client for the MGate (`MGateDriver`) and the IndraDrive state machine on top of it (`IndraDrive`). Includes `poll_rate_probe` and a monitor/move example. Register mapping is explained in [MAPPING.md](drivers/mgate5101_driver/MAPPING.md). |
| `wavemaker_interfaces` | `MoveWavemaker` action, `ReturnToUpright` and `CancelAllGoals` services. |
| `wavemaker_controller` | `wavemaker_node` (the lifecycle controller), the wave and trajectory maths, the IndraDrive actuator adapter, and `wavemaker_bridge` (see the note below). |
| `wavemaker_bringup` | Launch file and per-wavemaker configuration (`config/wavemakers.yaml`). |

> **`wavemaker_bridge`** connects LabVIEW (RTI DDS, topics only) to the controller. It works with
> the fake controller but has not been tested on the rig, and its LabVIEW interface is not yet
> documented here (TODO 16). Its topics are `amplitude`, `period`, `start`, `stop` and `heartbeat`
> in, and `wavemaker_state` / `wavemaker_message` out.

## Build

Requires ROS 2 Jazzy with `rclcpp`, `rclcpp_lifecycle`, `rclcpp_action`, `bondcpp`, `std_msgs`
and `nav2_lifecycle_manager`.

```bash
cd ~/ros_ws
source /opt/ros/jazzy/setup.bash
colcon build --packages-up-to wavemaker_bringup
source install/setup.bash
```

## Configuration

All wavemakers are configured in
[`wavemaker_bringup/config/wavemakers.yaml`](wavemaker_bringup/config/wavemakers.yaml), one block
per wavemaker (`ladertanken`, `lilletanken`, `mc_lab`). Check every value against the real rig
before connecting to hardware. Changes take effect at the next `configure`.

### Wavemaker and geometry

| Parameter | Meaning |
|---|---|
| `wavemaker_type` | `piston` or `flap`. Selects the wave transfer function. |
| `water_depth` | Water depth in m. |
| `flap_attachment_height` | Flap only: height of the actuator attachment above the hinge, m. Must be greater than `water_depth`. |
| `wavemaker_upright_position_m` | Paddle position at upright (the mean position), m. Returns go here; waves oscillate around it. |
| `wavemaker_upright_is_minimum` | `true`: the wave runs from upright to one side only. `false`: symmetric around upright. |
| `wavemaker_mode_pregenerated` | `true`: goals carry sampled `positions`. `false`: goals carry `amplitude` and `period`. |
| `wavemaker_transfer_gain` | Measured / target wave height for regular waves (default 1.0 = linear theory). The stroke is divided by it; to correct from wave-probe data, new gain = old gain × measured / target. Not applied to pregenerated waves. |

### Actuator and limits

| Parameter | Meaning |
|---|---|
| `actuator_drive_type` | `angular` (drive position in degrees, converted with the lead) or `linear`. Linear units are not verified yet (TODO 19). |
| `actuator_lead_m_per_degree` | Angular only: paddle travel per drive degree. |
| `actuator_upright_angle_deg` | Angular only: drive angle at upright. |
| `min_position_deg` / `max_position_deg` | Angular travel limits in drive degrees. Goals that leave them are rejected; the drive also refuses targets outside them. |
| `wavemaker_minimum` / `wavemaker_maximum`, `min_position_m` / `max_position_m` | Linear travel limits in m (node and drive side; keep them equal). |
| `max_velocity_rpm` | Drive speed limit. Goals whose peak speed exceeds it are rejected. |
| `hold_velocity_rpm` | Speed cap used when stopping and holding position (default 10, ≤ `max_velocity_rpm`). |

### Communication

| Parameter | Meaning |
|---|---|
| `driver_address`, `driver_port`, `driver_unit_id` | MGate IP address, Modbus port and unit ID. |
| `input_base_reg` / `input_word_count`, `output_base_reg` / `output_word_count` | Process-data areas in the MGate (see [MAPPING.md](drivers/mgate5101_driver/MAPPING.md)). |
| `status_base_reg` / `status_word_count` | Gateway status word and PROFIBUS live list (768 / 9). |
| `profibus_address` | The drive's PROFIBUS address, used to check that it is live. |
| `poll_interval_ms` | Gateway exchange period. **The control loop runs at this rate.** See [Choosing the poll interval](#choosing-the-poll-interval). |
| `write_only_dirty` | Keep `false`, so outputs are written every cycle (needed for the MGate output fault timeout). |
| `timeout_ms`, `reconnect_ms`, `step_timeout_ms` | Modbus timeout, reconnect delay, and timeout per drive state-machine step. |

### Motion tuning (defaults apply if not set)

| Parameter | Default | Meaning |
|---|---|---|
| `return_to_upright_max_velocity_mps` | 0.1 | Peak speed of a return to upright, and of the blend at the start of a pregenerated goal. Must not exceed the drive's limit. |
| `return_to_upright_min_duration_s` | 1.0 | Shortest return or blend. |
| `return_to_upright_default_tolerance_m` | 0.002 | Tolerance for "at upright"; also the margin allowed outside the travel limits at the start of a motion. |
| `return_to_upright_timeout_margin_s` | 2.0 | Extra time after the planned return before it times out. |
| `positioning_velocity_margin` | 1.2 | The drive's speed cap is the motion's peak speed × this. Raise it if the paddle lags at its fastest point; lower it if it moves in steps. |

## Running a wavemaker

There are two ways to run a wavemaker. Both read `config/wavemakers.yaml` and take
`wavemaker:=ladertanken|lilletanken|mc_lab` (default `ladertanken`).

| | Terminal mode (experienced users) | Bridge mode (normal users) |
|---|---|---|
| Launch file | `wavemakers.launch.py` | `wavemaker_bridge.launch.py` |
| Nodes | Controller only | Controller and `wavemaker_bridge` |
| Lifecycle | Stepped by hand (lifecycle manager optional, off by default) | Configured and activated automatically by the Nav2 lifecycle manager |
| Control | `ros2 action` / `ros2 service` (see [Interfaces](#interfaces)) | The bridge's topics: `amplitude`, `period`, then `start`; `stop`; a `heartbeat` from LabVIEW |
| If a node dies | Nothing stops the controller | The lifecycle manager brings both nodes down after `bond_timeout` (default 1 s), which disables the drive |

> Bridge mode has not been tested on the rig yet (TODO 10, 16). For first tests, use terminal mode.

### Terminal mode

```bash
ros2 launch wavemaker_bringup wavemakers.launch.py wavemaker:=ladertanken
```

The controller starts unconfigured. Step through the lifecycle by hand; replace `ladertanken`
with the selected wavemaker.

```bash
ros2 lifecycle set /wavemakers/ladertanken/controller configure   # reads parameters, no connection yet
ros2 lifecycle set /wavemakers/ladertanken/controller activate    # starts the gateway link, enables the drive
ros2 lifecycle get /wavemakers/ladertanken/controller

ros2 lifecycle set /wavemakers/ladertanken/controller deactivate  # stops motion, disables the drive
ros2 lifecycle set /wavemakers/ladertanken/controller cleanup     # frees everything
```

After a driver fault the controller ends up **unconfigured** and must be configured and activated
again (see [Faults and stopping](#faults-and-stopping)). To let the lifecycle manager do the
configure and activate steps, add `enable_lifecycle_manager:=true`; only do this after manual
lifecycle testing.

### Bridge mode

```bash
ros2 launch wavemaker_bringup wavemaker_bridge.launch.py wavemaker:=ladertanken
```

Both nodes are configured and activated as soon as they start, so the drive is enabled
straight away. If either node stops sending bond heartbeats for `bond_timeout` seconds (a crash,
a hang, or the bridge being killed), the lifecycle manager brings both down and the paddle
stops. Set `bond_timeout:=<seconds>` to change this; too short a value can trigger on a briefly
busy PC.

Launch arguments of `wavemakers.launch.py`: `wavemaker`, `with_bridge` (default `false`),
`enable_lifecycle_manager` (default `false`) and `bond_timeout` (default `4.0`, passed to the
lifecycle manager and to both nodes). `wavemaker_bridge.launch.py` sets the first three for you and
defaults `bond_timeout` to `1.0`.

## Interfaces

All names are under `/wavemakers/<wavemaker>/`.

### Action `move_wavemaker` (`wavemaker_interfaces/action/MoveWavemaker`)

Regular wave (`wavemaker_mode_pregenerated: false`). `amplitude` is the wave amplitude in m
(half the wave height) and `period` is in s; the controller converts it to a paddle stroke with the
wave transfer function. The wave runs until it is cancelled.

```bash
ros2 action send_goal --feedback /wavemakers/ladertanken/move_wavemaker \
  wavemaker_interfaces/action/MoveWavemaker "{amplitude: 0.00005, period: 10.0}"
```

For ladertanken (flap, 1.0 m depth, 1.30 m attachment, 0.00053 m/deg) this is about ±1.2° at the
drive. Recalculate if those values change.

Pregenerated (`wavemaker_mode_pregenerated: true`): absolute paddle positions in m at a fixed
`sample_interval` in s. The goal succeeds after the last sample.

```bash
ros2 action send_goal --feedback /wavemakers/ladertanken/move_wavemaker \
  wavemaker_interfaces/action/MoveWavemaker \
  "{positions: [-0.00053, 0.0, 0.00053, 0.0, -0.00053], sample_interval: 1.0}"
```

A goal is **rejected** if the controller is not active, another goal or a return is running, the
drive is faulted or not live, the paddle is outside the travel limits (plus the tolerance), any
position the motion needs is outside the limits, or its peak speed exceeds the drive's limit.

Feedback: `desired_position`, `actual_position` (m) and `elapsed_time` (s).

Cancel a goal with Ctrl-C in `ros2 action send_goal`, or cancel everything with the service below.
A cancelled wave stops and holds position; call `return_to_upright` to go back to upright
(see TODO 18).

### Service `return_to_upright` (`wavemaker_interfaces/srv/ReturnToUpright`)

Moves the paddle to `wavemaker_upright_position_m` with a smooth (quintic) profile and replies when
it is there. `tolerance` 0 uses the default.

```bash
ros2 service call /wavemakers/ladertanken/return_to_upright \
  wavemaker_interfaces/srv/ReturnToUpright "{requester: 'cli', tolerance: 0.0}"
```

Reply `status`: `SUCCESS`, `CONTROLLER_INACTIVE`, `BUSY`, `ACTUATOR_NOT_READY`, `TIMEOUT`,
`ACTUATOR_FAULT` or `POSITION_INVALID`, with `final_position` and a `message`.

### Service `cancel_all_goals` (`wavemaker_interfaces/srv/CancelAllGoals`)

```bash
ros2 service call /wavemakers/ladertanken/cancel_all_goals \
  wavemaker_interfaces/srv/CancelAllGoals "{requester: 'cli'}"
```

Replies when the cancel is accepted, not when the paddle has stopped.

### Topics (`std_msgs/Float64`, published every control cycle during goals and returns)

| Topic | Content |
|---|---|
| `wavemaker_setpoint` | Paddle position sent to the drive, m (after clamping to the limits). |
| `wavemaker_position` | Measured paddle position, m. |
| `wavemaker_velocity` | Trajectory velocity, m/s. |
| `wavemaker_actuator_setpoint` | Debug: `wavemaker_setpoint` as sent to the drive, in actuator units (degrees for an angular drive). |

Plot setpoint against position to see how well the drive tracks:

```bash
ros2 run rqt_plot rqt_plot /wavemakers/ladertanken/wavemaker_setpoint/data \
  /wavemakers/ladertanken/wavemaker_position/data
```

## How a motion runs

- **Start blend.** A regular wave blends from the measured position into the wave over one period
  (quintic smoothstep, phase-continuous). A pregenerated goal blends from the measured position to
  its first sample at the return speed, then plays the samples with linear interpolation.
- **Control loop.** Runs at `1000 / poll_interval_ms` Hz on the steady clock, so every setpoint
  reaches the drive.
- **Drive commands.** The drive runs in drive-internal interpolation: each cycle it gets a target
  position and a speed cap. The cap is the motion's peak speed × `positioning_velocity_margin`
  (never above `max_velocity_rpm`).
- **Stopping.** At the end of a goal, on cancel, and after a return, the drive holds the last target
  and stays enabled, ready for the next motion. Deactivation disables the drive.

## Faults and stopping

| Event | What happens |
|---|---|
| Drive error, PROFIBUS slave lost, or Modbus link lost | The driver latches a fault and sets the control word to 0 (drive off). The controller aborts the running goal and deactivates; it ends up unconfigured. Configure and activate again after fixing the cause. |
| Setpoint rejected by the drive | Same as a fault: goal aborted, drive stopped, controller deactivated. |
| Lifecycle deactivate / cleanup / shutdown | Running goal aborted, return stopped, drive disabled. |
| Ctrl-C on the controller | The node stops the drive in its destructor. |
| PC or controller crashes | The gateway keeps the last outputs unless its **output fault timeout** is set. Configure it on the MGate (fault value 0, a few times `poll_interval_ms`, e.g. 500 ms, longer than the ~210 ms stalls seen on ladertanken) so the drive switches off by itself. |

## Choosing the poll interval

The gateway exchanges data with the drive every `poll_interval_ms`, and the control loop runs at
that rate. Shorter intervals give smoother motion, as long as the gateway keeps up. Measure it on
the rig, **with the controller stopped and the drive disabled** (the tool writes control word 0):

```bash
ros2 run mgate5101_driver poll_rate_probe --seconds 10
# options: --host 192.168.1.70 --port 502 --intervals 50,40,30,20,15,10,5,2
```

It reports cycle times, overruns and errors per interval and recommends the shortest usable one.
Set `poll_interval_ms` to that (or the next longer value), and keep the MGate output fault timeout
at several times it. The register layout in the tool matches ladertanken.

## Tests

```bash
cd ~/ros_ws
colcon build --packages-select mgate5101_driver wavemaker_controller
colcon test --packages-select mgate5101_driver wavemaker_controller
colcon test-result --verbose
```

- `test_wavemaker_node`: lifecycle, goals, cancel, return to upright, faults, speed limits, clamping
  and topics, using a fake actuator.
- `test_wave_trajectory`: start blends, peak-speed bounds and pregenerated sampling.
- `test_indradrive_actuator`: unit conversion and parameter checks.
- `mapping` (driver): register mapping.

The fake actuator does not model all drive behaviour, so passing tests do not replace the rig
checks. `colcon test` also runs the linters; uncrustify and lint_cmake still report style issues
in older files (TODO 32).

## Recommended test order on the rig

1. Complete the physical checks in [TODO.md](TODO.md).
2. Run `poll_rate_probe` and set `poll_interval_ms`; configure the MGate output fault timeout.
3. Launch one wavemaker with the lifecycle manager off, and check communication without motion
   (`mgate_example` in monitor mode, or configure only).
4. Test the lifecycle transitions by hand and confirm that activation causes no motion.
5. Call `return_to_upright`, then send a very small, low-frequency goal.
6. Test cancel, then a second goal, deactivation while moving, communication loss and the
   emergency stop.
7. Plot `wavemaker_setpoint` against `wavemaker_position` and tune `positioning_velocity_margin`.
8. Enable the lifecycle manager only after manual testing succeeds.
