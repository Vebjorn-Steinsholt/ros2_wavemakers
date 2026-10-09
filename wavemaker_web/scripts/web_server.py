#!/usr/bin/env python3
"""
Web page for running a wavemaker through wavemaker_bridge, in place of LabVIEW.

Serves the page in html/ and a small JSON API on one port. The page plays LabVIEW's role: the
server publishes height, period, start and stop to the bridge, and the bridge heartbeat only
while an open page keeps acknowledging the server's events. When the page closes, the browser
freezes or the network drops, the heartbeat stops and the bridge stops the wave.

    ros2 run wavemaker_web web_server --ros-args -r __ns:=/wavemakers/ladertanken -p port:=8080

Every open page counts: the wave keeps running while any one of them is alive.
"""

from collections import deque
from http import HTTPStatus
from http.server import BaseHTTPRequestHandler, ThreadingHTTPServer
import json
import math
from pathlib import Path
import re
import socket
import threading
import time

from ament_index_python.packages import get_package_share_directory
import rclpy
from rclpy.executors import ExternalShutdownException, MultiThreadedExecutor
from rclpy.node import Node
from rclpy.qos import DurabilityPolicy, qos_profile_sensor_data, QoSProfile, ReliabilityPolicy
from std_msgs.msg import Bool, Float64, String
from wavemaker_interfaces.srv import ReturnToUpright

ACK_TIMEOUT_S = 0.5      # a page is alive while it acknowledged an event this recently
EVENT_PERIOD_S = 0.1     # how often the page gets an update
PLOT_WINDOW_S = 30.0     # setpoint and position history kept for the plot
START_DELAY_S = 0.3      # between sending height/period and start
START_CHECK_S = 2.0      # how long to wait for the bridge's "Starting wave" message
RETURN_TIMEOUT_S = 60.0
STARTING_RE = re.compile(r'height ([-+0-9.eE]+) m, period ([-+0-9.eE]+) s')
RETURN_STATUS = {
    0: 'SUCCESS', 1: 'CONTROLLER_INACTIVE', 2: 'BUSY', 3: 'ACTUATOR_NOT_READY',
    4: 'TIMEOUT', 5: 'ACTUATOR_FAULT', 6: 'POSITION_INVALID',
}
STATIC_FILES = {
    '/': ('index.html', 'text/html; charset=utf-8'),
    '/index.html': ('index.html', 'text/html; charset=utf-8'),
    '/app.js': ('app.js', 'text/javascript; charset=utf-8'),
    '/styles.css': ('styles.css', 'text/css; charset=utf-8'),
}


class WebNode(Node):

    def __init__(self):
        super().__init__('wavemaker_web')
        self.declare_parameter('host', '0.0.0.0')
        self.declare_parameter('port', 8080)

        self._lock = threading.Lock()
        self._state = 'unknown'
        self._message = ''
        self._message_count = 0
        self._started_with = None  # (message count, height, period) of the last start
        self._notice = ''
        self._samples = deque()   # (index, time, kind, value)
        self._sample_count = 0
        self._last_ack = -math.inf
        self._t0 = time.monotonic()

        command_qos = QoSProfile(depth=10, reliability=ReliabilityPolicy.RELIABLE)
        status_qos = QoSProfile(
            depth=1, reliability=ReliabilityPolicy.RELIABLE,
            durability=DurabilityPolicy.TRANSIENT_LOCAL)
        self._height_pub = self.create_publisher(Float64, 'height', command_qos)
        self._period_pub = self.create_publisher(Float64, 'period', command_qos)
        self._start_pub = self.create_publisher(Bool, 'start', command_qos)
        self._stop_pub = self.create_publisher(Bool, 'stop', command_qos)
        self._heartbeat_pub = self.create_publisher(Bool, 'heartbeat', qos_profile_sensor_data)

        self.create_subscription(String, 'wavemaker_state', self._on_state, status_qos)
        self.create_subscription(String, 'wavemaker_message', self._on_message, status_qos)
        self.create_subscription(
            Float64, 'wavemaker_setpoint', lambda msg: self._on_sample('setpoint', msg), 10)
        self.create_subscription(
            Float64, 'wavemaker_position', lambda msg: self._on_sample('position', msg), 10)
        self._return_client = self.create_client(ReturnToUpright, 'return_to_upright')
        self.create_timer(0.1, self._send_heartbeat)

    # ROS callbacks

    def _on_state(self, msg):
        with self._lock:
            self._state = msg.data

    def _on_message(self, msg):
        match = STARTING_RE.search(msg.data)
        with self._lock:
            self._message = msg.data
            self._message_count += 1
            if match:
                self._started_with = (
                    self._message_count, float(match.group(1)), float(match.group(2)))

    def _on_sample(self, kind, msg):
        now = self._now()
        with self._lock:
            self._sample_count += 1
            self._samples.append((self._sample_count, now, kind, msg.data))
            while self._samples and self._samples[0][1] < now - PLOT_WINDOW_S:
                self._samples.popleft()

    def _send_heartbeat(self):
        if self.page_alive():
            self._heartbeat_pub.publish(Bool(data=True))

    # Used by the HTTP handler

    def _now(self):
        return time.monotonic() - self._t0

    def acknowledge(self):
        with self._lock:
            self._last_ack = time.monotonic()

    def page_alive(self):
        with self._lock:
            return time.monotonic() - self._last_ack < ACK_TIMEOUT_S

    def snapshot(self, after_sample):
        """Return the page update and the index of the last sample it contains."""
        with self._lock:
            samples = [[t, kind, value] for index, t, kind, value in self._samples
                       if index > after_sample]
            update = {
                'namespace': self.get_namespace(),
                'state': self._state,
                'message': self._message,
                'notice': self._notice,
                'time': self._now(),
                'samples': samples,
            }
            return update, self._sample_count

    def start(self, height, period):
        with self._lock:
            state = self._state
            first_message = self._message_count
            self._notice = ''
        if state != 'idle':
            return False, f'The bridge is {state}; it must be idle to start a wave.'
        self._height_pub.publish(Float64(data=height))
        self._period_pub.publish(Float64(data=period))
        time.sleep(START_DELAY_S)
        self._start_pub.publish(Bool(data=True))
        threading.Thread(
            target=self._check_start, args=(height, period, first_message), daemon=True).start()
        return True, 'Start sent.'

    def _check_start(self, height, period, first_message):
        # height, period and start are separate topics and DDS does not order them, so check
        # that the bridge started with the values sent, and stop the wave if it did not.
        deadline = time.monotonic() + START_CHECK_S
        while time.monotonic() < deadline:
            with self._lock:
                started_with = self._started_with
            if started_with and started_with[0] > first_message:
                _, used_height, used_period = started_with
                # The bridge prints the height to 4 and the period to 3 decimals.
                if abs(used_height - height) > 6e-5 or abs(used_period - period) > 6e-4:
                    self._stop_pub.publish(Bool(data=True))
                    with self._lock:
                        self._notice = (
                            f'Stopped: the bridge started with height {used_height} m, period '
                            f'{used_period} s instead of {height} m, {period} s. Start again.')
                return
            time.sleep(0.05)

    def stop(self):
        self._stop_pub.publish(Bool(data=True))
        return True, 'Stop sent.'

    def return_to_upright(self):
        if not self._return_client.wait_for_service(timeout_sec=1.0):
            return False, 'The return_to_upright service is not available.'
        request = ReturnToUpright.Request(requester='web', tolerance=0.0)
        done = threading.Event()
        future = self._return_client.call_async(request)
        future.add_done_callback(lambda _: done.set())
        if not done.wait(RETURN_TIMEOUT_S):
            return False, 'No reply from return_to_upright.'
        response = future.result()
        status = RETURN_STATUS.get(response.status, str(response.status))
        text = f'{status}: {response.message}' if response.message else status
        return response.status == ReturnToUpright.Response.SUCCESS, text


