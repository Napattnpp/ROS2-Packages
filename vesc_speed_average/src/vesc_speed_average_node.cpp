#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/float64.hpp>

#include <algorithm>
#include <chrono>
#include <deque>
#include <string>

using std::placeholders::_1;
using namespace std::chrono_literals;

class VescSpeedAverageNode : public rclcpp::Node
{
public:
  VescSpeedAverageNode()
  : Node("vesc_speed_average_node"), sum_(0.0), latest_average_(0.0), has_data_(false)
  {
    input_topic_ = this->declare_parameter<std::string>("input_topic", "/sensors/core.state.speed");
    output_topic_ = this->declare_parameter<std::string>("output_topic", "/vesc_avg");
    // window = the last N samples used to compute average (here N=10)
    window_size_ = this->declare_parameter<int>("window_size", 10);
    publish_rate_hz_ = this->declare_parameter<double>("publish_rate_hz", 50.0);
    qos_depth_ = this->declare_parameter<int>("qos_depth", 50);

    if (window_size_ < 1) {
      RCLCPP_WARN(this->get_logger(), "window_size < 1, forcing to 1");
      window_size_ = 1;
    }
    if (publish_rate_hz_ <= 0.0) {
      RCLCPP_WARN(this->get_logger(), "publish_rate_hz <= 0, forcing to 50.0");
      publish_rate_hz_ = 50.0;
    }
    if (qos_depth_ < 1) {
      RCLCPP_WARN(this->get_logger(), "qos_depth < 1, forcing to 1");
      qos_depth_ = 1;
    }

    auto qos = rclcpp::QoS(rclcpp::KeepLast(static_cast<size_t>(qos_depth_)));

    speed_sub_ = this->create_subscription<std_msgs::msg::Float64>(
      input_topic_, qos, std::bind(&VescSpeedAverageNode::speedCallback, this, _1));

    avg_pub_ = this->create_publisher<std_msgs::msg::Float64>(output_topic_, qos);

    const auto period = std::chrono::duration<double>(1.0 / publish_rate_hz_);
    publish_timer_ = this->create_wall_timer(
      std::chrono::duration_cast<std::chrono::nanoseconds>(period),
      std::bind(&VescSpeedAverageNode::publishAveragedSpeed, this));

    RCLCPP_INFO(
      this->get_logger(),
      "Filtering '%s' -> '%s' with window=%d at %.2f Hz",
      input_topic_.c_str(), output_topic_.c_str(), window_size_, publish_rate_hz_);
  }

private:
  void speedCallback(const std_msgs::msg::Float64::SharedPtr msg)
  {
    const double speed = msg->data;

    speed_window_.push_back(speed);
    sum_ += speed;

    if (static_cast<int>(speed_window_.size()) > window_size_) {
      sum_ -= speed_window_.front();
      speed_window_.pop_front();
    }

    latest_average_ = sum_ / static_cast<double>(speed_window_.size());
    has_data_ = true;
  }

  void publishAveragedSpeed()
  {
    if (!has_data_) {
      return;
    }

    std_msgs::msg::Float64 out;
    out.data = latest_average_;
    avg_pub_->publish(out);
  }

  std::string input_topic_;
  std::string output_topic_;
  int window_size_;
  double publish_rate_hz_;
  int qos_depth_;

  std::deque<double> speed_window_;
  double sum_;
  double latest_average_;
  bool has_data_;

  rclcpp::Subscription<std_msgs::msg::Float64>::SharedPtr speed_sub_;
  rclcpp::Publisher<std_msgs::msg::Float64>::SharedPtr avg_pub_;
  rclcpp::TimerBase::SharedPtr publish_timer_;
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<VescSpeedAverageNode>());
  rclcpp::shutdown();
  return 0;
}
