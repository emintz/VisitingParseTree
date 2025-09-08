/*
 * IntegerNode.h
 *
 *  Created on: Sep 3, 2025
 *      Author: Eric Mintz
 *
 * Integer value node
 */

#ifndef INTEGERNODE_H_
#define INTEGERNODE_H_

#include <memory>

#include "TraversalStatus.h"
#include "Supplier.h"
#include "ValueNode.h"

namespace VisitingParseTree {

class IntegerNodeSupplier;
class IntegerNodeVisitor;

class IntegerNode : public ValueNode {
  friend IntegerNodeSupplier;
public:
  IntegerNode(BaseAttrNode::forbid_public_access) {};
  virtual ~IntegerNode() = default;
  IntegerNode(const IntegerNode &other) = delete;
  IntegerNode(IntegerNode &&other) = delete;
  IntegerNode& operator=(const IntegerNode &other) = delete;
  IntegerNode& operator=(IntegerNode &&other) = delete;

  static IntegerNodeSupplier SUPPLIER;

  virtual TraversalStatus accept(Visitor *visitor) override;

  virtual Supplier<BaseAttrNode>& supplier() override;
};

class IntegerNodeSupplier : public Supplier<BaseAttrNode> {
  friend IntegerNode;
  IntegerNodeSupplier();
public:
  virtual ~IntegerNodeSupplier() = default;

  virtual std::shared_ptr<BaseAttrNode> make_shared() override;
};

class IntegerNodeVisitor {
protected:
  IntegerNodeVisitor() = default;
public:
  virtual ~IntegerNodeVisitor() = default;

  virtual TraversalStatus process_integer_node(IntegerNode *node) = 0;
};

} /* namespace VisitingParseTree */

#endif /* INTEGERNODE_H_ */
