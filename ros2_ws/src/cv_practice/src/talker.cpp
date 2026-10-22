// talker：定时向 chatter 话题发布字符串；演示 参数声明/读取 + Publisher + 日志
#include <chrono>
#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

class Talker : public rclcpp::Node
{
public:
  Talker()
  : Node("talker"), count_(0)
  {
    // 三个参数：发布频率、消息前缀、最多发多少条（0 表示不限）
    publish_rate_ = this->declare_parameter<double>("publish_rate", 1.0);
    message_prefix_ = this->declare_parameter<std::string>("message_prefix", "hello");
    max_count_ = this->declare_parameter<int>("max_count", 0);

    publisher_ = this->create_publisher<std_msgs::msg::String>("chatter", 10);

    // 由参数算出周期，改参数就等于改频率，不用重新编译
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

    std_msgs::msg::String msg;
    msg.data = message_prefix_ + " " + std::to_string(count_);
    publisher_->publish(msg);
    RCLCPP_INFO(this->get_logger(), "发布: '%s'", msg.data.c_str());
    ++count_;
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
