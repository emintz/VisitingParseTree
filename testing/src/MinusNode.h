/*
 * MinusNode.h
 *
 *  Created on: Sep 2, 2025
 *      Author: Eric Mintz
 *
 * Subtraction operator node
 */

#ifndef MINUSNODE_H_
#define MINUSNODE_H_

#include <memory>

#include "OperatorNode.h"
#include "Supplier.h"
#include "TraversalStatus.h"
#include "Visitor.h"

namespace VisitingParseTree {

class MinusNodeSupplier;
class MinusNodeVisitor;

class MinusNode : public OperatorNode {
  friend class MinusNodeSupplier;
public:
  MinusNode(BaseAttrNode::forbid_public_access) {}
  virtual ~MinusNode() = default;
  MinusNode(const MinusNode &other) = delete;
  MinusNode(MinusNode &&other) = delete;
  MinusNode& operator=(const MinusNode &other) = delete;
  MinusNode& operator=(MinusNode &&other) = delete;

  static MinusNodeSupplier SUPPLIER;

  virtual TraversalStatus accept(Visitor *visitor) override;

  virtual Supplier<BaseAttrNode>& supplier() override;
};

class MinusNodeSupplier : public Supplier<BaseAttrNode> {
  friend MinusNode;
  MinusNodeSupplier();
public:
  virtual ~MinusNodeSupplier() = default;

  virtual std::shared_ptr<BaseAttrNode> make_shared() override;
};

class MinusNodeVisitor {
protected:
  MinusNodeVisitor() = default;
public:
  virtual ~MinusNodeVisitor() = default;

  virtual TraversalStatus process_minus_node(MinusNode *node) = 0;
};

} /* namespace VisitingParseTree */

#endif /* MINUSNODE_H_ */
