#include <memory>
#include <cmath>
#include "rclcpp/rclcpp.hpp"
#include "interfaces/msg/rectangle.hpp"
#include "interfaces/msg/vector.hpp"
#include "interfaces/msg/angular.hpp"

class DistanceCompute : public rclcpp::Node {
    public:
        DistanceCompute() : Node("distance_compute") {
            _publisher = this->create_publisher<interfaces::msg::Angular>("distance_topic", 10);

            auto callback = [this](interfaces::msg::Rectangle::UniquePtr msg) -> void {
              RCLCPP_INFO(this->get_logger(), "got rect from cam_topic: %d, %d, %d, %d", msg->x1, msg->y1, msg->x2, msg->y2);
            
              auto message = interfaces::msg::Angular();

              int origin_x = 256;
              int origin_y = 256;

              // Distance
              int target_center_x = msg->x1 + (int)((msg->x2 - msg->x1) / 2);
              int target_center_y = msg->y1 + (int)((msg->y2 - msg->y1) / 2);

              int c_radius = std::abs(origin_x - target_center_x) + std::abs(origin_y - target_center_y);

              // Pan
              int p_x = origin_x - target_center_x;
              double p_angle = std::atan2(p_x, c_radius) * 180.0 / M_PI;

              // Tilt
              int t_x = origin_y - target_center_y;
              double t_angle = std::atan2(t_x, c_radius) * 180.0 / M_PI;

              message.r_pan = (int)std::round(p_angle);
              message.r_tilt = (int)std::round(t_angle);

                //   if ((target_center_x <= origin_x) && (target_center_y <= origin_y)) {
                //     message.quadrant = 1;
                //   } else if ((target_center_x > origin_x) && (target_center_y <= origin_y)) {
                //     message.quadrant = 2;
                //   } else if ((target_center_x <= origin_x) && (target_center_y > origin_y)) {
                //     message.quadrant = 3;
                //   } else {
                //     message.quadrant = 4;
                //   } 

              RCLCPP_INFO(this->get_logger(), "publishing angular to distance_topic: <%d, %d>", message.r_pan, message.r_tilt);
              this->_publisher->publish(message);
            };

            _subscriber = this->create_subscription<interfaces::msg::Rectangle>("cam_topic", 10, callback);
        };

    private:
        rclcpp::Publisher<interfaces::msg::Angular>::SharedPtr _publisher;
        rclcpp::Subscription<interfaces::msg::Rectangle>::SharedPtr _subscriber;
};

int main (int argc, char * argv[]) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<DistanceCompute>());
    rclcpp::shutdown();
    return 0;
}
