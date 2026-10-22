#include <chrono>
#include <cstdlib>
#include <memory>

#include "example_interfaces/srv/add_two_ints.hpp"
#include "rclcpp/rclcpp.hpp"

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  auto node = rclcpp::Node::make_shared("add_two_ints_client");

  auto client = node->create_client<example_interfaces::srv::AddTwoInts>("add_two_ints");

  // 先等服务端上线，避免"没人在听"就发请求
  if (!client->wait_for_service(std::chrono::seconds(5))) {
    RCLCPP_ERROR(node->get_logger(), "服务 add_two_ints 在 5 秒内不可用");
    rclcpp::shutdown();
    return 1;
  }

  auto request = std::make_shared<example_interfaces::srv::AddTwoInts::Request>();
  request->a = (argc > 1) ? std::atol(argv[1]) : 3;
  request->b = (argc > 2) ? std::atol(argv[2]) : 4;

  auto future = client->async_send_request(request);

  // 等结果
  auto ret = rclcpp::spin_until_future_complete(node, future, std::chrono::seconds(5));
  if (ret != rclcpp::FutureReturnCode::SUCCESS) {
    RCLCPP_ERROR(node->get_logger(), "等待响应失败或超时");
    rclcpp::shutdown();
    return 1;
  }

  auto response = future.get();
  RCLCPP_INFO(node->get_logger(), "结果: %ld + %ld = %ld",
    request->a, request->b, response->sum);

  rclcpp::shutdown();
  return 0;
}