#include <iostream>
#include "rclcpp/rclcpp.hpp"
#include "interfaces/msg/angular.hpp"
#include "std_msgs/msg/float64.hpp"

class SimActuator : public rclcpp::Node {
    public:
        SimActuator() : Node("sim_actuator") {
            _pan_publisher = this->create_publisher<std_msgs::msg::Float64>("/cmd_pitch", 10);
            _tilt_publisher = this->create_publisher<std_msgs::msg::Float64>("/cmd_yaw", 10);

            auto callback = [this](interfaces::msg::Angular::UniquePtr msg) -> void {
                RCLCPP_INFO(this->get_logger(), "got angular, sending...");

                std_msgs::msg::Float64 pan_msg;
                std_msgs::msg::Float64 tilt_msg;

                pan_msg.data = msg->r_pan;
                tilt_msg.data = msg->r_tilt;

                this->_pan_publisher->publish(pan_msg);
                this->_tilt_publisher->publish(tilt_msg);
            };

            _subscriber = this->create_subscription<interfaces::msg::Angular>("distance_topic", 10, callback);
        };
    private:
        rclcpp::Subscription<interfaces::msg::Angular>::SharedPtr _subscriber;
        rclcpp::Publisher<std_msgs::msg::Float64>::SharedPtr _pan_publisher;
        rclcpp::Publisher<std_msgs::msg::Float64>::SharedPtr _tilt_publisher;

};

int main(int argc, char* argv[]) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<SimActuator>());
    rclcpp::shutdown();
    return 0;
}