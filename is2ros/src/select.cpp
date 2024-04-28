#include "select.h"
#include "kgraph.h"

#include <set>

namespace iS2ROS {

std::vector<std::shared_ptr<Device>>
match(std::vector<std::shared_ptr<Capability>> caps_need,
      std::vector<int> request_devices_count) {
  LOG_F(INFO, "[START MATCH]");
  std::vector<std::shared_ptr<Device>> devices;
  std::set<std::shared_ptr<Device>> devices_all;
  for (auto cap : caps_need) {
    auto node = get_node_by_cap_name(cap->name);
    if (node == nullptr) {
      LOG_F(ERROR, "Capability %s not found in the knowledge graph",
            cap->name.c_str());
      continue;
    }
    auto atomic_caps = node->get_all_atomic_cap_nodes();
    for (auto atomic_cap : atomic_caps) {
      devices_all.insert(atomic_cap->device);
    }
  }

  // cap1, need 1 device
  // cap2, need 2 devices

  // iterate all subset of devices
  // check whether these devices can cover all the capabilities

  // 1. get all subsets of devices
  auto devices_all_vec = std::vector<std::shared_ptr<Device>>(
      devices_all.begin(), devices_all.end());
  int n = devices_all_vec.size();
  int m = request_devices_count.size();
  assert(m == caps_need.size()); // these two should be equal
  for (int i = 0; i < (1 << n); i++) {
    std::vector<std::shared_ptr<Device>> subset;
    for (int j = 0; j < n; j++) {
      if (i & (1 << j)) {
        subset.push_back(devices_all_vec[j]);
      }
    }
    
  }

  return devices;
}

} // namespace iS2ROS