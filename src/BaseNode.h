/*
 * BaseNode.h
 *
 *  Created on: Aug 18, 2025
 *      Author: Eric Mintz
 */

#pragma once

#include <atomic>

namespace VisitingParseTree {

class BaseNode {

  static int next_id();
  const int node_id;

public:
  BaseNode(const BaseNode &other) = delete;
  BaseNode(BaseNode &&other) = delete;
  BaseNode& operator=(const BaseNode &other) = delete;
  BaseNode& operator=(BaseNode &&other) = delete;

  BaseNode();

  int id() const {
    return node_id;
  }
};

} /* namespace VisitingParseTree */
