/*
 * AttributedTestNode.h
 *
 *  Created on: Jul 14, 2025
 *      Author: Eric Mintz
 */

#ifndef ATTRIBUTEDTESTNODE_H_
#define ATTRIBUTEDTESTNODE_H_

#include <BaseAttrNode.h>
#include <Attribute.h>
#include <AttrNode.h>
#include <Supplier.h>
#include <TraversalStatus.h>
#include <Visitor.h>
#include <VoidFunction.h>

#include <memory>

class AttributedTestNode;
class AttributedTestNodeVisitor;

class AttributedTestNodeSupplier : public VisitingParseTree::Supplier<VisitingParseTree::BaseAttrNode> {
  friend class AttributedTestNode;
public:
  AttributedTestNodeSupplier(void);

  virtual ~AttributedTestNodeSupplier() = default;

  virtual std::shared_ptr<VisitingParseTree::BaseAttrNode> make_shared(void) override;
};


class AttributedTestNode :
    public VisitingParseTree::BaseAttrNode {
  friend class AttributedTestNodeSupplier;

  AttributedTestNode(void) = default;
public:

  static AttributedTestNodeSupplier SUPPLIER;

  virtual ~AttributedTestNode() = default;

  virtual VisitingParseTree::TraversalStatus accept(
      VisitingParseTree::Visitor *visitor) override;

  virtual AttributedTestNodeSupplier& supplier(void) override;
};

class AttributedTestNodeVisitor {
public:
  virtual ~AttributedTestNodeVisitor() = default;

  virtual VisitingParseTree::TraversalStatus process_attributed_test_node (
      AttributedTestNode *host) = 0;
};
#endif /* ATTRIBUTEDTESTNODE_H_ */
