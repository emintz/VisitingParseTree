/*
 * OperatorNode.h
 *
 *  Created on: Sep 2, 2025
 *      Author: Eric Mintz
 *
 * Arithmetic operator node superclass
 */

#ifndef OPERATORNODE_H_
#define OPERATORNODE_H_

#include <memory>

#include "BaseAttrNode.h"
#include "TraversalStatus.h"
#include "Visitor.h"

namespace VisitingParseTree {

class OperatorNode : public BaseAttrNode {
protected:
  OperatorNode() = default;
public:
  virtual ~OperatorNode() = default;
  virtual TraversalStatus accept(Visitor *visitor) override;
};

class OperatorNodeVisitor {
protected:
  OperatorNodeVisitor() = default;
public:
  virtual ~OperatorNodeVisitor() = default;

  virtual TraversalStatus process_operator_node(OperatorNode *node) = 0;

};

} /* namespace VisitingParseTree */

#endif /* OPERATORNODE_H_ */
