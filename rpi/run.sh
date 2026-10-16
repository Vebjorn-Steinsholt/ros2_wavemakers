#!/usr/bin/env bash
set -eo pipefail

# Refuse domain 0: anyone on the network could command the wavemaker
[[ -n "$ROS_DOMAIN_ID" && "$ROS_DOMAIN_ID" != 0 ]] || { echo "set ROS_DOMAIN_ID" >&2; exit 1; }

source /opt/ros/jazzy/setup.bash
source "$HOME/ros_ws/install/setup.bash"

args=(wavemaker:="$WAVEMAKER" port:="${PORT:-8080}")
[[ -n "$WATER_DEPTH" ]] && args+=(water_depth:="$WATER_DEPTH")

# exec: systemd's SIGINT must reach ros2 launch, not bash.
# sigterm_timeout: launch escalates to SIGTERM after 5 s by default; give the controller time.
exec ros2 launch wavemaker_bringup lab.launch.py "${args[@]}" \
  sigterm_timeout:=20 sigkill_timeout:=5
