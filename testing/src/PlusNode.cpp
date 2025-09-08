/*
 * PlusNode.cpp
 *
 *  Created on: Sep 2, 2025
 *      Author: Eric Mintz
 */

#include "PlusNode.h"

namespace VisitingParseTree {

PlusNodeSupplier PlusNode::SUPPLIER;

TraversalStatus PlusNode::accept(Visitor *visitor) {
  PlusNodeVisitor *concrete_visitor = dynamic_cast<PlusNodeVisitor *> (visitor);
  return concrete_visitor
      ? concrete_visitor->process_plus_node(this)
      : OperatorNode::accept(visitor);
}

Supplier<BaseAttrNode>& PlusNode::supplier() {
  return SUPPLIER;
}

PlusNodeSupplier::PlusNodeSupplier() :
    Supplier<BaseAttrNode>("PlusNode") {
}

std::shared_ptr<BaseAttrNode> PlusNodeSupplier::make_shared() {
  return std::static_pointer_cast<BaseAttrNode>(
      std::make_shared<PlusNode>(BaseAttrNode::forbid_public_access::here));
}

} /* namespace VisitingParseTree */
