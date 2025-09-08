/*
 * ValueNode.cpp
 *
 *  Created on: Sep 3, 2025
 *      Author: Eric Mintz
 */

#include <ValueNode.h>

namespace VisitingParseTree {

TraversalStatus ValueNode::accept(Visitor *visitor) {
  ValueNodeVisitor *concrete_visitor = dynamic_cast<ValueNodeVisitor *>(visitor);
  return concrete_visitor
      ? concrete_visitor->process_value_node(this)
      : BaseAttrNode::accept(visitor);
}

} /* namespace VisitingParseTree */
