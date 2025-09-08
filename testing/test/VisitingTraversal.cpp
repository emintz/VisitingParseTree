/*
 * VisitingTraversal.cpp
 *
 *  Created on: Sep 5, 2025
 *      Author: Eric Mintz
 *
 * Test visitor-based traversal.
 */

#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "gtest/gtest.h"

#include "BaseAttrNodeTraversal.h"
#include "DivNode.h"
#include "IntegerNode.h"
#include "MinusNode.h"
#include "OperatorNode.h"
#include "NodeAction.h"
#include "PlusNode.h"
#include "RootNode.h"
#include "TestAttribute.h"
#include "TestTrees.h"
#include "TimesNode.h"
#include "ValueNode.h"
#include "VacuousVoidFunction.h"
#include "VisitingTraversal.h"
#include "VoidFunction.h"

using namespace std;
using namespace VisitingParseTree;

class OperatorNodeCounter :
    public Visitor,
    public OperatorNodeVisitor {
  int counter_;
public:
  OperatorNodeCounter() :
    counter_(0) {
  }

  virtual ~OperatorNodeCounter() = default;

  virtual TraversalStatus process_operator_node(OperatorNode *node) override {
    ++counter_;
    return TraversalStatus::CONTINUE;
  }

  int counter() const {
    return counter_;
  }
};

class ValueNodeCounter :
    public Visitor,
    public ValueNodeVisitor {
  int counter_;
public:
  ValueNodeCounter() :
    counter_(0) {
  }

  virtual ~ValueNodeCounter() = default;

  virtual TraversalStatus process_value_node(ValueNode *node) {
    ++counter_;
    return TraversalStatus::CONTINUE;
  }

  int counter() const {
    return counter_;
  }
};

class CalcContext {
  vector<int> value_stack;
public:
  virtual ~CalcContext() = default;

  void push_value(const string& formatted_value) {
    value_stack.push_back(atoi(formatted_value.c_str()));
  }

  void push_value(int value) {
    value_stack.push_back(value);
  }

  int pop_value() {
    int top_element = value_stack.back();
    value_stack.pop_back();
    return top_element;
  }

  int stack_depth() const {
    return value_stack.size();
  }
};

class CalcOnEntry :
    public Visitor,
    public IntegerNodeVisitor {

  CalcContext& context_;
public:
  virtual ~CalcOnEntry() = default;

  CalcOnEntry(CalcContext& context) :
    context_(context) {
  }

  virtual TraversalStatus process_integer_node(IntegerNode *node) {
    context_.push_value(node->get(TestAttribute::VALUE));
    return TraversalStatus::CONTINUE;
  }
};

class CalcOnExit :
    public Visitor,
    public DivNodeVisitor,
    public MinusNodeVisitor,
    public PlusNodeVisitor,
    public TimesNodeVisitor {
  CalcContext& context_;
public:
  CalcOnExit(CalcContext& context) :
    context_(context) {
    }
  virtual ~CalcOnExit() = default;

  virtual TraversalStatus process_div_node(DivNode *node) override {
    int rhs = context_.pop_value();
    int lhs = context_.pop_value();
    context_.push_value(lhs / rhs);
    return TraversalStatus::CONTINUE;
  }

  virtual TraversalStatus process_minus_node(MinusNode *node) {
    int rhs = context_.pop_value();
    int lhs = context_.pop_value();
    context_.push_value(lhs - rhs);
    return TraversalStatus::CONTINUE;
  }

  virtual TraversalStatus process_plus_node(PlusNode *node) {
    int rhs = context_.pop_value();
    int lhs = context_.pop_value();
    context_.push_value(lhs + rhs);
    return TraversalStatus::CONTINUE;
  }

  virtual TraversalStatus process_times_node(TimesNode *node) {
    int rhs = context_.pop_value();
    int lhs = context_.pop_value();
    context_.push_value(lhs * rhs);
    return TraversalStatus::CONTINUE;
  }
};

static VacuousVoidFunction do_nothing;

TEST(VisitingTraversal, AbstractNodeVisitation) {
  OperatorNodeCounter on_entry;
  ValueNodeCounter on_exit;

  VisitingTraversal<BaseAttrNode> traversal(
      &on_entry, &on_exit, do_nothing, do_nothing);
  auto root = TestTrees::addition_and_multiplication();
  ASSERT_EQ(RootNode::SUPPLIER, root->supplier());
  traversal(root);
  ASSERT_EQ(2, on_entry.counter());
  ASSERT_EQ(3, on_exit.counter());
}

TEST(VisitingTraversal, SimpleArithmetic) {
  CalcContext context;
  CalcOnEntry on_entry(context);
  CalcOnExit on_exit(context);
  auto root = TestTrees::addition_and_multiplication();
  ASSERT_EQ(RootNode::SUPPLIER, root->supplier());
  VisitingTraversal<BaseAttrNode> traversal(
      &on_entry, &on_exit, do_nothing, do_nothing);
  traversal(root);
  ASSERT_EQ(1, context.stack_depth());
  ASSERT_EQ(152, context.pop_value());
}

TEST(VisitingTraversal, FullCalculation) {
  CalcContext context;
  CalcOnEntry on_entry(context);
  CalcOnExit on_exit(context);
  auto root = TestTrees::all_operations();
  ASSERT_EQ(RootNode::SUPPLIER, root->supplier());
  VisitingTraversal<BaseAttrNode> traversal(
      &on_entry, &on_exit, do_nothing, do_nothing);
  traversal(root);
//  ASSERT_EQ(1, context.stack_depth());
//  ASSERT_EQ(14, context.pop_value());
}
