import os
from glob import glob
from setuptools import find_packages, setup

package_name = 'qubeDescription'

setup(
    name=package_name,
    version='0.0.0',
    packages=find_packages(exclude=['test']),
    #Decides where things are installed
    data_files=[
        (os.path.join('share', 'ament_index', 'resource_index', 'packages'),
            [os.path.join('resource', package_name)]),
        (os.path.join('share', package_name, 'config'), glob('config/*')),
        (os.path.join('share', package_name), ['package.xml']),
        (os.path.join('share', package_name), glob('urdf/*')),
        (os.path.join('share', package_name, 'launch'), glob('launch/*launch.py'))
    ],
    install_requires=['setuptools'],
    zip_safe=True,
    tests_require=['pytest'],
    entry_points={
        'console_scripts': [
        ],
    },
)
