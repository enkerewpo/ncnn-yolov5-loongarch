#include "task.h"
#include "loguru.hpp"

namespace iS2ROS {

std::vector<std::shared_ptr<Task>> tasks;

std::vector<std::shared_ptr<Task>> get_tasks() { return tasks; }

void add_task(std::shared_ptr<Task> task) {
  LOG_F(INFO, "Task added: id = %d, target capability = %s, threshold = %f",
        task->task_id, task->target_cap_name.c_str(),
        task->target_cap_threshold);
  tasks.push_back(task);
}

} // namespace iS2ROS