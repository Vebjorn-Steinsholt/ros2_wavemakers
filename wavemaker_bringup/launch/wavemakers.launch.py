"""
Launch one wavemaker controller, optionally with the bridge and the Nav2 lifecycle manager.

Terminal use (experienced users): the controller only, lifecycle stepped by hand.
    ros2 launch wavemaker_bringup wavemakers.launch.py wavemaker:=ladertanken

Bridge use (normal users): use wavemaker_bridge.launch.py, which includes this file with
with_bridge:=true and the lifecycle manager on.
"""

from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, OpaqueFunction
from launch_ros.actions import LifecycleNode, Node

WAVEMAKERS = ['ladertanken', 'lilletanken', 'mclab']


def _is_true(context, name):
    return context.launch_configurations[name].lower() in ('true', '1', 'yes')


def _launch_setup(context):
    config_file = get_package_share_directory('wavemaker_bringup') + '/config/wavemakers.yaml'
    wavemaker = context.launch_configurations['wavemaker']
    with_bridge = _is_true(context, 'with_bridge')
    enable_lifecycle_manager = _is_true(context, 'enable_lifecycle_manager')
    bond_timeout = float(context.launch_configurations['bond_timeout'])
    namespace = f'wavemakers/{wavemaker}'

    actions = [
        LifecycleNode(
            package='wavemaker_controller',
            executable='wavemaker_node',
            namespace=namespace,
            name='controller',
            parameters=[config_file, {'bond_timeout': bond_timeout}],
            output='screen',
        )
    ]
    managed_nodes = ['controller']

    if with_bridge:
        actions.append(
            LifecycleNode(
                package='wavemaker_controller',
                executable='wavemaker_bridge',
                namespace=namespace,
                name='wavemaker_bridge',
                parameters=[{'bond_timeout': bond_timeout}],
                output='screen',
            )
        )
        managed_nodes.append('wavemaker_bridge')

    if enable_lifecycle_manager:
        # The manager configures and activates the nodes in order and, if any of them stops
        # sending bond heartbeats for bond_timeout seconds, brings all of them down. With the
        # bridge this is what stops the paddle when the bridge crashes.
        actions.append(
            Node(
                package='nav2_lifecycle_manager',
                executable='lifecycle_manager',
                name='lifecycle_manager_wavemaker',
                namespace=namespace,
                output='screen',
                parameters=[{
                    'autostart': True,
                    'node_names': managed_nodes,
                    'bond_timeout': bond_timeout,
                }],
            )
        )

    return actions


def generate_launch_description():
    return LaunchDescription([
        DeclareLaunchArgument(
            'wavemaker',
            default_value='ladertanken',
            choices=WAVEMAKERS,
            description='Wavemaker to launch',
        ),
        DeclareLaunchArgument(
            'with_bridge',
            default_value='false',
            choices=['true', 'false'],
            description='Also start wavemaker_bridge (normally via wavemaker_bridge.launch.py)',
        ),
        DeclareLaunchArgument(
            'enable_lifecycle_manager',
            default_value='false',
            choices=['true', 'false'],
            description='Start the Nav2 lifecycle manager to configure and activate the nodes',
        ),
        DeclareLaunchArgument(
            'bond_timeout',
            default_value='4.0',
            description='Seconds without a bond heartbeat before the lifecycle manager brings '
                        'the nodes down',
        ),
        OpaqueFunction(function=_launch_setup),
    ])
