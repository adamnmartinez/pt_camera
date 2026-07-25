#include <memory>
#include "rclcpp/rclcpp.hpp"
#include "interfaces/msg/rectangle.hpp"
#include "interfaces/msg/vector.hpp"

class DistanceCompute : public rclcpp::Node {
    public:
        DistanceCompute() : Node("distance_compute") {
            _publisher = this->create_publisher<interfaces::msg::Vector>("distance_topic", 10);

            auto callback = [this](interfaces::msg::Rectangle::UniquePtr msg) -> void {
              RCLCPP_INFO(this->get_logger(), "got rect from cam_topic: %d, %d, %d, %d", msg->x1, msg->y1, msg->x2, msg->y2);
            
              auto message = interfaces::msg::Vector();

              int target_x = msg->x1 + (int)((msg->x2 - msg->x1) / 2);
              int target_y = msg->y1 + (int)((msg->y2 - msg->y1) / 2);
              int origin_x = 256;
              int origin_y = 256;

              message.x = origin_x - target_x;
              message.y = origin_y - target_y;
              
              RCLCPP_INFO(this->get_logger(), "publishing vector to distance_topic: <%d, %d>", message.x, message.y);
              this->_publisher->publish(message);
            };

            _subscriber = this->create_subscription<interfaces::msg::Rectangle>("cam_topic", 10, callback);
        };

    private:
        rclcpp::Publisher<interfaces::msg::Vector>::SharedPtr _publisher;
        rclcpp::Subscription<interfaces::msg::Rectangle>::SharedPtr _subscriber;
};

int main (int argc, char * argv[]) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<DistanceCompute>());
    rclcpp::shutdown();
    return 0;
}
