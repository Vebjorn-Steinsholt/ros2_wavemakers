"""
Try the web page without hardware: fake controller, bridge, lifecycle manager and web page.

Everything runs on its own ROS domain (default 77) and only on this computer, so the demo can
never reach LabVIEW, another computer or the real controller. The web server prints the page's
address, http://<hostname>.local:8080/ (for example http://ladertanken-rpi.local:8080/).

    ros2 launch wavemaker_web demo.launch.py
"""

from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, SetEnvironmentVariable
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node
from launch_ros.parameter_descriptions import ParameterValue

NAMESPACE = 'wavemakers/ladertanken'


def generate_launch_description():
    port = LaunchConfiguration('port')
    return LaunchDescription([
        DeclareLaunchArgument('port', default_value='8080', description='HTTP port'),
        DeclareLaunchArgument(
            'domain_id', default_value='77',
            description='ROS domain for the demo; keep it different from the lab domain'),
        SetEnvironmentVariable('ROS_DOMAIN_ID', LaunchConfiguration('domain_id')),
        SetEnvironmentVariable('ROS_AUTOMATIC_DISCOVERY_RANGE', 'LOCALHOST'),
        Node(
            package='wavemaker_controller', executable='fake_controller',
            namespace=NAMESPACE, name='controller', output='screen'),
        Node(
            package='wavemaker_controller', executable='wavemaker_bridge',
            namespace=NAMESPACE, name='wavemaker_bridge', output='screen',
            parameters=[{'bond_timeout': 1.0}]),
        Node(
            package='nav2_lifecycle_manager', executable='lifecycle_manager',
            namespace=NAMESPACE, name='lifecycle_manager_wavemaker', output='screen',
            parameters=[{
                'autostart': True,
                'node_names': ['wavemaker_bridge'],
                'bond_timeout': 1.0,
            }]),
        Node(
            package='wavemaker_web', executable='web_server',
            namespace=NAMESPACE, name='wavemaker_web', output='screen',
            parameters=[{'port': ParameterValue(port, value_type=int)}]),
    ])
