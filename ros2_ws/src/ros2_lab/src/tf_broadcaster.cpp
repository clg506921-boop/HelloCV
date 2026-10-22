#include <chrono>
#include <cmath>
#include <memory>
#include <string>

#include "geometry_msgs/msg/transform_stamped.hpp"
#include "rclcpp/rclcpp.hpp"
#include "tf2/LinearMath/Quaternion.h"
#include "tf2_ros/transform_broadcaster.h"

using namespace std::chrono_literals;

class TfBroadcaster : public rclcpp::Node
{
public:
  TfBroadcaster() : Node("tf_broadcaster"), yaw_(0.0)
  {
    // 两个参数：子坐标系名字、绕 z 轴转多快（度/秒）
    child_frame_ = this->declare_parameter<std::string>("child_frame", "laser");
    rate_deg_    = this->declare_parameter<double>("rate_deg", 45.0);

    broadcaster_ = std::make_unique<tf2_ros::TransformBroadcaster>(*this);
    timer_ = this->create_wall_timer(100ms, std::bind(&TfBroadcaster::on_timer, this));

    RCLCPP_INFO(this->get_logger(), "tf 广播启动: base_link -> %s, rate_deg=%.1f",
      child_frame_.c_str(), rate_deg_);
  }

private:
  void on_timer()
  {
    geometry_msgs::msg::TransformStamped t;
    t.header.stamp = this->get_clock()->now();
    t.header.frame_id = "base_link";      // 父坐标系
    t.child_frame_id  = child_frame_;     // 子坐标系

    // 雷达装在车体前方 20cm、高 10cm
    t.transform.translation.x = 0.20;
    t.transform.translation.y = 0.0;
    t.transform.translation.z = 0.10;

    // 每 100ms 转一点，让 rviz 里能看到它在动
    const double kPi = 3.14159265358979323846;
    yaw_ += rate_deg_ * 0.1 * kPi / 180.0;

    tf2::Quaternion q;
    q.setRPY(0.0, 0.0, yaw_);             // 只绕 z 轴
    t.transform.rotation.x = q.x();
    t.transform.rotation.y = q.y();
    t.transform.rotation.z = q.z();
    t.transform.rotation.w = q.w();

    broadcaster_->sendTransform(t);
  }

  std::unique_ptr<tf2_ros::TransformBroadcaster> broadcaster_;
  rclcpp::TimerBase::SharedPtr timer_;
  std::string child_frame_;
  double rate_deg_;
  double yaw_;
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<TfBroadcaster>());
  rclcpp::shutdown();
  return 0;
}
