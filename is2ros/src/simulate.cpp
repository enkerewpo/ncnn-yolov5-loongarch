#include "simulate.h"

namespace iS2ROS {

std::shared_ptr<EnvSimulator> env;

void env_sim_init() {
  env = std::make_shared<EnvSimulator>();
  // create a new thread which update the env every 1s
  env->start();
  LOG_F(INFO, "Environment simulator initialized");
}

std::shared_ptr<EnvSimulator> get_env() { return env; }

} // namespace iS2ROS