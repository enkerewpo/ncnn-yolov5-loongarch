#include "kgraph.h"

#include <functional>
#include <iostream>

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

std::shared_ptr<KGraphNode> get_node_by_cap_name(std::string cap_name) {
  std::shared_ptr<KGraphNode> node = nullptr;
  std::function<void(std::shared_ptr<KGraphNode>)> dfs =
      [&](std::shared_ptr<KGraphNode> n) {
        if (n->cap != nullptr && n->cap->name == cap_name) {
          node = n;
          return;
        }
        for (auto child : n->children) {
          dfs(child);
        }
      };
  dfs(root_node);
  return node;
}

bool is_ancestor(std::shared_ptr<KGraphNode> ancestor,
                 std::shared_ptr<KGraphNode> node) {
  std::function<bool(std::shared_ptr<KGraphNode>)> dfs =
      [&](std::shared_ptr<KGraphNode> n) {
        if (n == ancestor) {
          return true;
        }
        for (auto child : n->children) {
          if (dfs(child)) {
            return true;
          }
        }
        return false;
      };
  return dfs(node);
}

} // namespace iS2ROS
