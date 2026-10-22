// listener：订阅 chatter；演示 Subscription + QoS（keep_last(10) + reliable）
#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

class Listener : public rclcpp::Node
{
public:
  Listener()
  : Node("listener")
  {
    // QoS：保留最近 10 条、可靠传输，与发布端的深度对应
    rclcpp::QoS qos(rclcpp::KeepLast(10));
    qos.reliable();

    subscription_ = this->create_subscription<std_msgs::msg::String>(
      "chatter", qos,
      [this](const std_msgs::msg::String::SharedPtr msg) {
        RCLCPP_INFO(this->get_logger(), "收到: '%s'", msg->data.c_str());
      });

    RCLCPP_INFO(this->get_logger(), "listener 已订阅 chatter (QoS: keep_last=10, reliable)");
  }

private:
  rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscription_;
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<Listener>());
  rclcpp::shutdown();
  return 0;
}
