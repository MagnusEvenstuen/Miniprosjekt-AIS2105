import os
from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.actions import IncludeLaunchDescription, ExecuteProcess
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.substitutions import Command, PathJoinSubstitution
from launch_ros.actions import Node

def generate_launch_description():
    #Finds location of files
    qubeBringupDir = get_package_share_directory('qubeBringup')
    qubeDriverDir = get_package_share_directory('qube_driver')
    
    #Process URDF with xacro
    urdfFile = os.path.join(qubeBringupDir, 'urdf', 'controlledQube.urdf.xacro')
    robotDescription = Command(['xacro ', urdfFile])
    
    return LaunchDescription([
        #Include Qube driver launch
        IncludeLaunchDescription(
            PythonLaunchDescriptionSource(
                os.path.join(qubeDriverDir, 'launch', 'qube_driver.launch.py')
            )
        ),
        
        #Publishes the robots transform tree
        Node(
            package='robot_state_publisher',
            executable='robot_state_publisher',
            name='robot_state_publisher',
            parameters=[{'robot_description': robotDescription}]
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
            arguments=['-d', os.path.join(qubeBringupDir, 'config', 'qube.rviz')]
        ),
        ExecuteProcess(
            cmd=['ros2', 'control', 'load_controller', 'joint_state_broadcaster'],
        ),
    ])