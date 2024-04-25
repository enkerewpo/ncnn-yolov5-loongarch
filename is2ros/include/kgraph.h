#pragma once

#include <iostream>
#include <memory>
#include <vector>

namespace iS2ROS {
class Capability;

class KGraphNode {
public:
  int node_id;
  std::shared_ptr<iS2ROS::Capability> cap;
  std::shared_ptr<KGraphNode> parent;
  std::vector<std::shared_ptr<KGraphNode>> children;
};

void kgraph_init();
} // namespace iS2ROS