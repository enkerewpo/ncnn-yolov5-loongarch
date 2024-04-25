#pragma once

#include <memory>
#include <string>

namespace iS2ROS {

class Device {
public:
  int device_id;
  int device_type;
  double power_consumption;
  Device(int _device_id, int _device_type, double _power_consumption = -1) {
    device_id = _device_id;
    device_type = _device_type;
    power_consumption = _power_consumption;
  }
};

class Capability {
public:
  std::string name;
  double access_score; // capability's score accessed by access module
  std::shared_ptr<Device> device;

  Capability(std::string _name, std::shared_ptr<Device> _device) {
    name = _name;
    device = _device;
  }
};
} // namespace iS2ROS