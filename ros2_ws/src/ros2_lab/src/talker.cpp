#include <chrono>
#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

class Talker : public rclcpp::Node
{
public:
  Talker() : Node("talker"), count_(0)
  {
    // 声明三个参数（名字, 默认值）
    publish_rate_   = this->declare_parameter<double>("publish_rate", 1.0);
    message_prefix_ = this->declare_parameter<std::string>("message_prefix", "hello");
    max_count_      = this->declare_parameter<int>("max_count", 0);

    publisher_ = this->create_publisher<std_msgs::msg::String>("chatter", 10);

    // 周期只在构造时算一次
    auto period = std::chrono::duration<double>(1.0 / publish_rate_);
    timer_ = this->create_wall_timer(
      std::chrono::duration_cast<std::chrono::nanoseconds>(period),
      std::bind(&Talker::on_timer, this));

    RCLCPP_INFO(this->get_logger(),
      "talker 启动: publish_rate=%.2f Hz, message_prefix=%s, max_count=%d",
      publish_rate_, message_prefix_.c_str(), max_count_);
  }

private:
  void on_timer()
  {
    if (max_count_ > 0 && static_cast<int>(count_) >= max_count_) {
      RCLCPP_INFO(this->get_logger(), "已发满 max_count=%d 条，停止发布", max_count_);
      timer_->cancel();
      return;
    }

    // 每条消息都现读一次前缀
    const std::string prefix = this->get_parameter("message_prefix").as_string();

    std_msgs::msg::String msg;
    msg.data = prefix + " " + std::to_string(count_++);
    publisher_->publish(msg);
    RCLCPP_INFO(this->get_logger(), "发布: '%s'", msg.data.c_str());
  }

  rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
  rclcpp::TimerBase::SharedPtr timer_;
  double publish_rate_;
  std::string message_prefix_;
  int max_count_;
  size_t count_;
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<Talker>());
  rclcpp::shutdown();
  return 0;
}