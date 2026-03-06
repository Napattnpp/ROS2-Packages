# vesc_speed_average

ROS 2 package to smooth VESC speed values with a rolling average and republish at a fixed rate.

## Topics

- Subscribe: `/sensors/core.state.speed` (`std_msgs/msg/Float64`)
- Publish: `/vesc_to_odom` (`std_msgs/msg/Float64`)

## Parameters

- `input_topic` (string, default: `/sensors/core.state.speed`)
- `output_topic` (string, default: `/vesc_to_odom`)
- `window_size` (int, default: `10`)
- `publish_rate_hz` (double, default: `50.0`)
- `qos_depth` (int, default: `50`)

## Build

```bash
colcon build --packages-select vesc_speed_average
```

## Run

```bash
ros2 run vesc_speed_average vesc_speed_average_node
```

or

```bash
ros2 launch vesc_speed_average vesc_speed_average.launch.py
```
