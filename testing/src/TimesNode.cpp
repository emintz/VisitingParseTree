/*
 * TimesNode.cpp
 *
 *  Created on: Sep 4, 2025
 *      Author: Eric Mintz
 */

#include "TimesNode.h"

namespace VisitingParseTree {

TimesNodeSupplier TimesNode::SUPPLIER;

TimesNode::TimesNode(BaseAttrNode::forbid_public_access) {
}

TraversalStatus TimesNode::accept(Visitor *visitor) {
  TimesNodeVisitor *concrete_visitor = dynamic_cast<TimesNodeVisitor *>(visitor);
  return concrete_visitor
      ? concrete_visitor->process_times_node(this)
      : OperatorNode::accept(visitor);
}

Supplier<BaseAttrNode>& TimesNode::supplier() {
  return SUPPLIER;
}

TimesNodeSupplier::TimesNodeSupplier() :
  Supplier<BaseAttrNode>("TimesNode") {
}

std::shared_ptr<BaseAttrNode> TimesNodeSupplier::make_shared() {
  return std::static_pointer_cast<BaseAttrNode>(
      std::make_shared<TimesNode>(BaseAttrNode::forbid_public_access::here));
}

} /* namespace VisitingParseTree */
