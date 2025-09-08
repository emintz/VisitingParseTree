/*
 * DivNode.cpp
 *
 *  Created on: Sep 4, 2025
 *      Author: Eric Mintz
 */

#include "DivNode.h"

namespace VisitingParseTree {

DivNodeSupplier DivNode::SUPPLIER;

DivNode::DivNode(BaseAttrNode::forbid_public_access) {
}

TraversalStatus DivNode::accept(Visitor *visitor) {
  DivNodeVisitor *concrete_visitor = dynamic_cast<DivNodeVisitor *>(visitor);
  return concrete_visitor
      ? concrete_visitor->process_div_node(this)
      : OperatorNode::accept(visitor);
}

Supplier<BaseAttrNode>& DivNode::supplier() {
  return SUPPLIER;
}

DivNodeSupplier::DivNodeSupplier() :
    Supplier<BaseAttrNode>("DivNode") {}

std::shared_ptr<BaseAttrNode> DivNodeSupplier::make_shared() {
  return std::static_pointer_cast<BaseAttrNode>(
      std::make_shared<DivNode>(BaseAttrNode::forbid_public_access::here));
}

} /* namespace VisitingParseTree */
