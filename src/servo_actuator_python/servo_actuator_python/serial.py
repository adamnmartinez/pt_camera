import serial
import rclpy
import math
from rclpy.node import Node
from interfaces.msg import Angular

class ServoActuator(Node):
	def __init__(self):
		super().__init__('servo_actuator')

		self._ser = serial.Serial('/dev/ttyUSB0', 115200, timeout=1)
		self._subscriber = self.create_subscription(Angular, "distance_topic", self._callback, 10)

	def _callback(self, msg):
		r_pan = msg.r_pan
		r_tilt = msg.r_tilt

		serial_message = f"{r_pan} {r_tilt}\n"
		self.get_logger().info(f"Sending this message over serial: {serial_message}")
		self._ser.write(serial_message.encode('ascii'))
		self._ser.flush()


def main(args=None):
	rclpy.init(args=args)
	actuator = ServoActuator()
	rclpy.spin(actuator)
	actuator.destroy_node()
	rclpy.shutdown()

if __name__ == "__main__":
	main()
