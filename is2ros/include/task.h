#pragma once

#include <memory>
#include <string>
#include <vector>

namespace iS2ROS {
class Task {
public:
  int task_id;
  std::string target_cap_name;
  double target_cap_threshold;
  Task(int _task_id, std::string _target_cap_name,
       double _target_cap_threshold) {
    task_id = _task_id;
    target_cap_name = _target_cap_name;
    target_cap_threshold = _target_cap_threshold;
  }
};

struct TaskResult {
  int task_id;
  std::string result;
  std::vector<std::string> img_paths; // corresponding to the device_ids
  std::vector<int> device_ids;
};

std::vector<std::shared_ptr<Task>> get_tasks();
void add_task(std::shared_ptr<Task> task);
TaskResult run(int task_id);

} // namespace iS2ROS