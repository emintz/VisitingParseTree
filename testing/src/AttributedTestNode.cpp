/*
 * AttributedTestNode.cpp
 *
 *  Created on: Jul 14, 2025
 *      Author: Eric Mintz
 */

#include "AttributedTestNode.h"

AttributedTestNodeSupplier::AttributedTestNodeSupplier(void) :
    Supplier<VisitingParseTree::BaseAttrNode>("AttributedTestNode") {
}

std::shared_ptr<VisitingParseTree::BaseAttrNode> AttributedTestNodeSupplier::make_shared(void) {
  return std::shared_ptr<VisitingParseTree::BaseAttrNode>(new AttributedTestNode());
}

AttributedTestNodeSupplier AttributedTestNode::SUPPLIER;

VisitingParseTree::TraversalStatus AttributedTestNode::accept(
    VisitingParseTree::Visitor *visitor) {
  auto concrete_visitor = dynamic_cast<AttributedTestNodeVisitor *> (visitor);

  return concrete_visitor
      ? concrete_visitor->process_attributed_test_node(this)
      : VisitingParseTree::BaseAttrNode::accept(visitor);
}

AttributedTestNodeSupplier& AttributedTestNode::supplier() {
  return SUPPLIER;
}
