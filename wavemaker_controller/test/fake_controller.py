#!/usr/bin/env python3
"""
Fake wavemaker controller for testing wavemaker_bridge without hardware.

Serves the move_wavemaker action like the real controller, without a drive:
  - accepts one goal at a time and rejects goals while one is running ("busy"),
  - runs a regular wave until it is cancelled, sending feedback at 10 Hz,
  - optionally rejects every goal (--reject) or aborts a goal after N s (--abort-after).

Do not run it next to the real controller: both would serve the same action.

    source ~/ros_ws/install/setup.bash
    python3 fake_controller.py --ros-args -r __ns:=/wavemakers/ladertanken
    python3 fake_controller.py --abort-after 5 --ros-args -r __ns:=/wavemakers/ladertanken
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
from wavemaker_interfaces.action import MoveWavemaker


class FakeController(Node):

    def __init__(self, reject, abort_after):
        super().__init__('controller')
        self._reject = reject
        self._abort_after = abort_after
        self._busy = False
        self._lock = threading.Lock()
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

    def _execute(self, goal_handle):
        goal = goal_handle.request
        result = MoveWavemaker.Result()
        feedback = MoveWavemaker.Feedback()
        start = time.monotonic()
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
                position = goal.height / 2.0 * math.sin(2.0 * math.pi * elapsed / goal.period)
                feedback.desired_position = position
                feedback.actual_position = position
                feedback.elapsed_time = elapsed
                goal_handle.publish_feedback(feedback)
                time.sleep(0.1)
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
