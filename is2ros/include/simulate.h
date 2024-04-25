#pragma once

#include "loguru.hpp"
#include <chrono>
#include <cmath>
#include <memory>
#include <thread>

namespace iS2ROS {
class EnvSimulator {
public:
  double temperature;
  double humidity;
  double visibility;
  double timestamp; // current emulated time in seconds

  std::thread t;
  double timer_end = 5;

  void start() {
    t = std::thread([this] {
      while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        timestamp += 1;
        if (timestamp > timer_end) {
          break;
        }
        temperature = 20 + 5 * sin(timestamp / 10);
        humidity = 50 + 10 * sin(timestamp / 20);
        visibility = 100 - 50 * sin(timestamp / 30);
        LOG_F(INFO,
              "EnvSimulator: temperature: %f, humidity: %f, visibility: %f",
              temperature, humidity, visibility);
      }
    });
  }
};

void env_sim_init();
std::shared_ptr<EnvSimulator> get_env();

} // namespace iS2ROS