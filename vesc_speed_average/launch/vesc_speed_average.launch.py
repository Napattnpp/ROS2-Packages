from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node


def generate_launch_description():
    input_topic = LaunchConfiguration('input_topic')
    output_topic = LaunchConfiguration('output_topic')
    window_size = LaunchConfiguration('window_size')
    publish_rate_hz = LaunchConfiguration('publish_rate_hz')

    return LaunchDescription([
        DeclareLaunchArgument('input_topic', default_value='/sensors/core.state.speed'),
        DeclareLaunchArgument('output_topic', default_value='/vesc_to_odom'),
        DeclareLaunchArgument('window_size', default_value='10'),
        DeclareLaunchArgument('publish_rate_hz', default_value='50.0'),
        Node(
            package='vesc_speed_average',
            executable='vesc_speed_average_node',
            name='vesc_speed_average_node',
            output='screen',
            parameters=[{
                'input_topic': input_topic,
                'output_topic': output_topic,
                'window_size': window_size,
                'publish_rate_hz': publish_rate_hz,
            }],
        ),
    ])
