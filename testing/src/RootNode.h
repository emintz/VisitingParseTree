/*
 * RootNode.h
 *
 *  Created on: Sep 3, 2025
 *      Author: Eric Mintz
 *
 * Root node, the top (or bottom, if you prefer) of the tree
 */

#ifndef ROOTNODE_H_
#define ROOTNODE_H_

#include <memory>

#include "BaseAttrNode.h"
#include "TraversalStatus.h"
#include "Supplier.h"

namespace VisitingParseTree {

class RootNodeSupplier;
class RootNodeVisitor;

class RootNode : public BaseAttrNode {
  friend RootNodeSupplier;
public:
  RootNode(BaseAttrNode::forbid_public_access) {}
  virtual ~RootNode() = default;
  RootNode(const RootNode &other) = delete;
  RootNode(RootNode &&other) = delete;
  RootNode& operator=(const RootNode &other) = delete;
  RootNode& operator=(RootNode &&other) = delete;

  static RootNodeSupplier SUPPLIER;

  virtual TraversalStatus accept(Visitor *visitor) override;

  virtual Supplier<BaseAttrNode>& supplier() override;
};

class RootNodeSupplier : public Supplier<BaseAttrNode> {
  friend RootNode;
  RootNodeSupplier();
public:
  virtual ~RootNodeSupplier() = default;

  virtual std::shared_ptr<BaseAttrNode> make_shared() override;
};

class RootNodeVisitor {
protected:
  RootNodeVisitor() = default;
public:
  virtual ~RootNodeVisitor() = default;

  virtual TraversalStatus process_root_node(RootNode *node) = 0;
};

} /* namespace VisitingParseTree */

#endif /* ROOTNODE_H_ */
