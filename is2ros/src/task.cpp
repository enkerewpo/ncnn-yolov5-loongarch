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

  // 4. Act
  // this will trigger on/off of the corresponding capability/devices(just in
  // simulation)
  // and also return the image of the selected devices

  TaskResult result;
  result.task_id = task_id;
  result.result = "success";
  return result;
}

} // namespace iS2ROS