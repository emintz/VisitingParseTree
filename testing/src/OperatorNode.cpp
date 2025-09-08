/*
 * OperatorNode.cpp
 *
 *  Created on: Sep 2, 2025
 *      Author: Eric Mintz
 */

#include <OperatorNode.h>

namespace VisitingParseTree {

TraversalStatus OperatorNode::accept(Visitor *visitor) {
  auto concrete_visitor = dynamic_cast<OperatorNodeVisitor *> (visitor);

  return concrete_visitor
      ? concrete_visitor->process_operator_node(this)
      : BaseAttrNode::accept(visitor);
}

} /* namespace VisitingParseTree */
