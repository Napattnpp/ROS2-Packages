# vesc_speed_average

ROS 2 package to smooth VESC `state.speed` with a rolling average and republish as `vesc_msgs/msg/VescStateStamped`.

## Topics

- Subscribe: `/sensors/core` (`vesc_msgs/msg/VescStateStamped`)
- Publish: `/vesc_average` (`vesc_msgs/msg/VescStateStamped`)

## Parameters

- `input_topic` (string, default: `/sensors/core`)
- `output_topic` (string, default: `/vesc_average`)
- `window_size` (int, default: `10`)
- `publish_rate_hz` (double, default: `50.0`)
- `qos_depth` (int, default: `50`)

## Build

```bash
colcon build --packages-select vesc_speed_average
```

## Run

```bash
ros2 launch vesc_speed_average vesc_speed_average.launch.py
```

## Use in vesc_to_odom

Subscribe to `/vesc_average` with `vesc_msgs::msg::VescStateStamped` and reuse your existing callback.
