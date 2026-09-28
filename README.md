A fully autonomous hands-free pan-tilt camera. Closed loop control is achieved through a neural network built with PyTorch, trained on a [facial detection dataset](https://www.kaggle.com/datasets/iamtushara/face-detection-dataset) (credit to Tushar Anand on Kaggle). The output of the network is passed on to a computation node which finds the
necessary servo rotations to align the center of the camera with the center of the network rectangle output. This communication is handled using the ROS2 framework.

The network was modelled off [AlexNet](https://en.wikipedia.org/wiki/AlexNet) due to its historical success in visual recognition.

The hardware includes to SG90 servos contained within a plastic bracket. A Raspberry Pi 5 hosts the node architecture and processes image data using the detection model, while an Arduino Nano microcontroller handles direct servo control via serial
communication from the Pi.

This repository is maintained for the purposes of documenting this project. 

This repository contains the ROS system of nodes, as well as an SDF file for the simulation of the build within Gazebo.

## Gallery
<img width="929" height="935" alt="Screenshot (49)" src="https://github.com/user-attachments/assets/b428d128-3c4b-4348-b553-d188b81c6a0e" />

Figure 1: Abstract mechanics simulation of how servos connect and move the camera within Gazebo. In green, the servos. In blue, the brackets connecting the servos and the ground. In pink, the camera.

https://github.com/user-attachments/assets/69b81096-a899-4383-82b9-50227c6882ed

Figure 2: A showcase of the simulated movement of the camera body in Gazebo. Keystrokes are used to actuate the pan and tilt servos in tandem.

https://github.com/user-attachments/assets/24095a64-7052-4e2c-84e3-526441ba85a8

Figure 3: Recording of the physical prototype following my face. 

