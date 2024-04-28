#pragma once

#include "loguru.hpp"
#include <chrono>
#include <cmath>
#include <cstdio>
#include <iostream>
#include <memory>
#include <mutex>
#include <string>
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
  double timer_end = 300; // total emulated time(seconds)
  double INTERVAL = 2;

  // update env every INTERVAL seconds
  // from 8AM in the morning to 6PM in the even ing
  // push forward 30mins every INTERVAL seconds
  // temperature from 10 to 30, to 15

  double emulated_hour; // as 24-hour format

  void start() {
    start_time = std::chrono::duration_cast<std::chrono::seconds>(
                     std::chrono::system_clock::now().time_since_epoch())
                     .count();
    emulated_hour = 8;
    t = std::thread([this] {
      while (true) {
        auto time = get_timestamp();
        if (time > timer_end) {
          LOG_F(INFO, "EnvSimulator: timer ended");
          break;
        }
        if (emulated_hour >= 18) {
          emulated_hour = 8;
        }
        temperature = calc_temperature(emulated_hour);
        humidity = 50 + 20 * sin(time / 20);
        visibility = 10 + 5 * sin(time / 30);
        LOG_F(INFO,
              "EnvSimulator: temperature: %f, "
              "current_emulated_time= %s",
              temperature, get_datetime().c_str());
        env_lock.lock();
        this->temperature = temperature;
        this->humidity = humidity;
        this->visibility = visibility;
        env_lock.unlock();
        emulated_hour += 0.5;
        std::this_thread::sleep_for(
            std::chrono::milliseconds((int)(INTERVAL * 1000)));
      }
    });
  }

  std::string get_datetime() {
    int hour = (int)emulated_hour;
    int minute = (int)((emulated_hour - hour) * 60);
    char buffer[100];
    sprintf(buffer, "%02d:%02d", hour, minute);
    return std::string(buffer);
  }

  double calc_temperature(double hour) {
    double temp = 0;
    if (hour < 12) {
      double k = 20.0 / 4;
      temp = 10 + k * (hour - 8);
    } else {
      double k = 20.0 / 6;
      temp = 30 - k * (hour - 12);
    }
    temp += 2 * sin(hour / 2);
    return temp;
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