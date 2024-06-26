#pragma once

#include <cassert>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#define LOGURU_WITH_STREAMS 1
#include <loguru.hpp>

#include "capability.h"

namespace iS2ROS {

class KGraphNode : public std::enable_shared_from_this<KGraphNode> {
public:
  int node_id;
  std::shared_ptr<Capability> cap;
  std::shared_ptr<KGraphNode> parent;
  std::vector<std::shared_ptr<KGraphNode>> children;

  KGraphNode() {
    node_id = -1;
    cap = nullptr;
    parent = nullptr;
    children.clear();
  }

  KGraphNode(int id, std::shared_ptr<iS2ROS::Capability> c) {
    node_id = id;
    cap = c;
    parent = nullptr;
    children.clear();
  }

  void add_child(std::shared_ptr<KGraphNode> child) {
    assert(child->children.size() == 0); // only leaf nodes can be added
    children.push_back(child);
    child->parent = shared_from_this();
    LOG_F(INFO, "Added child node %d (cap name = %s) to parent node %d",
          child->node_id, child->cap->name.c_str(), node_id);
  }

  // pretty print with table ascii pretty print
  void pretty_print(int depth) {
    std::string s = "";
    // ascii 192 is └
    for (int i = 0; i < depth; i++) {
      s += "  ";
    }
    if (depth > 0) {
      s += "└─";
    }
    s += "Node ID: " + std::to_string(node_id);
    if (cap != nullptr) {
      s += " (Cap name: " + cap->name + ")";
    } else {
      s += " (Root node)";
    }
    LOG_F(INFO, "%s", s.c_str());
    for (auto child : children) {
      child->pretty_print(depth + 1);
    }
  }

  std::vector<std::shared_ptr<Capability>> get_all_atomic_cap_nodes() {
    // only return leaf nodes
    std::vector<std::shared_ptr<Capability>> vec;
    if (children.size() == 0) {
      vec.push_back(cap);
    } else {
      for (auto child : children) {
        auto child_vec = child->get_all_atomic_cap_nodes();
        vec.insert(vec.end(), child_vec.begin(), child_vec.end());
      }
    }
    return vec;
  }

  void print() { pretty_print(0); }
};

void kgraph_init();
std::shared_ptr<KGraphNode> get_root_node();
std::shared_ptr<KGraphNode> get_node_by_cap_name(std::string cap_name);
bool is_ancestor(std::shared_ptr<KGraphNode> ancestor,
                 std::shared_ptr<KGraphNode> node);
} // namespace iS2ROS