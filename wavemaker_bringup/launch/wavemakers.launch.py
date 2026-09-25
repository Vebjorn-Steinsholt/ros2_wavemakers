from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.conditions import IfCondition
from launch.substitutions import EqualsSubstitution, LaunchConfiguration, PythonExpression
from launch_ros.actions import LifecycleNode, Node


def generate_launch_description():
    config_file = get_package_share_directory("wavemaker_bringup") + "/config/wavemakers.yaml"

    wavemakers = ["ladertanken", "lilletanken", "mc_lab"]
    actions = [
        DeclareLaunchArgument(
            "wavemaker",
            default_value="ladertanken",
            choices=wavemakers,
            description="Wavemaker to launch",
        ),
        DeclareLaunchArgument(
            "enable_lifecycle_manager",
            default_value="false",
            description="Start Nav2 lifecycle manager for the selected wavemaker",
        ),
    ]

    for wavemaker in wavemakers:
        condition = IfCondition(
            EqualsSubstitution(LaunchConfiguration("wavemaker"), wavemaker)
        )
        namespace = f"wavemakers/{wavemaker}"

        actions.append(
            LifecycleNode(
                package="wavemaker_controller",
                executable="wavemaker_node",
                namespace=namespace,
                name="controller",
                parameters=[config_file],
                condition=condition,
                output="screen",
            )
        )

        actions.append(
            Node(
                package="nav2_lifecycle_manager",
                executable="lifecycle_manager",
                name="lifecycle_manager_wavemaker",
                namespace=namespace,
                output="screen",
                parameters=[{
                    "autostart": True,
                    "node_names": ["controller"],
                    "bond_timeout": 4.0,
                }],
                condition=IfCondition(
                    PythonExpression([
                        "'",
                        LaunchConfiguration("enable_lifecycle_manager"),
                        "' == 'true' and '",
                        LaunchConfiguration("wavemaker"),
                        "' == '",
                        wavemaker,
                        "'",
                    ])
                ),
            )
        )

    return LaunchDescription(actions)