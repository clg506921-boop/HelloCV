# 一次启动 talker + listener + tf_broadcaster
# 演示：参数从 launch 传、话题 remap、多个节点一起起
from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node
from launch_ros.parameter_descriptions import ParameterValue


def generate_launch_description():
    prefix = LaunchConfiguration('prefix')
    rate = LaunchConfiguration('rate')

    return LaunchDescription([
        DeclareLaunchArgument('prefix', default_value='launch', description='消息前缀'),
        DeclareLaunchArgument('rate', default_value='2.0', description='发布频率 Hz'),

        Node(
            package='cv_practice',
            executable='talker',
            name='talker',
            output='screen',
            parameters=[{
                'publish_rate': ParameterValue(rate, value_type=float),
                'message_prefix': ParameterValue(prefix, value_type=str),
            }],
            # 话题 remap：把 chatter 改到 demo/chatter
            remappings=[('chatter', 'demo/chatter')],
        ),

        Node(
            package='cv_practice',
            executable='listener',
            name='listener',
            output='screen',
            remappings=[('chatter', 'demo/chatter')],
        ),

        Node(
            package='cv_practice',
            executable='tf_broadcaster',
            name='tf_broadcaster',
            output='screen',
            parameters=[{'rate_deg': 30.0}],
        ),
    ])
