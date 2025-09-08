/*
 * Traversal.cpp
 *
 *  Created on: Sep 3, 2025
 *      Author: Eric Mintz
 *
 * Tests tree basic and visiting tree traversal
 */

#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "gtest/gtest.h"

#include "BaseAttrNodeTraversal.h"
#include "IntegerNode.h"
#include "MinusNode.h"
#include "OperatorNode.h"
#include "NodeAction.h"
#include "PlusNode.h"
#include "RootNode.h"
#include "TestAttribute.h"
#include "TestTrees.h"
#include "Traversal.h"
#include "ValueNode.h"
#include "VacuousVoidFunction.h"
#include "VoidFunction.h"

using namespace VisitingParseTree;
using namespace std;

static TraversalStatus entry_status_of(shared_ptr<BaseAttrNode> node) {
  if (node->has(TestAttribute::CANCEL_ON_ENTRY)) {
    return TraversalStatus::CANCEL;
  }
  if (node->has(TestAttribute::BYPASS_CHILDREN_ON_ENTRY)) {
    return TraversalStatus::BYPASS_CHILDREN;
  }
  return TraversalStatus::CONTINUE;
}

static TraversalStatus exit_status_of(shared_ptr<BaseAttrNode> node) {
  if (node->has(TestAttribute::CANCEL_ON_EXIT)) {
    return TraversalStatus::CANCEL;
  }
  if (node->has(TestAttribute::BYPASS_CHILDREN_ON_EXIT)) {
    return TraversalStatus::BYPASS_CHILDREN;
  }
  return TraversalStatus::CONTINUE;
}

// Hide test-specific classes from other tests
namespace TraversalTest {
class TraversalContext {
  int level_ = 0;
  vector<pair<string, int>> entries_;
  vector<pair<string, int>> exits_;
public:
  void enter(shared_ptr<BaseAttrNode> node) {
    entries_.emplace_back(node->get(TestAttribute::SERIAL_NO), level_);
  }

  void exit(shared_ptr<BaseAttrNode> node) {
    exits_.emplace_back(node->get(TestAttribute::SERIAL_NO), level_);
  }

  vector<pair<string, int>>& entries() {
    return entries_;
  }

  vector<pair<string, int>>& exits() {
    return exits_;
  }

  void descend() {
    ++level_;
  }

  void ascend() {
    --level_;
  }
};

class AscendFunction : public VoidFunction {
  TraversalContext& context_;
public:
  AscendFunction(TraversalContext& context) :
    context_(context) {}

  virtual void operator() () override {
    context_.ascend();
  }
};

class DescendFunction : public VoidFunction {
  TraversalContext& context_;
public:
  DescendFunction(TraversalContext& context) :
    context_(context) {}

  virtual void operator() () override {
    context_.descend();
  }
};

class Enter : public NodeAction<BaseAttrNode> {
  TraversalContext& context_;
public:
  Enter(TraversalContext& context) :
    context_(context) {}

  virtual TraversalStatus operator()(shared_ptr<BaseAttrNode> node) override {
    context_.enter(node);
    return entry_status_of(node);
  }
};

class Exit : public NodeAction<BaseAttrNode> {
  TraversalContext& context_;
public:
  Exit(TraversalContext& context) :
    context_(context) {}

  virtual TraversalStatus operator()(shared_ptr<BaseAttrNode> node) override {
    context_.exit(node);
    return exit_status_of(node);
  }
};

};/* namespace TraversalTest */

using namespace TraversalTest;

TEST(Traversal, Basic) {
  vector<pair<string, int>> expected_entries = {
      {"1", 0},
      {"2", 1},
      {"3", 2},
      {"4", 2}
  };
  vector<pair<string, int>> expected_exits = {
      {"3", 2},
      {"4", 2},
      {"2", 1},
      {"1", 0}
  };

  TraversalContext context;
  AscendFunction on_ascent(context);
  DescendFunction on_descent(context);
  Enter on_entry(context);
  Exit on_exit(context);

  auto root = TestTrees::simple_addition();
  ASSERT_STREQ("RootNode", root->supplier().class_name().c_str());
  BaseAttrNodeTraversal traversal(on_entry, on_exit, on_descent, on_ascent);
  traversal(root);
  ASSERT_EQ(expected_entries, context.entries());
  ASSERT_EQ(expected_exits, context.exits());
}

