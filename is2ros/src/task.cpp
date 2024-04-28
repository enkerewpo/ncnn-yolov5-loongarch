#include "task.h"
#include "assess.h"
#include "kgraph.h"
#include "loguru.hpp"
#include "select.h"
#include "simulate.h"

namespace iS2ROS {

std::vector<std::shared_ptr<Task>> tasks;

std::vector<std::shared_ptr<Task>> get_tasks() { return tasks; }

void add_task(std::shared_ptr<Task> task) {
  LOG_F(INFO, "Task added: id = %d, target capability = %s, threshold = %f",
        task->task_id, task->target_cap_name.c_str(),
        task->target_cap_threshold);
  tasks.push_back(task);
}

std::shared_ptr<Task> get_task_by_id(int task_id) {
  for (auto task : tasks) {
    if (task->task_id == task_id) {
      return task;
    }
  }
  return nullptr;
}

TaskResult run(int task_id) {

  // trigger a full OODA loop here

  // 1. Observe
  // the environment simulator is running in a separate thread
  // updated every 0.5 seconds
  // no need to do anything here

  // 2. Orient
  // sample the environment simulator to get the latest env data
  // use this data to update the knowledge graph

  auto env = get_env();
  // iterate all nodes on the knowledge graph
  auto vec = get_root_node()->get_all_atomic_cap_nodes();
  assess(vec, env);

  // 3. Decide
  // find the best match for this task added by user

  auto task = get_task_by_id(task_id);
  if (task == nullptr) {
    LOG_F(ERROR, "Task %d not found", task_id);
    TaskResult result;
    result.task_id = task_id;
    result.result = "failed";
    return result;
  }
  // find the cap node according to the task's target capability
  auto cap_node = get_node_by_cap_name(task->target_cap_name);
  if (cap_node == nullptr) {
    LOG_F(ERROR, "Capability %s not found in the knowledge graph",
          task->target_cap_name.c_str());
    TaskResult result;
    result.task_id = task_id;
    result.result = "failed";
    return result;
  }
  // set the threshold
  cap_node->cap->threshold = task->target_cap_threshold;
  auto caps_need = std::vector<std::shared_ptr<Capability>>{cap_node->cap};
  auto request_devices_count = std::vector<int>{1};
  auto devices = select(caps_need, request_devices_count);

  // 4. Act
  // this will trigger on/off of the corresponding capability/devices(just in
  // simulation)
  // and also return the image of the selected devices

  TaskResult result;
  result.task_id = task_id;
  result.result = "success";
  for (auto device : devices) {
    result.device_ids.push_back(device->device_id);
    result.img_paths.push_back("/home/wheatfox/code/ncnn-yolov5-"
                               "loongarch/images/0f61069510.jpg");
  }
  return result;
}

} // namespace iS2ROS