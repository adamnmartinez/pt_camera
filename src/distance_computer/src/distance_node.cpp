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
            _pan_kp = 1.0;
            _tilt_kp = 1.0;
            _image_height = 512;
            _image_width = 512;
            _fov = 90;
            _origin_x = _image_width / 2.0;
            _origin_y = _image_height / 2.0;
            _f_x = _origin_x / std::tan((_fov / 2.0) * M_PI / 180.0);
            _f_y = _origin_y / std::tan((_fov / 2.0) * M_PI / 180.0);

            auto callback = [this](interfaces::msg::Rectangle::UniquePtr msg) -> void {
              RCLCPP_INFO(this->get_logger(), "got rect from cam_topic: %d, %d, %d, %d", msg->x1, msg->y1, msg->x2, msg->y2);

              auto message = interfaces::msg::Angular();

              // Distance
              int target_center_x = msg->x1 + (int)((msg->x2 - msg->x1) / 2);
              int target_center_y = msg->y1 + (int)((msg->y2 - msg->y1) / 2);

              // Pan
              int p_x = _origin_x - target_center_x;
              double p_angle = std::atan2(p_x, _f_x) * 180.0 / M_PI;

              // Tilt
              int t_x = _origin_y - target_center_y;
              double t_angle = std::atan2(t_x, _f_y) * 180.0 / M_PI;

              message.r_pan = (int)std::round(p_angle * _pan_kp);
              message.r_tilt = (int)std::round(t_angle * _tilt_kp);

              RCLCPP_INFO(this->get_logger(), "publishing angular to distance_topic: <%d, %d>", message.r_pan, message.r_tilt);
              this->_publisher->publish(message);
            };

            _subscriber = this->create_subscription<interfaces::msg::Rectangle>("cam_topic", 10, callback);
        };

    private:
        rclcpp::Publisher<interfaces::msg::Angular>::SharedPtr _publisher;
        rclcpp::Subscription<interfaces::msg::Rectangle>::SharedPtr _subscriber;
        float _pan_kp;
        float _tilt_kp;
        float _image_height;
        float _image_width;
        float _f_x;
        float _f_y;
        float _fov;
        float _origin_x;
        float _origin_y;
};

int main (int argc, char * argv[]) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<DistanceCompute>());
    rclcpp::shutdown();
    return 0;
}