TEST(Traversal, BypassOnEntry) {

  vector<pair<string, int>> expected_entries = {
      {"1", 0},
      {"2", 1},
      {"3", 2},
      {"4", 2},
      {"8", 2}
  };
  vector<pair<string, int>> expected_exits = {
      {"3", 2},
      {"4", 2},
      {"8", 2},
      {"2", 1},
      {"1", 0}
  };

  TraversalContext context;
  AscendFunction on_ascent(context);
  DescendFunction on_descent(context);
  Enter on_entry(context);
  Exit on_exit(context);
  auto root = TestTrees::bypass_on_entry();
  ASSERT_STREQ("RootNode", root->supplier().class_name().c_str());

  BaseAttrNodeTraversal traversal(on_entry, on_exit, on_descent, on_ascent);
  traversal(root);
  ASSERT_EQ(context.entries(), expected_entries);
  ASSERT_EQ(context.exits(), expected_exits);
}

TEST(Traversal, BypassOnExit) {
  vector<pair<string, int>> expected_entries = {
      {"1", 0},
      {"2", 1},
      {"3", 2},
      {"4", 2}
  };
  vector<pair<string, int>> expected_exits = {
      {"3", 2},
      {"4", 2},
      {"2", 1},
      {"1", 0}
  };

  TraversalContext context;
  AscendFunction on_ascent(context);
  DescendFunction on_descent(context);
  Enter on_entry(context);
  Exit on_exit(context);
  BaseAttrNodeTraversal traversal(on_entry, on_exit, on_descent, on_ascent);

  auto root = TestTrees::bypass_on_exit();
  ASSERT_STREQ("RootNode", root->supplier().class_name().c_str());
  traversal(root);
  ASSERT_EQ(expected_entries, context.entries());
  ASSERT_EQ(expected_exits, context.exits());
}

TEST(Traversal, CancelOnEntry) {

  vector<pair<string, int>> expected_entries = {
      {"1", 0},
      {"2", 1},
      {"3", 2},
      {"4", 2}
  };
  vector<pair<string, int>> expected_exits = {
      {"3", 2}
  };

  TraversalContext context;
  AscendFunction on_ascent(context);
  DescendFunction on_descent(context);
  Enter on_entry(context);
  Exit on_exit(context);
  BaseAttrNodeTraversal traversal(on_entry, on_exit, on_descent, on_ascent);

  auto root = TestTrees::cancel_on_entry();
  traversal(root);
  ASSERT_STREQ("RootNode", root->supplier().class_name().c_str());
  ASSERT_EQ(context.entries(), expected_entries);
  ASSERT_EQ(context.exits(), expected_exits);
}

TEST(Traversal, CancelOnExit) {

  vector<pair<string, int>> expected_entries = {
      {"1", 0},
      {"2", 1},
      {"3", 2},
      {"4", 2},
      {"5", 3},
      {"6", 3}
  };
  vector<pair<string, int>> expected_exits = {
      {"3", 2},
      {"5", 3},
      {"6", 3},
      {"4", 2}
  };

  TraversalContext context;
  AscendFunction on_ascent(context);
  DescendFunction on_descent(context);
  Enter on_entry(context);
  Exit on_exit(context);
  BaseAttrNodeTraversal traversal(on_entry, on_exit, on_descent, on_ascent);

  auto root = TestTrees::cancel_on_exit();
  traversal(root);
  ASSERT_STREQ("RootNode", root->supplier().class_name().c_str());
  ASSERT_EQ(context.entries(), expected_entries);
  ASSERT_EQ(context.exits(), expected_exits);
}
