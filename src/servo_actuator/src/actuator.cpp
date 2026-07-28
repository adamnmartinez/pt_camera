#include <boost/asio.hpp>
#include <iostream>
#include "rclcpp/rclcpp.hpp"
#include "interfaces/msg/angular.hpp"

using namespace std;
using namespace boost;

class ServoActuator : public rclcpp::Node {
    public:
        ServoActuator() : Node("servo_actuator") {
            asio::io_service io; 
            asio::serial_port serial(io);

            serial.open("/dev/ttyUSB0");
            serial.set_option(asio::serial_port_base::baud_rate(115200));

            auto callback = [this, &serial](interfaces::msg::Angular::UniquePtr msg) -> void {
              RCLCPP_INFO(this->get_logger(), "got angular from distance_topic: <%d, %d>", msg->r_pan, msg->r_tilt);
              std::stringstream ss;
              ss << "PAN " << msg->r_pan << "TILT " << msg->r_tilt;
              string message = ss.str();
              _write_to_serial(serial, message);
            };

            _subscriber = this->create_subscription<interfaces::msg::Angular>("cam_topic", 10, callback);
        };

    private:
        rclcpp::Subscription<interfaces::msg::Angular>::SharedPtr _subscriber;
        void _write_to_serial(asio::serial_port& serial, const string& message) {
            system::error_code ec;
            asio::write(serial, asio::buffer(message), ec);
            if (ec) {
                cerr << "Error writing to serial port: " << ec.message() << endl;
            }
        }
};

int main (int argc, char * argv[]) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<ServoActuator>());
    rclcpp::shutdown();
    return 0;
}