/*
 * DivNode.h
 *
 *  Created on: Sep 4, 2025
 *      Author: Eric Mintz
 */

#ifndef DIVNODE_H_
#define DIVNODE_H_

#include <memory>

#include "OperatorNode.h"
#include "Supplier.h"
#include "TraversalStatus.h"
#include "Visitor.h"

namespace VisitingParseTree {

class DivNodeSupplier;
class DivNodeVisitor;

class DivNode : public OperatorNode {
  friend class DivNodeSupplier;
public:
  DivNode(BaseAttrNode::forbid_public_access);
  virtual ~DivNode() = default;
  DivNode(const DivNode &other) = delete;
  DivNode(DivNode &&other) = delete;
  DivNode& operator=(const DivNode &other) = delete;
  DivNode& operator=(DivNode &&other) = delete;

  static DivNodeSupplier SUPPLIER;

  virtual TraversalStatus accept(Visitor *visitor) override;

  virtual Supplier<BaseAttrNode>& supplier();
};

class DivNodeSupplier : public Supplier<BaseAttrNode> {
  friend DivNode;
  DivNodeSupplier();
public:
  virtual ~DivNodeSupplier() = default;

  virtual std::shared_ptr<BaseAttrNode> make_shared() override;
};

class DivNodeVisitor {
protected:
  DivNodeVisitor() = default;
public:
  virtual ~DivNodeVisitor() = default;

  virtual TraversalStatus process_div_node(DivNode *node) = 0;
};
} /* namespace VisitingParseTree */

#endif /* DIVNODE_H_ */
