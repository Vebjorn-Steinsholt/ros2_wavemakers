#!/usr/bin/env python3
"""
Fake wavemaker controller for testing wavemaker_bridge without hardware.

Serves the move_wavemaker action like the real controller, without a drive:
  - accepts one goal at a time and rejects goals while one is running ("busy"),
  - runs a regular wave until it is cancelled, sending feedback at 10 Hz and the setpoint and
    a slightly lagging "measured" position on wavemaker_setpoint / wavemaker_position at 50 Hz,
  - holds its position after a cancel, and serves return_to_upright (back to 0 in 1.5 s),
  - optionally rejects every goal (--reject) or aborts a goal after N s (--abort-after).

Do not run it next to the real controller: both would serve the same action.

    source ~/ros_ws/install/setup.bash
    ros2 run wavemaker_controller fake_controller --ros-args -r __ns:=/wavemakers/ladertanken
    ros2 run wavemaker_controller fake_controller --abort-after 5 \
      --ros-args -r __ns:=/wavemakers/ladertanken
"""

import argparse
import math
import sys
import threading
import time

import rclpy
from rclpy.action import ActionServer, CancelResponse, GoalResponse
from rclpy.callback_groups import ReentrantCallbackGroup
from rclpy.executors import MultiThreadedExecutor
from rclpy.node import Node
from rclpy.utilities import remove_ros_args
from std_msgs.msg import Float64
from wavemaker_interfaces.action import MoveWavemaker
from wavemaker_interfaces.srv import ReturnToUpright

LOOP_S = 0.02            # setpoint rate, as the real controller at poll_interval_ms 20
LAG_S = 0.04             # the fake "measured" position trails the setpoint by this much
RETURN_DURATION_S = 1.5


class FakeController(Node):

    def __init__(self, reject, abort_after):
        super().__init__('controller')
        self._reject = reject
        self._abort_after = abort_after
        self._busy = False
        self._position = 0.0     # last setpoint, held after a stop
        self._lock = threading.Lock()
        self._setpoint_pub = self.create_publisher(Float64, 'wavemaker_setpoint', 10)
        self._measured_pub = self.create_publisher(Float64, 'wavemaker_position', 10)
        self.create_service(
            ReturnToUpright, 'return_to_upright', self._return_to_upright,
            callback_group=ReentrantCallbackGroup())
        self._server = ActionServer(
            self, MoveWavemaker, 'move_wavemaker',
            execute_callback=self._execute,
            goal_callback=self._on_goal,
            cancel_callback=self._on_cancel,
            callback_group=ReentrantCallbackGroup())
        self.get_logger().info(
            f'Fake controller ready (reject={reject}, abort_after={abort_after})')

    def _on_goal(self, goal):
        with self._lock:
            if self._reject:
                self.get_logger().warn('Rejecting goal (--reject)')
                return GoalResponse.REJECT
            if self._busy:
                self.get_logger().warn('Rejecting goal: another goal is running')
                return GoalResponse.REJECT
            if goal.height <= 0.0 or goal.period <= 0.0:
                self.get_logger().warn('Rejecting goal: height and period must be > 0')
                return GoalResponse.REJECT
            self._busy = True
        self.get_logger().info(
            f'Accepted goal: height {goal.height} m, period {goal.period} s')
        return GoalResponse.ACCEPT

    def _on_cancel(self, goal_handle):
        self.get_logger().info('Cancel requested')
        return CancelResponse.ACCEPT

    def _publish(self, setpoint, measured):
        self._position = setpoint
        self._setpoint_pub.publish(Float64(data=setpoint))
        self._measured_pub.publish(Float64(data=measured))

    def _return_to_upright(self, request, response):
        with self._lock:
            if self._busy:
                response.status = ReturnToUpright.Response.BUSY
                response.message = 'A goal is running (fake)'
                return response
            self._busy = True
        try:
            origin = self._position
            start = time.monotonic()
            while (elapsed := time.monotonic() - start) < RETURN_DURATION_S:
                u = elapsed / RETURN_DURATION_S
                blend = 10 * u**3 - 15 * u**4 + 6 * u**5
                u_lag = max(0.0, elapsed - LAG_S) / RETURN_DURATION_S
                blend_lag = 10 * u_lag**3 - 15 * u_lag**4 + 6 * u_lag**5
                self._publish(origin * (1.0 - blend), origin * (1.0 - blend_lag))
                time.sleep(LOOP_S)
            self._publish(0.0, 0.0)
        finally:
            with self._lock:
                self._busy = False
        self.get_logger().info(f'Returned to upright from {origin:.4f} m (requester '
                               f'{request.requester!r})')
        response.status = ReturnToUpright.Response.SUCCESS
        response.final_position = 0.0
        response.message = 'At upright (fake)'
        return response

    def _execute(self, goal_handle):
        goal = goal_handle.request
        result = MoveWavemaker.Result()
        feedback = MoveWavemaker.Feedback()
        amplitude = goal.height / 2.0
        omega = 2.0 * math.pi / goal.period
        origin = self._position
        start = time.monotonic()
        cycle = 0
        try:
            while rclpy.ok():
                elapsed = time.monotonic() - start
                if goal_handle.is_cancel_requested:
                    goal_handle.canceled()
                    result.success = False
                    result.message = 'Goal canceled; holding position (fake)'
                    self.get_logger().info('Goal canceled')
                    return result
                if self._abort_after is not None and elapsed >= self._abort_after:
                    goal_handle.abort()
                    result.success = False
                    result.message = 'Simulated drive fault (fake)'
                    self.get_logger().warn('Goal aborted (--abort-after)')
                    return result
                # Fade from the held position into the wave over the first period.
                fade = min(1.0, elapsed / goal.period)
                position = (1.0 - fade) * origin + fade * amplitude * math.sin(omega * elapsed)
                lagged = max(0.0, elapsed - LAG_S)
                lagged_fade = min(1.0, lagged / goal.period)
                measured = ((1.0 - lagged_fade) * origin +
                            lagged_fade * amplitude * math.sin(omega * lagged))
                self._publish(position, measured)
                cycle += 1
                if cycle % 5 == 0:
                    feedback.desired_position = position
                    feedback.actual_position = measured
                    feedback.elapsed_time = elapsed
                    goal_handle.publish_feedback(feedback)
                time.sleep(LOOP_S)
            goal_handle.abort()
            result.message = 'Shutting down'
            return result
        finally:
            with self._lock:
                self._busy = False


def main():
    parser = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    parser.add_argument('--reject', action='store_true', help='reject every goal')
    parser.add_argument(
        '--abort-after', type=float, default=None, metavar='SECONDS',
        help='abort each goal after this many seconds (simulates a fault)')
    args = parser.parse_args(remove_ros_args(sys.argv)[1:])

    rclpy.init(args=sys.argv)
    node = FakeController(args.reject, args.abort_after)
    executor = MultiThreadedExecutor()
    executor.add_node(node)
    try:
        executor.spin()
    except KeyboardInterrupt:
        pass
    finally:
        node.destroy_node()
        if rclpy.ok():
            rclpy.shutdown()


if __name__ == '__main__':
    main()
