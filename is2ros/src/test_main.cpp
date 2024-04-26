#include "capability.h"
#include "core.h"
#include "kgraph.h"
#include "simulate.h"
#include "task.h"

#include "loguru.hpp"

int main(int argc, char **argv) {
  // init log system
  loguru::g_stderr_verbosity = 1;
  loguru::init(argc, argv);
  loguru::add_file("is2ros.log", loguru::Append, loguru::Verbosity_MAX);

  LOG_F(INFO, "Welcome to iS2ROS framework(loongarch64) demo user program!");
  LOG_F(INFO, "Initializing iS2ROS framework...");

  iS2ROS::kgraph_init();
  iS2ROS::env_sim_init();

  LOG_F(INFO, "iS2ROS framework successfully initialized");

  LOG_F(INFO, "Adding Knowledge Graph nodes...");

  // first create two devices (1 UAV, 2 cams)

#define DEVICE_CAM_COLOR 1
#define DEVICE_CAM_IR 2

  std::shared_ptr<iS2ROS::Device> uav_dev_color_cam =
      std::make_shared<iS2ROS::Device>(1, DEVICE_CAM_COLOR, 10.0);
  std::shared_ptr<iS2ROS::Device> cam_dev_IR_cam =
      std::make_shared<iS2ROS::Device>(2, DEVICE_CAM_IR, 50.0);

  /**
   * Image Cap
   *      -> Color Camera Image Cap -> Device 1
   *      -> IR Camera Image Cap -> Device 2
   */

  std::shared_ptr<iS2ROS::Capability> img_cap =
      std::make_shared<iS2ROS::Capability>("Image", nullptr);
  iS2ROS::get_root_node()->add_child(
      std::make_shared<iS2ROS::KGraphNode>(1, img_cap));

  std::shared_ptr<iS2ROS::Capability> color_cam_img_cap =
      std::make_shared<iS2ROS::Capability>("Color Camera Image",
                                           uav_dev_color_cam);
  iS2ROS::get_root_node()->children[0]->add_child(
      std::make_shared<iS2ROS::KGraphNode>(2, color_cam_img_cap));

  std::shared_ptr<iS2ROS::Capability> IR_cam_img_cap =
      std::make_shared<iS2ROS::Capability>("IR Camera Image", cam_dev_IR_cam);
  iS2ROS::get_root_node()->children[0]->add_child(
      std::make_shared<iS2ROS::KGraphNode>(3, IR_cam_img_cap));

  iS2ROS::get_root_node()->print();

  LOG_F(INFO, "Finished adding Knowledge Graph nodes");

  while (1) {
    iS2ROS::add_task(std::make_shared<iS2ROS::Task>(1, "Image", 75.0));
    auto result = iS2ROS::run(1);
    if (result.result == "success") {
      LOG_F(INFO, "Task %d succeeded", result.task_id);
      break;
    }
  }

  return 0;
}