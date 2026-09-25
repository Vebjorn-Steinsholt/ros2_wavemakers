# ROS 2 Wavemakers

ROS 2 packages for controlling the Hydrolab wavemakers through an MGate 5101 Modbus/PROFIBUS gateway.

## Packages

- `mgate5101_driver`: Standalone C++ driver for the MGate 5101 gateway.
- `wavemaker_interfaces`: ROS 2 action definitions.
- `wavemaker_controller`: Lifecycle-based wavemaker controller and bridge nodes.
- `wavemaker_bringup`: Launch files and wavemaker configuration.

## Safety

Do not start physical motion until the checks in [TODO.md](TODO.md) are complete. In particular, verify the emergency stop, mechanical travel, actuator conversion, software limits, feedback direction, communication, and physical zero position before activating a controller or sending a motion goal.

## Prerequisites

The workspace requires a ROS 2 installation with the packages used by this project, including:

- `ament_cmake`
- `rclcpp` and `rclcpp_lifecycle`
- `rclcpp_action`
- `bondcpp`
- `nav2_lifecycle_manager`

Source the ROS 2 installation before building or running the workspace:

```bash
source /opt/ros/<ros-distribution>/setup.bash
```

Replace `<ros-distribution>` with the installed ROS 2 distribution.

## Build

Run the commands from the workspace root:

```bash
cd /home/hydrolab-linux/ros_ws
source /opt/ros/<ros-distribution>/setup.bash
colcon build
source install/setup.bash
```

To build only the bringup package and its workspace dependencies:

```bash
colcon build --packages-up-to wavemaker_bringup
source install/setup.bash
```

## Launch a Wavemaker

The bringup launch file starts one selected controller. The default selection is `ladertanken`.

Start `ladertanken` with the Nav2 lifecycle manager disabled for initial testing:

```bash
ros2 launch wavemaker_bringup wavemakers.launch.py \
  wavemaker:=ladertanken
```

The available wavemaker names are:

- `ladertanken`
- `lilletanken`
- `mc_lab`

For example:

```bash
ros2 launch wavemaker_bringup wavemakers.launch.py wavemaker:=lilletanken
ros2 launch wavemaker_bringup wavemakers.launch.py wavemaker:=mc_lab
```

## Lifecycle Manager

The Nav2 lifecycle manager is disabled by default. This allows the controller lifecycle to be tested manually before automatic activation is used.

Enable it for a selected wavemaker with:

```bash
ros2 launch wavemaker_bringup wavemakers.launch.py \
  wavemaker:=ladertanken \
  enable_lifecycle_manager:=true
```

When enabled, the lifecycle manager autostarts and manages the selected controller. Do not enable it until the safety and lifecycle checks in [TODO.md](TODO.md) are complete.

## Manual Lifecycle Testing

With the lifecycle manager disabled, inspect and change the controller state manually. The namespace must match the selected wavemaker:

```bash
ros2 lifecycle get /wavemakers/ladertanken/controller
ros2 lifecycle set /wavemakers/ladertanken/controller configure
ros2 lifecycle set /wavemakers/ladertanken/controller activate
```

To stop and reset the controller:

```bash
ros2 lifecycle set /wavemakers/ladertanken/controller deactivate
ros2 lifecycle set /wavemakers/ladertanken/controller cleanup
```

Replace `ladertanken` with `lilletanken` or `mc_lab` when testing another wavemaker.

## Topics and Actions

Monitor controller feedback during testing:

```bash
ros2 topic echo /wavemakers/ladertanken/controller/wavemaker_velocity
```

Send a small action goal only after the controller is active and the physical safety checks are complete:

```bash
ros2 action send_goal --feedback \
  /wavemakers/ladertanken/controller/move_wavemaker \
  wavemaker_interfaces/action/MoveWavemaker \
  "{amplitude: 0.001, period: 10.0}"
```

The controller configuration is in:

```text
wavemaker_bringup/config/wavemakers.yaml
```

Check the configured network address, Modbus port, unit ID, PROFIBUS address, wavemaker limits, actuator conversion, and calibration values before connecting to hardware.

## Driver Checks

The standalone driver includes a mapping test. After building, run the package tests with:

```bash
cd /home/hydrolab-linux/ros_ws
colcon test --packages-select mgate5101_driver
colcon test-result --verbose
```

## Recommended Test Order

1. Complete the physical checks in [TODO.md](TODO.md).
2. Launch one wavemaker with `enable_lifecycle_manager:=false`.
3. Verify communication without motion.
4. Test lifecycle transitions manually.
5. Confirm activation causes no unexpected motion.
6. Send a very small, low-frequency goal.
7. Test cancellation, deactivation, communication loss, and emergency-stop behavior.
8. Enable the lifecycle manager only after manual lifecycle testing is successful.
