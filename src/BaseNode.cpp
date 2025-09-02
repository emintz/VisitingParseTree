/*
 * BaseNode.cpp
 *
 *  Created on: Aug 18, 2025
 *      Author: eric
 */

#include "BaseNode.h"

namespace VisitingParseTree {

int BaseNode::next_id() {
  static std::atomic_int id_source = 0;
  return id_source++;
}

BaseNode::BaseNode() :
    node_id(next_id()) {}

} /* namespace VisitingParseTree */
