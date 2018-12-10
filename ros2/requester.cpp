
#include <rclcpp/node.hpp>
#include <rclcpp/executors.hpp>
#include <example_interfaces/srv/add_two_ints.hpp>

#include <iostream>

using AddTwoInts = example_interfaces::srv::AddTwoInts;

int main(int argc, char* argv[])
{
  rclcpp::init(argc, argv);

  auto node = std::make_shared<rclcpp::Node>("requester");
  auto client = node->create_client<AddTwoInts>("add_three_ints");

  auto request = std::make_shared<AddTwoInts::Request>();
  request->a = 1;
  request->b = 2;

  auto future_response = client->async_send_request(request);

  if(rclcpp::spin_until_future_complete(node, future_response)
     != rclcpp::executor::FutureReturnCode::SUCCESS)
  {
    std::cerr << "Failed to get a service response!" << std::endl;
    return 1;
  }

  std::cout << "Got the service response: "
            << future_response.get()->sum << std::endl;

  rclcpp::shutdown();
}
