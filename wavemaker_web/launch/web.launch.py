"""
Web page for one wavemaker, served by wavemaker_web's web_server.

The page talks to wavemaker_bridge, so start the bridge (wavemaker_bridge.launch.py) as well.
After idle_timeout_s without a wave the server stops the managed nodes and exits, and this launch
(or lab.launch.py, which includes it) shuts down with it. A crash of the server does the same.

    ros2 launch wavemaker_web web.launch.py wavemaker:=ladertanken port:=8080
"""

from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, Shutdown
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node
from launch_ros.parameter_descriptions import ParameterValue


def _shutdown_unless_stopping(event, context):
    # The server also exits during an ordinary shutdown (Ctrl-C, systemctl stop). A second
    # Shutdown then makes launch fail with "Cannot shutdown a ROS adapter that is not running".
    if context.is_shutdown:
        return None
    return [Shutdown(reason='web server exited')]


def generate_launch_description():
    return LaunchDescription([
        DeclareLaunchArgument(
            'wavemaker',
            default_value='ladertanken',
            choices=['ladertanken', 'lilletanken', 'mclab'],
            description='Wavemaker the page controls',
        ),
        DeclareLaunchArgument('port', default_value='8080', description='HTTP port'),
        DeclareLaunchArgument(
            'idle_timeout_s', default_value='7200',
            description='Stop the wavemaker after this long without a wave; 0 turns it off'),
        Node(
            package='wavemaker_web',
            executable='web_server',
            namespace=['wavemakers/', LaunchConfiguration('wavemaker')],
            name='wavemaker_web',
            output='screen',
            parameters=[{
                'port': ParameterValue(LaunchConfiguration('port'), value_type=int),
                'idle_timeout_s': ParameterValue(
                    LaunchConfiguration('idle_timeout_s'), value_type=float),
            }],
            on_exit=_shutdown_unless_stopping,
        ),
    ])
