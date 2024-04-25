#include "kgraph.h"

namespace iS2ROS {

std::shared_ptr<KGraphNode> root_node;

void kgraph_init() {
  root_node = std::make_shared<KGraphNode>();
  root_node->node_id = 0;
  root_node->cap = nullptr;
  root_node->parent = nullptr;
  root_node->children.clear();
  // std::cout << "KGraph initialized, root node created" << std::endl;
  LOG_F(INFO, "KGraph initialized, root node created");
}

std::shared_ptr<KGraphNode> get_root_node() { return root_node; }

}