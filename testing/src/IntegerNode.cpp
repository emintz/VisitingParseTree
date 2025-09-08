/*
 * IntegerNode.cpp
 *
 *  Created on: Sep 3, 2025
 *      Author: Eric Mintz
 */

#include <IntegerNode.h>

namespace VisitingParseTree {

IntegerNodeSupplier IntegerNode::SUPPLIER;

TraversalStatus IntegerNode::accept(Visitor *visitor) {
  IntegerNodeVisitor *concrete_visitor = dynamic_cast<IntegerNodeVisitor *>(visitor);
  return concrete_visitor
      ? concrete_visitor->process_integer_node(this)
      : ValueNode::accept(visitor);
}

Supplier<BaseAttrNode>& IntegerNode::supplier() {
  return SUPPLIER;
}

IntegerNodeSupplier::IntegerNodeSupplier() :
  Supplier<BaseAttrNode>("IntegerNode") {
}

std::shared_ptr<BaseAttrNode> IntegerNodeSupplier::make_shared() {
  return std::static_pointer_cast<BaseAttrNode>(
      std::make_shared<IntegerNode>(BaseAttrNode::forbid_public_access::here));
}

} /* namespace VisitingParseTree */
