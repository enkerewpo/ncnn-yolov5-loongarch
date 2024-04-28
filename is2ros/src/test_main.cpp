#include "capability.h"
#include "core.h"
#include "kgraph.h"
#include "loguru.hpp"
#include "simulate.h"
#include "task.h"

#include <chrono>
#include <map>

std::map<std::shared_ptr<iS2ROS::Device>,
         std::vector<std::shared_ptr<iS2ROS::Capability>>>
    device_to_caps;

std::map<std::shared_ptr<iS2ROS::Capability>,
         std::shared_ptr<iS2ROS::KGraphNode>>
    cap_to_node;

int main(int argc, char **argv) {
  // init log system
  loguru::g_stderr_verbosity = 1;
  // bool g_preamble_date    = true; // The date field
  // bool g_preamble_time    = true; // The time of the current day
  // bool g_preamble_uptime  = true; // The time since init call
  // bool g_preamble_thread  = true; // The logging thread
  // bool g_preamble_file    = true; // The file from which the log originates
  // from bool g_preamble_verbose = true; // The verbosity field bool
  // g_preamble_pipe    = true; // The pipe symbol right before the message
  loguru::g_preamble_date = false;
  loguru::g_preamble_time = false;
  loguru::g_preamble_file = false;
  loguru::init(argc, argv);
  loguru::add_file("is2ros.log", loguru::Append, loguru::Verbosity_MAX);

  LOG_F(INFO, "Welcome to iS2ROS framework(loongarch64) demo user program!");
  LOG_F(INFO, "Initializing iS2ROS framework...");

  iS2ROS::kgraph_init();
  iS2ROS::env_sim_init();

  LOG_F(INFO, "iS2ROS framework successfully initialized");

  LOG_F(INFO, "Adding Knowledge Graph nodes...");

  // first create two devices (1 UAV, 2 cams)

  std::shared_ptr<iS2ROS::Device> cam1 =
      std::make_shared<iS2ROS::ColorCameraType1>(1, 0.5, 640, 480);

  std::shared_ptr<iS2ROS::Device> cam2 =
      std::make_shared<iS2ROS::ColorCameraType2>(2, 0.5, 1920, 1080);

  /**
   * Image Cap
   *      -> Color Camera Image Cap -> Device 1
   *      -> Color Camera Image Cap -> Device 2
   */

  std::shared_ptr<iS2ROS::Capability> img_cap =
      std::make_shared<iS2ROS::Capability>("Image", nullptr);
  auto node1 = std::make_shared<iS2ROS::KGraphNode>(1, img_cap);
  iS2ROS::get_root_node()->add_child(node1);
  cap_to_node[img_cap] = node1;

  std::shared_ptr<iS2ROS::Capability> cap1 =
      std::make_shared<iS2ROS::Capability>(
          "Color Camera Image(low res, heat resistance)", cam1);

  auto node2 = std::make_shared<iS2ROS::KGraphNode>(2, cap1);
  iS2ROS::get_root_node()->children[0]->add_child(node2);
  cap_to_node[cap1] = node2;

  std::shared_ptr<iS2ROS::Capability> cap2 =
      std::make_shared<iS2ROS::Capability>(
          "Color Camera Image(high res, heat vulnerable)", cam2);
  auto node3 = std::make_shared<iS2ROS::KGraphNode>(3, cap2);
  iS2ROS::get_root_node()->children[0]->add_child(node3);
  cap_to_node[cap2] = node3;

  // update device to cap map
  device_to_caps[cam1].push_back(cap1);
  device_to_caps[cam2].push_back(cap2);

  iS2ROS::get_root_node()->print();

  LOG_F(INFO, "Finished adding Knowledge Graph nodes");

  while (1) {
    iS2ROS::add_task(std::make_shared<iS2ROS::Task>(1, "Image", 75.0));
    auto result = iS2ROS::run(1);
    if (result.result == "success") {
      LOG_F(INFO, "Task %d succeeded", result.task_id);
    }
    // sleep 2s
    std::this_thread::sleep_for(std::chrono::seconds(2));
  }

  return 0;
}