#pragma once

#include <memory>
#include <string>
#include <vector>

namespace iS2ROS {

#define DEVICE_CAM_COLOR_TYPE1 1
#define DEVICE_CAM_COLOR_TYPE2 2

class Capability;

class Device {
public:
  int device_id;
  int device_type;
  double power_consumption;
  double res_x;
  double res_y;
  double temperture_when_last_assessed;

  std::vector<std::shared_ptr<Capability>> capabilities;

  Device(int _device_id, int _device_type, double _power_consumption = -1,
         double _res_x = -1, double _res_y = -1) {
    device_id = _device_id;
    device_type = _device_type;
    power_consumption = _power_consumption;
    res_x = _res_x;
    res_y = _res_y;
    temperture_when_last_assessed = -1;
  }
  void set_t(double temp) { temperture_when_last_assessed = temp; }
  auto get_t() { return temperture_when_last_assessed; }

  // add comparable < for device_id
  bool operator<(const Device &d) const { return device_id < d.device_id; }

  std::string get_device_type_str() {
    if (device_type == DEVICE_CAM_COLOR_TYPE1) {
      return "CAM_COLOR_TYPE1";
    } else if (device_type == DEVICE_CAM_COLOR_TYPE2) {
      return "CAM_COLOR_TYPE2";
    } else {
      return "UNKNOWN";
    }
  }

  std::string to_string() {
    return "Device ID: " + std::to_string(device_id) +
           ", Device Type: " + get_device_type_str() +
           ", Power Consumption: " + std::to_string(power_consumption) +
           ", Resolution: " + std::to_string(res_x) + "x" +
           std::to_string(res_y);
  }

  virtual double get_noise_level_by_temp(double temperture) = 0;
};

/// type 1 has a flat temp->noise level curve which means it is not affected by
/// temperature changes
class ColorCameraType1 : public Device {
public:
  ColorCameraType1(int _device_id, double _power_consumption, double _res_x,
                   double _res_y)
      : Device(_device_id, DEVICE_CAM_COLOR_TYPE1, _power_consumption, _res_x,
               _res_y) {}

  double get_noise_level_by_temp(double temperture) override {
    // a linear relation from [0, 50] to [1%, 10%]
    double k = (0.1 - 0.01) / 50;
    double b = 0.01;
    return k * temperture + b;
  }
};

/// type 2 has a curve temp->noise level curve which means it is affected by
/// temperature changes, as temperature increases, noise level increases
class ColorCameraType2 : public Device {
public:
  ColorCameraType2(int _device_id, double _power_consumption, double _res_x,
                   double _res_y)
      : Device(_device_id, DEVICE_CAM_COLOR_TYPE2, _power_consumption, _res_x,
               _res_y) {}

  double get_noise_level_by_temp(double temperture) override {
    // f(x) = 0.0005 (x >= 0, x <= 25)
    // f(x) = 0.16(x - 25) (x > 25, x <= 30)
    if (temperture <= 25) {
      return 0.0005 * temperture;
    } else if (temperture <= 50) {
      return 0.16 * (temperture - 25) + 0.0005 * 25;
    } else {
      return 1;
    }
  }
};

class KGraphNode;

class Capability : public std::enable_shared_from_this<Capability> {
public:
  std::string name;
  double access_score; // capability's score accessed by access module
  double threshold;    // capability's threshold accessed by select module
  std::shared_ptr<Device> device;

  Capability(std::string _name, std::shared_ptr<Device> _device) {
    name = _name;
    device = _device;
    access_score = -1;
  }
};
} // namespace iS2ROS