def make_handler(node, html_dir):

    class Handler(BaseHTTPRequestHandler):

        def log_message(self, *args):
            pass  # the page acknowledges every event; logging each request would flood

        def do_GET(self):
            if self.path == '/api/events':
                self._events()
            elif self.path in STATIC_FILES:
                name, content_type = STATIC_FILES[self.path]
                self._send(HTTPStatus.OK, (html_dir / name).read_bytes(), content_type)
            else:
                self._send(HTTPStatus.NOT_FOUND, b'Not found', 'text/plain')

        def do_POST(self):
            if self.path == '/api/ack':
                node.acknowledge()
                self._send(HTTPStatus.NO_CONTENT, b'', 'text/plain')
                return
            if self.path == '/api/start':
                try:
                    body = json.loads(self.rfile.read(int(self.headers['Content-Length'])))
                    height, period = float(body['height']), float(body['period'])
                except (KeyError, TypeError, ValueError):
                    self._json(False, 'Send height and period as numbers.')
                    return
                if not (math.isfinite(height) and math.isfinite(period) and
                        height > 0.0 and period > 0.0):
                    self._json(False, 'Height and period must be positive.')
                    return
                self._json(*node.start(height, period))
            elif self.path == '/api/stop':
                self._json(*node.stop())
            elif self.path == '/api/return':
                self._json(*node.return_to_upright())
            else:
                self._send(HTTPStatus.NOT_FOUND, b'Not found', 'text/plain')

        def _events(self):
            self.send_response(HTTPStatus.OK)
            self.send_header('Content-Type', 'text/event-stream')
            self.send_header('Cache-Control', 'no-cache')
            self.end_headers()
            last_sample = 0
            try:
                while True:
                    update, last_sample = node.snapshot(last_sample)
                    self.wfile.write(f'data: {json.dumps(update)}\n\n'.encode())
                    self.wfile.flush()
                    time.sleep(EVENT_PERIOD_S)
            except (BrokenPipeError, ConnectionResetError):
                pass

        def _json(self, ok, message):
            body = json.dumps({'ok': ok, 'message': message}).encode()
            self._send(HTTPStatus.OK, body, 'application/json')

        def _send(self, status, body, content_type):
            self.send_response(status)
            self.send_header('Content-Type', content_type)
            self.send_header('Content-Length', str(len(body)))
            self.end_headers()
            self.wfile.write(body)

    return Handler


def page_url(host, port):
    """Return the page's address by hostname, as the lab Pis announce it (<hostname>.local)."""
    if host not in ('0.0.0.0', '::', ''):
        return f'http://{host}:{port}/'
    name = socket.gethostname()
    return f'http://{name if "." in name else name + ".local"}:{port}/'


def main():
    rclpy.init()
    node = WebNode()
    html_dir = Path(get_package_share_directory('wavemaker_web')) / 'html'
    host = node.get_parameter('host').value
    port = node.get_parameter('port').value
    server = ThreadingHTTPServer((host, port), make_handler(node, html_dir))
    server.daemon_threads = True
    threading.Thread(target=server.serve_forever, daemon=True).start()
    node.get_logger().info(f'Open the wavemaker page at {page_url(host, port)}')

    executor = MultiThreadedExecutor()
    executor.add_node(node)
    try:
        executor.spin()
    except (KeyboardInterrupt, ExternalShutdownException):
        pass
    finally:
        server.shutdown()
        node.destroy_node()
        if rclpy.ok():
            rclpy.shutdown()


if __name__ == '__main__':
    main()
