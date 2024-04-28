#include "select.h"
#include "kgraph.h"

#include <map>
#include <set>
#include <vector>

extern std::map<std::shared_ptr<iS2ROS::Device>,
                std::vector<std::shared_ptr<iS2ROS::Capability>>>
    device_to_caps;

extern std::map<std::shared_ptr<iS2ROS::Capability>,
                std::shared_ptr<iS2ROS::KGraphNode>>
    cap_to_node;

namespace iS2ROS {

std::vector<std::shared_ptr<Device>>
select(std::vector<std::shared_ptr<Capability>> caps_need,
       std::vector<int> request_devices_count) {
  LOG_F(INFO, "[START MATCH]");

  // print caps_need and request_devices_count
  LOG_F(INFO, "Caps need and request devices count:");
  for (int i = 0; i < caps_need.size(); i++) {
    LOG_F(INFO, "Capability %s, request %d devices", caps_need[i]->name.c_str(),
          request_devices_count[i]);
  }

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

  // print all devices
  // LOG_F(INFO, "All devices:");
  // for (auto d : devices_all) {
  //   LOG_F(INFO, "Device %d", d->device_id);
  // }

  // cap1, need 1 device
  // cap2, need 2 devices

  // iterate all subset of devices
  // check whether these devices can cover all the capabilities

  // 1. get all subsets of devices
  auto devices_all_vec = std::vector<std::shared_ptr<Device>>(
      devices_all.begin(), devices_all.end());

  double best_cost = -1;

  int n = devices_all_vec.size();
  int m = request_devices_count.size();
  assert(m == caps_need.size()); // these two should be equal
  for (int i = 1; i < (1 << n); i++) {
    // LOG_F(INFO, "Checking subset %d", i);
    std::vector<std::shared_ptr<Device>> subset;
    for (int j = 0; j < n; j++) {
      if (i & (1 << j)) {
        subset.push_back(devices_all_vec[j]);
      }
    }
    // now we got the subset of devices
    // print it
    // LOG_F(INFO, "Subset of devices:");
    // for (auto d : subset) {
    //   LOG_F(INFO, "Device %d", d->device_id);
    // }
    // let's find out whether this subset can cover all the capabilities
    bool _continue = false;
    auto flags = std::vector<int>(m, 0);
    for (auto d : subset) {
      // get all capabilities of this device
      auto caps = device_to_caps[d];
      bool used = false;
      for (auto c : caps) {
        // LOG_F(INFO, "Checking capability %s on device %d", c->name.c_str(),
        //       d->device_id);
        auto node_c = cap_to_node[c];
        // LOG_F(INFO, "Access score: %f", c->access_score);
        // LOG_F(INFO, "Node id: %d", node_c->node_id);
        for (int k = 0; k < caps_need.size(); k++) {
          auto node_need = cap_to_node[caps_need[k]];
          if (is_ancestor(node_c, node_need)) {
            // check threshold
            // LOG_F(INFO, "Cap need is %s", caps_need[k]->name.c_str());
            // LOG_F(INFO, "Threshold: %f", caps_need[k]->threshold);
            auto cap = node_c->cap;
            if (cap->access_score < caps_need[k]->threshold) {
              continue;
            }
            flags[k] += 1; // this device can cover this capability
            used = true;
          }
        }
      }
      if (!used) {
        // this device cannot cover any capability
        // LOG_F(INFO, "Device %d cannot cover any capability", d->device_id);
        _continue = true;
        break;
      }
    }

    if (_continue) {
      continue;
    }

    // check whether all capabilities are covered and the number of devices is
    // matched
    bool all_covered = true;
    for (int k = 0; k < m; k++) {
      if (flags[k] != request_devices_count[k]) {
        all_covered = false;
        break;
      }
    }
    // if not all capabilities are covered, continue
    if (!all_covered) {
      continue;
    }
    // print the subset of devices
    LOG_F(INFO, "Found a subset of devices that can cover all capabilities:");
    for (auto d : subset) {
      LOG_F(INFO, "Device %d", d->device_id);
    }
    // calculate the cost of this subset by SIGMA all scores of the devices'
    // caps
    double cost = 0;
    for (auto d : subset) {
      auto caps = device_to_caps[d];
      for (auto c : caps) {
        cost += c->access_score;
      }
    }
    // update the best cost if cost > best_cost
    if (cost >= best_cost) {
      LOG_F(INFO, "Cost of this subset: %f, now this is the best subset", cost);
      best_cost = cost;
      devices = subset;
    }
  }

  LOG_F(INFO, "[END MATCH]");

  for (auto d : devices) {
    LOG_F(INFO, "Selected device %d", d->device_id);
  }

  return devices;
}

} // namespace iS2ROS