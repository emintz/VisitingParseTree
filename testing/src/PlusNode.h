/*
 * PlusNode.h
 *
 *  Created on: Sep 2, 2025
 *      Author: Eric Mintz
 *
 * Addition operator node
 */

#ifndef PLUSNODE_H_
#define PLUSNODE_H_

#include <memory>

#include "OperatorNode.h"
#include "TraversalStatus.h"
#include "Supplier.h"

namespace VisitingParseTree {

class PlusNodeSupplier;
class PlusNodeVisitor;

class PlusNode : public OperatorNode {
  friend PlusNodeSupplier;
public:
  PlusNode(BaseAttrNode::forbid_public_access) {};
  virtual ~PlusNode() = default;
  PlusNode(const PlusNode &other) = delete;
  PlusNode(PlusNode &&other) = delete;
  PlusNode& operator=(const PlusNode &other) = delete;
  PlusNode& operator=(PlusNode &&other) = delete;

  static PlusNodeSupplier SUPPLIER;

  virtual TraversalStatus accept(Visitor *visitor) override;

  virtual Supplier<BaseAttrNode>& supplier() override;
};

class PlusNodeSupplier : public Supplier<BaseAttrNode> {
  friend PlusNode;
  PlusNodeSupplier();
public:
  virtual ~PlusNodeSupplier() = default;

  virtual std::shared_ptr<BaseAttrNode> make_shared() override;
};

class PlusNodeVisitor {
protected:
  PlusNodeVisitor() = default;
public:
  virtual ~PlusNodeVisitor() = default;

  virtual TraversalStatus process_plus_node(PlusNode *node) = 0;
};
} /* namespace VisitingParseTree */

#endif /* PLUSNODE_H_ */
