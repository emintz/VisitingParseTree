/*
 * TimesNode.h
 *
 *  Created on: Sep 4, 2025
 *      Author: Eric Mintz
 */

#ifndef TIMESNODE_H_
#define TIMESNODE_H_

#include "OperatorNode.h"
#include "Supplier.h"
#include "TraversalStatus.h"
#include "Visitor.h"

namespace VisitingParseTree {

class TimesNodeSupplier;
class TimesNodeVisitor;

class TimesNode : public OperatorNode {
  friend class TimesNodeSupplier;
public:
  TimesNode(BaseAttrNode::forbid_public_access);
  virtual ~TimesNode() = default;
  TimesNode(const TimesNode &other) = delete;
  TimesNode(TimesNode &&other) = delete;
  TimesNode& operator=(const TimesNode &other) = delete;
  TimesNode& operator=(TimesNode &&other) = delete;

  static TimesNodeSupplier SUPPLIER;

  virtual TraversalStatus accept(Visitor *visitor) override;

  virtual Supplier<BaseAttrNode>& supplier() override;
};

class TimesNodeSupplier : public Supplier<BaseAttrNode> {
  friend TimesNode;
  TimesNodeSupplier();
public:
  virtual ~TimesNodeSupplier() = default;

  virtual std::shared_ptr<BaseAttrNode> make_shared() override;
};

class TimesNodeVisitor {
protected:
  TimesNodeVisitor() = default;
public:
  virtual ~TimesNodeVisitor() = default;

  virtual TraversalStatus process_times_node(TimesNode *node) = 0;
};

} /* namespace VisitingParseTree */

#endif /* TIMESNODE_H_ */
