#!/usr/bin/env bash
# ExecStop: Shut the managed noded down in order(lifecycle manager -> bridge -> controller) before system d sends
# SIGINT. If this fails, systemd will send SIGINT after the configured timeout.

set -eo pipefail

source /opt/ros/jazzy/setup.bash
source "$HOME/ros_ws/install/setup.bash"

# Ros 2 service call has no timeout; without one it waits forever for the service. 
timeout 15 ros2 service call \
  "/wavemakers/$WAVEMAKER/lifecycle_manager_wavemaker/manage_nodes" \
  nav2_msgs/srv/ManageLifecycleNodes "{command: 4}" # 4 corresponds to the SHUTDOWN command in the ManageLifecycleNodes service
  