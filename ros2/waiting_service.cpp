
#include <rclcpp/node.hpp>
#include <rclcpp/executors.hpp>

#include <std_msgs/msg/int64.hpp>
#include <example_interfaces/srv/add_two_ints.hpp>

#include <functional>

class Listener : public rclcpp::Node
{
public:

  Listener()
    : rclcpp::Node("Listener"),
      _request(nullptr)

  {
    _subscription = create_subscription<std_msgs::msg::Int64>(
          "third_int",
          std::bind(&Listener::callback, this, std::placeholders::_1));
  }

  void callback(const std_msgs::msg::Int64::SharedPtr publication)
  {
    std::unique_lock<std::mutex> lock(_mutex);

    // If a request has not been made, then we'll just ignore the incoming message
    if(!_request)
      return;

    _request->set_value(publication->data);
    _request = nullptr;
  }

  std::future<int64_t> request_next_publication(
      std::promise<int64_t>* request)
  {
    std::unique_lock<std::mutex> lock(_mutex);
    _request = request;

    return _request->get_future();
  }

private:

  std::promise<int64_t>* _request;
  std::mutex _mutex;

  rclcpp::Subscription<std_msgs::msg::Int64>::SharedPtr _subscription;

};

using AddTwoInts = example_interfaces::srv::AddTwoInts;

class WaitingService : public rclcpp::Node
{
public:

  WaitingService(const std::shared_ptr<Listener>& listener)
    : rclcpp::Node("WaitingServiceNode"),
      _listener(listener)
  {
    _service = create_service<AddTwoInts>(
          "add_three_ints",
          [this](
            const std::shared_ptr<rmw_request_id_t> request_header,
            const std::shared_ptr<AddTwoInts::Request> request,
            const std::shared_ptr<AddTwoInts::Response> response)
          {
            this->wait_then_add(request_header, request, response);
          });
  }

  void wait_then_add(
      const std::shared_ptr<rmw_request_id_t> /*request_header*/,
      const std::shared_ptr<AddTwoInts::Request> request,
      const std::shared_ptr<AddTwoInts::Response> response)
  {
    std::promise<int64_t> promise;
    std::future<int64_t> future = _listener->request_next_publication(&promise);
    std::cout << "Requested next topic information" << std::endl;
    future.wait();
    std::cout << "Received next topic information" << std::endl;
    int64_t data = future.get();

    response->sum = request->a + request->b + data;
  }

private:

  std::shared_ptr<Listener> _listener;
  rclcpp::Service<AddTwoInts>::SharedPtr _service;

};

int main(int argc, char* argv[])
{
  rclcpp::init(argc, argv);

  auto listener = std::make_shared<Listener>();
  auto waiting_service = std::make_shared<WaitingService>(listener);

  const auto spinner = [](const rclcpp::Node::SharedPtr node)
  {
    rclcpp::spin(node);
  };

  auto listener_thread = std::thread(spinner, listener);
  auto waiter_thread = std::thread(spinner, waiting_service);

  listener_thread.join();
  waiter_thread.join();

  rclcpp::shutdown();
}
