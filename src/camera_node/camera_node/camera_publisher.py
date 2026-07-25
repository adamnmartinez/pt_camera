import rclpy
from rclpy.node import Node
from interfaces.msg import Rectangle

import os
import cv2
import torch
import math

from camera_node.network import Net

from ament_index_python.packages import get_package_share_directory

class CameraPublisher(Node):
    def __init__(self):
        super().__init__('camera_publisher')
        self.net = Net()
        self.capture = cv2.VideoCapture(0)

        self.get_logger().info('initalized camera.')

        share_directory = get_package_share_directory("camera_node")
        model_path = share_directory + "/models/face_detection.pt"

        if os.path.isfile(model_path):
            self.net.load_state_dict(torch.load(model_path))
        else:
            self.get_logger().info("no model found")
            raise FileNotFoundError(model_path)

        self.get_logger().info('loaded model.')

        self.net.eval()

        self.publisher = self.create_publisher(Rectangle, 'cam_topic', 10)
        self.timer = self.create_timer(0.5, self.publish_callback)

    def publish_callback(self):
        ret, image = self.capture.read()

        if not ret:
            self.get_logger().info("no camera output, exiting.")
            exit()

        image = cv2.resize(image, (512, 512))

        image_tensor = self.process_image(image)

        with torch.no_grad():
            output = self.net.forward(image_tensor)
            label = torch.flatten(output)

            width = image.shape[1]
            height = image.shape[0]

            cx = math.floor(label[0] * width)
            cy = math.floor(label[1] * height)
            w = math.floor(label[2] * width)
            h = math.floor(label[3] * height)

            x1 = int(cx - w / 2)
            y1 = int(cy - h / 2)
            x2 = int(cx + w / 2)
            y2 = int(cy + h / 2)

            rect = Rectangle()
            rect.x1 = x1
            rect.y1 = y1
            rect.x2 = x2
            rect.y2 = y2

            self.get_logger().info(f"publishing rect to cam_topic: ({x1}, {y1}) ({x2}, {y2})")
            self.publisher.publish(rect)

    def process_image(self, image):
        image_tensor = torch.from_numpy(image).float().permute(2, 0, 1) / 255.0
        image_tensor = image_tensor.unsqueeze(0)

        return image_tensor

def main(args=None):
    rclpy.init(args=args)
    publisher = CameraPublisher()
    rclpy.spin(publisher)
    publisher.destroy_node()
    rclpy.shutdown()

if __name__ == "__main__":
    main()