"""
Everything a lab Pi runs: controller, bridge, lifecycle manager and web page.

Started by lab staff, normally through the wavemaker service (see pi/README.md):

    ros2 launch wavemaker_bringup lab.launch.py wavemaker:=ladertanken port:=8080

The lifecycle manager configures and activates the controller at start, so the drive is enabled
as soon as this is running. Ctrl-C (SIGINT) stops everything and the controller disables the
drive.
"""

from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, IncludeLaunchDescription
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.substitutions import LaunchConfiguration


def generate_launch_description():
    bridge_launch = (
        get_package_share_directory('wavemaker_bringup') + '/launch/wavemaker_bridge.launch.py')
    web_launch = get_package_share_directory('wavemaker_web') + '/launch/web.launch.py'
    return LaunchDescription([
        DeclareLaunchArgument(
            'wavemaker',
            default_value='ladertanken',
            choices=['ladertanken', 'lilletanken', 'mclab'],
            description='Wavemaker to run',
        ),
        DeclareLaunchArgument('port', default_value='8080', description='Web page port'),
        IncludeLaunchDescription(
            PythonLaunchDescriptionSource(bridge_launch),
            launch_arguments={'wavemaker': LaunchConfiguration('wavemaker')}.items(),
        ),
        IncludeLaunchDescription(
            PythonLaunchDescriptionSource(web_launch),
            launch_arguments={
                'wavemaker': LaunchConfiguration('wavemaker'),
                'port': LaunchConfiguration('port'),
            }.items(),
        ),
    ])
