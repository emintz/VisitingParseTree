/*
 * RootNode.cpp
 *
 *  Created on: Sep 3, 2025
 *      Author: Eric Mintz
 */

#include <RootNode.h>

namespace VisitingParseTree {

RootNodeSupplier RootNode::SUPPLIER;

TraversalStatus RootNode::accept(Visitor *visitor) {
  RootNodeVisitor *concrete_visitor = dynamic_cast<RootNodeVisitor *>(visitor);
  return concrete_visitor
      ? concrete_visitor->process_root_node(this)
      : BaseAttrNode::accept(visitor);
}

Supplier<BaseAttrNode>& RootNode::supplier() {
  return SUPPLIER;
}

RootNodeSupplier::RootNodeSupplier() :
    Supplier<BaseAttrNode>("RootNode") {
}

std::shared_ptr<BaseAttrNode> RootNodeSupplier::make_shared() {
  return std::static_pointer_cast<BaseAttrNode>(
      std::make_shared<RootNode>(BaseAttrNode::forbid_public_access::here));
}

} /* namespace VisitingParseTree */
