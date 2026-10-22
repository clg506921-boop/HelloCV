// 服务端：提供 add_two_ints，演示 rclcpp::Service
#include <memory>

#include "example_interfaces/srv/add_two_ints.hpp"
#include "rclcpp/rclcpp.hpp"

class AddTwoIntsServer : public rclcpp::Node
{
public:
  AddTwoIntsServer()
  : Node("add_two_ints_server")
  {
    service_ = this->create_service<example_interfaces::srv::AddTwoInts>(
      "add_two_ints",
      std::bind(&AddTwoIntsServer::handle_add, this,
        std::placeholders::_1, std::placeholders::_2));
    RCLCPP_INFO(this->get_logger(), "服务 add_two_ints 已就绪");
  }

private:
  void handle_add(
    const std::shared_ptr<example_interfaces::srv::AddTwoInts::Request> request,
    std::shared_ptr<example_interfaces::srv::AddTwoInts::Response> response)
  {
    response->sum = request->a + request->b;
    RCLCPP_INFO(this->get_logger(), "收到请求: %ld + %ld = %ld",
      request->a, request->b, response->sum);
  }

  rclcpp::Service<example_interfaces::srv::AddTwoInts>::SharedPtr service_;
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<AddTwoIntsServer>());
  rclcpp::shutdown();
  return 0;
}
