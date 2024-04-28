#pragma once

#include "capability.h"
#include "loguru.hpp"
#include <memory>
#include <vector>

namespace iS2ROS {

// select (capability_canuse, capability_need) -> vector of sensors

std::vector<std::shared_ptr<Device>>
match(std::vector<std::shared_ptr<Capability>> caps_need);

} // namespace iS2ROS