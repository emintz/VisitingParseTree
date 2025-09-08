/*
 * MinusNode.cpp
 *
 *  Created on: Sep 2, 2025
 *      Author: Eric Mintz
 */

#include "MinusNode.h"

namespace VisitingParseTree {

MinusNodeSupplier MinusNode::SUPPLIER;

TraversalStatus MinusNode::accept(Visitor *visitor) {
  MinusNodeVisitor *concrete_visitor = dynamic_cast<MinusNodeVisitor *>(visitor);
  return concrete_visitor
      ? concrete_visitor->process_minus_node(this)
      : OperatorNode::accept(visitor);
}

Supplier<BaseAttrNode>& MinusNode::supplier() {
  return SUPPLIER;
}

MinusNodeSupplier::MinusNodeSupplier() :
    Supplier<BaseAttrNode>("MinusNode") {
}

std::shared_ptr<BaseAttrNode> MinusNodeSupplier::make_shared() {
  return std::static_pointer_cast<BaseAttrNode>(
      std::make_shared<MinusNode>(BaseAttrNode::forbid_public_access::here));
}

} /* namespace VisitingParseTree */
