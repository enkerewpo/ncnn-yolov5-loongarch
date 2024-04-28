#pragma once

#include "capability.h"
#include "simulate.h"

#include <memory>
#include <vector>

namespace iS2ROS {

void assess(std::vector<std::shared_ptr<Capability>> capabilities,
            std::shared_ptr<EnvSimulator> env);

} // namespace iS2ROS