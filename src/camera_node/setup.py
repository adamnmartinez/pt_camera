from setuptools import find_packages, setup

package_name = 'camera_node'

setup(
    name=package_name,
    version='0.0.0',
    packages=find_packages(exclude=['test']),
    data_files=[
        ('share/ament_index/resource_index/packages',
            ['resource/' + package_name]),
        ('share/' + package_name, ['package.xml']),
        ('share/' + package_name + "/models/", ["camera_node/models/face_detection.pt"]),
    ],
    install_requires=['setuptools'],
    zip_safe=True,
    maintainer='adam',
    maintainer_email='adam.nmartinez@gmail.com',
    description='TODO: Package description',
    license='Apache-2.0',
    extras_require={
        'test': [
            'pytest',
        ],
    },
    entry_points={
        'console_scripts': [
            'pi_publisher = camera_node.picam_publisher:main',
            'pi_visual = camera_node.picam_visual:main',
            'opencv_publisher = camera_node.opencv_publisher:main'
        ],
    },
)
