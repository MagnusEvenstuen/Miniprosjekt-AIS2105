from launch import LaunchDescription
from launch_ros.actions import Node
from ament_index_python.packages import get_package_share_directory
import os
import xacro

def generate_launch_description():
    packageName = 'qubeDescription'
    
    #Process URDF with xacro
    xacroPath = os.path.join(
        get_package_share_directory(packageName),
        'qube.urdf.xacro'
    )
    robotDescription = xacro.process_file(xacroPath).toxml()

    #RViz config path
    rvizConfig = os.path.join(
        get_package_share_directory(packageName),
        'config',
        'qubeConfig.rviz'
    )

    return LaunchDescription([
        #Publishes the robots transform tree
        Node(
            package='robot_state_publisher',
            executable='robot_state_publisher',
            name='robot_state_publisher',
            parameters=[{'robot_description': robotDescription}],
        ),
        #Publishes the joints
        Node(
            package='joint_state_publisher',
            executable='joint_state_publisher',
            name='joint_state_publisher',
        ),
        #Launches Rviz
        Node(
            package='rviz2',
            executable='rviz2',
            name='rviz2',
            arguments=['-d', rvizConfig],
        ),
    ])