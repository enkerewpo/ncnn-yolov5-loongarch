#pragma once

#include "loguru.hpp"
#include <chrono>
#include <cmath>
#include <memory>
#include <mutex>
#include <thread>

namespace iS2ROS {
class EnvSimulator {
public:
  double temperature;
  double humidity;
  double visibility;
  double start_time; // current emulated time(seconds)

  std::mutex env_lock;

  std::thread t;
  double timer_end = 20; // total emulated time(seconds)

  void start() {
    start_time = std::chrono::duration_cast<std::chrono::seconds>(
                     std::chrono::system_clock::now().time_since_epoch())
                     .count();
    t = std::thread([this] {
      while (true) {
        auto time = get_timestamp();
        if (time > timer_end) {
          LOG_F(INFO, "EnvSimulator: timer ended");
          break;
        }
        temperature = 20 + 5 * sin(time / 10);
        humidity = 50 + 20 * sin(time / 20);
        visibility = 10 + 5 * sin(time / 30);
        LOG_F(INFO,
              "EnvSimulator: temperature: %f, humidity: %f, visibility: %f",
              temperature, humidity, visibility);
        env_lock.lock();
        this->temperature = temperature;
        this->humidity = humidity;
        this->visibility = visibility;
        env_lock.unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
      }
    });
  }

  double get_timestamp() {
    return std::chrono::duration_cast<std::chrono::seconds>(
               std::chrono::system_clock::now().time_since_epoch())
               .count() -
           start_time;
  }
};

void env_sim_init();
std::shared_ptr<EnvSimulator> get_env();

} // namespace iS2ROS