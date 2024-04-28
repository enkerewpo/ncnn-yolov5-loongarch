#pragma once

#include "capability.h"
#include "loguru.hpp"
#include <memory>
#include <vector>

namespace iS2ROS {

// select (capability_canuse, capability_need) -> vector of sensors

std::vector<std::shared_ptr<Device>>
select(std::vector<std::shared_ptr<Capability>> caps_need,
       std::vector<int> request_devices_count);

} // namespace iS2ROS