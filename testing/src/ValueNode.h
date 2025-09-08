/*
 * ValueNode.h
 *
 *  Created on: Sep 3, 2025
 *      Author: Eric Mintz
 */

#ifndef VALUENODE_H_
#define VALUENODE_H_

#include <memory>

#include "BaseAttrNode.h"
#include "TraversalStatus.h"
#include "Visitor.h"

namespace VisitingParseTree {

class ValueNode : public BaseAttrNode {
protected:
  ValueNode() = default;
public:
  virtual ~ValueNode() = default;
  ValueNode(const ValueNode &other) = delete;
  ValueNode(ValueNode &&other) = delete;
  ValueNode& operator=(const ValueNode &other) = delete;
  ValueNode& operator=(ValueNode &&other) = delete;

  virtual TraversalStatus accept(Visitor *visitor) override;
};

class ValueNodeVisitor {
protected:
  ValueNodeVisitor() = default;
public:
  virtual TraversalStatus process_value_node(ValueNode *node) = 0;
};

} /* namespace VisitingParseTree */

#endif /* VALUENODE_H_ */
