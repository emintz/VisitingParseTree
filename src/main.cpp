/*
 * main.cpp
 *
 *  Created on: Jul 19, 2025
 *      Author: Eric Mintz
 *
 * A poor excuse for testing.
 *
 * Copyright (C) 2025 Eric Mintz
 * All Rights Reserved
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "AttrNode.h"
#include "Host.h"
#include "Node.h"
#include "Supplier.h"
#include "NodeAction.h"
#include "Traversal.h"
#include "TreeCorruptError.h"
#include "VisitingAction.h"
#include "VisitingTraversal.h"
#include "Visitor.h"

#include <iostream>
#include <memory>

static const char *TRUE = "True";
static const char *FALSE = "False";
static const char *MISSING = ">>> MISSING <<<";

static const char *to_string(bool b) {
  return b ? TRUE : FALSE;
}

class TestNode : public VisitingParseTree::Node<TestNode> {

  class TestSupplier : public VisitingParseTree::Supplier<TestNode> {
  public:
    TestSupplier(void) :
        Supplier("TestNode") {
    }

    virtual ~TestSupplier() {}

    virtual std::shared_ptr<TestNode> make_shared(void) override {
      return std::make_shared<TestNode>();
    }
  };

  // Note: generate this in a CPP file.
  static TestSupplier TEST_SUPPLIER;

public:

  static TestSupplier& static_supplier() {
    return TEST_SUPPLIER;
  }

  virtual VisitingParseTree::Supplier<TestNode>& supplier(void) override {
    return TEST_SUPPLIER;
  }
};

TestNode::TestSupplier TestNode::TEST_SUPPLIER;

class TestHost;

class TestHostVisitor : public VisitingParseTree::Visitor {
protected:

  TestHostVisitor(void) {
  }

public:
  virtual ~TestHostVisitor() {
  }


  virtual VisitingParseTree::TraversalStatus
      process_test_host_node(TestHost *host) = 0;
};

class TestHost : public VisitingParseTree::Host<TestHost> {
  TestHost(void) :
    Host() {
  }
public:

  class TestHostSupplier : public VisitingParseTree::Supplier<TestHost> {

  public:
    TestHostSupplier(void) :
        Supplier("TestHost") {
    }

    virtual ~TestHostSupplier() {
    }

    virtual std::shared_ptr<TestHost> make_shared(void) {
      return std::shared_ptr<TestHost>(new TestHost());
    }
  };

  friend TestHostSupplier;

  static TestHostSupplier SUPPLIER;

  virtual VisitingParseTree::TraversalStatus accept(VisitingParseTree::Visitor *visitor) {
    auto concrete_visitor = dynamic_cast<TestHostVisitor *>(visitor);
    return concrete_visitor
        ? concrete_visitor->process_test_host_node(this)
        : VisitingParseTree::TraversalStatus::CONTINUE;
  }

  virtual TestHostSupplier& supplier(void) override {
    return SUPPLIER;
  }
};

TestHost::TestHostSupplier TestHost::SUPPLIER;

class CountingVisitor : public TestHostVisitor {
  int count_;
public:
  CountingVisitor(void) :
    count_(0) {
  }

  virtual ~CountingVisitor() {
  }

  inline int count(void) {
    return count_;
  }

  virtual VisitingParseTree::TraversalStatus process_test_host_node(TestHost *host) override {
    ++count_;
    return VisitingParseTree::TraversalStatus::CONTINUE;
  }
};

class TestAttribute : public VisitingParseTree::Attribute {
  TestAttribute(const char *name) :
    VisitingParseTree::Attribute(name) {
  }
public:
  static const TestAttribute NAME;
  static const TestAttribute STREET;
};

const TestAttribute TestAttribute::NAME("name");
const TestAttribute TestAttribute::STREET("street");

class TestAttrNode;

class TestAttrNodeVisitor {
protected:
  TestAttrNodeVisitor(void) {
  }

public:
  virtual ~TestAttrNodeVisitor() {
  }

  virtual VisitingParseTree::TraversalStatus
      process_test_attr_node(TestAttrNode *host) = 0;
};

/**
 * Supertype for attributed nodes.
 */
class TestAttrNode : public VisitingParseTree::AttrNode<TestAttrNode> {

protected:
  TestAttrNode() {
  }

public:
  virtual ~TestAttrNode() {
  }

  virtual VisitingParseTree::TraversalStatus
      accept(VisitingParseTree::Visitor *visitor) override {
    auto concrete_visitor = dynamic_cast<TestAttrNodeVisitor *>(visitor);
    return concrete_visitor
        ? concrete_visitor->process_test_attr_node(this)
        : VisitingParseTree::TraversalStatus::CONTINUE;
  }
};

class NameNode;

class NameNodeVisitor : public VisitingParseTree::Visitor  {
protected:
  NameNodeVisitor(void) {
  }
public:

  virtual VisitingParseTree::TraversalStatus process_name_node(NameNode *host) = 0;
};

class NameNode : TestAttrNode {
  friend class NameNodeSupplier;

  NameNode() :
    TestAttrNode() {
  }
public:

  class NameNodeSupplier : public VisitingParseTree::Supplier<TestAttrNode> {
    friend class NameNode;
    NameNodeSupplier(void) :
        VisitingParseTree::Supplier<TestAttrNode>("Name") {
    }
  public:
    virtual std::shared_ptr<TestAttrNode> make_shared(void) override {
      TestAttrNode *node = new NameNode();
      return std::shared_ptr<TestAttrNode>(node);
    }
  };

  static NameNodeSupplier SUPPLIER;

  virtual VisitingParseTree::TraversalStatus accept(VisitingParseTree::Visitor *visitor) {
    auto concrete_visitor = dynamic_cast<NameNodeVisitor *>(visitor);
    return concrete_visitor
        ? concrete_visitor->process_name_node(this)
        : TestAttrNode::accept(visitor);
  }

  virtual NameNodeSupplier &supplier(void) override {
    return SUPPLIER;
  }
};

NameNode::NameNodeSupplier NameNode::SUPPLIER;

class PersonNode;

class PersonNodeVisitor {
protected:
  PersonNodeVisitor(void) {
  }

public:
  virtual ~PersonNodeVisitor() {
  }

  virtual VisitingParseTree::TraversalStatus process_person_node(PersonNode *host) = 0;
};

class PersonNode : public TestAttrNode {
  friend class PersonNodeSupplier;

  PersonNode(void) :
    TestAttrNode() {
  }

  class PersonNodeSupplier : public VisitingParseTree::Supplier<TestAttrNode> {
    friend PersonNode;
    PersonNodeSupplier(void) :
      VisitingParseTree::Supplier<TestAttrNode>("PersonNode") {
    }

  public:
    virtual std::shared_ptr<TestAttrNode> make_shared(void) override {
      auto node = new PersonNode();
      return std::shared_ptr<TestAttrNode>(node);
    }
  };

public:

  static PersonNodeSupplier SUPPLIER;

  virtual VisitingParseTree::TraversalStatus accept(VisitingParseTree::Visitor *visitor) {
    auto concrete_visitor = dynamic_cast<PersonNodeVisitor *>(visitor);
    return concrete_visitor
        ? concrete_visitor->process_person_node(this)
        : TestAttrNode::accept(visitor);
  }

  virtual VisitingParseTree::Supplier<TestAttrNode>& supplier(void) override {
    return SUPPLIER;
  }
};

PersonNode::PersonNodeSupplier PersonNode::SUPPLIER;

class AttrNodeDumper : public TestAttrNodeVisitor, public VisitingParseTree::Visitor {
  std::string prefix_;

public:
  AttrNodeDumper(const char *prefix) :
      prefix_(prefix) {
  }

  virtual VisitingParseTree::TraversalStatus process_test_attr_node(TestAttrNode *node) override {
    std::cout << "On " << prefix_ << ", type: " << node->type_name();

    std::optional<std::string> value;

    value = node->get(TestAttribute::NAME);
    if (value.has_value()) {
      std::cout << ", name: " << value.value();
    }

    value = node->get(TestAttribute::STREET);
    if (value.has_value()) {
      std::cout << ", street: " << value.value();
    }

    std::cout << '.' << std::endl;
    return VisitingParseTree::TraversalStatus::CONTINUE;
  }
};

static int test_node(void) {
  std::cout << "Node type name: " << TestNode::static_supplier().class_name() << std::endl;
  auto shared_node = TestNode::static_supplier().make_shared();
  if (!shared_node) {
    std::cout << "Node was not created :-(" << std::endl;
    return 1;
  }
  std::cout << "Node created with " << shared_node->child_count() << " children \\0/" << std::endl;
  std::shared_ptr<TestNode> new_child = shared_node->append_child(TestNode::static_supplier());
  std::cout << "Node now has " << shared_node->child_count() << " children!" << std::endl;

  auto child = shared_node->child(0);
  if (!child) {
    std::cout << "Child node not found!" << std::endl;
    return 2;
  }
  std::cout << "Child node found!" << std::endl;
  auto child_itr = child->find_in_parent();
  if (child != *child_itr) {
    std::cout << "Child not found in parent :-(" << std::endl;
    return 3;
  }
  std::cout << "Child found in parent \\0/" << std::endl;
  return 0;
}

static int test_host(void) {
  std::cout << "Test host node type is: " << TestHost::SUPPLIER.class_name() << std::endl;
  auto node = TestHost::SUPPLIER.make_shared();
  if (!node) {
    std::cout << "Host node creation failed :-(" << std::endl;
    return 1;
  }
  std::cout << "Host node created \\0/" << std::endl;
  std::cout << "Child count is: " << node->child_count() << std::endl;
  node->append_child(TestHost::SUPPLIER);
  std::cout << "Child count after append is: " << node->child_count() << std::endl;
  auto child = node->child(0);
  if (!child) {
    std::cout << "Child not found :-(" << std::endl;
    return 2;
  }
  std::cout << "Child found\\0/" << std::endl;
  auto child_iter = child->find_in_parent();
  if (child != *child_iter) {
    std::cout << "Find in parent FAILED :-(" << std::endl;
    return 3;
  }
  std::cout << "Found child in parent \\0/" << std::endl;

  CountingVisitor visitor;
  std::cout<< "Initial count is: " << visitor.count() << std::endl;
  auto visitor_status = node->accept(&visitor);
  if (VisitingParseTree::TraversalStatus::CONTINUE != visitor_status) {
    std::cout << "Bad return from visitor :-(" << std::endl;
    return 4;
  }
  std::cout << "Visitor count is: " << visitor.count() << std::endl;
  return 0;
}

static std::string attr_value(std::shared_ptr<TestAttrNode> node, const TestAttribute& attribute) {
  std::optional<std::string> maybe_value = node->get(attribute);
  return maybe_value.has_value()
      ? maybe_value.value()
      : MISSING;
}

static int test_attr(void) {
  auto root = NameNode::SUPPLIER.make_shared();
  std::cout << "Root attributed node type: " << root->type_name()
      << " with " << root->attribute_count() << " attributes." << std::endl;
  root->set(TestAttribute::NAME, "Metuchen");
  std::cout << "After set() root has " << root->attribute_count() << " attributes." << std::endl;
  std::cout << "Has NAME is: " << to_string(root->has(TestAttribute::NAME))
      << ", has STREET is: " << to_string(root->has(TestAttribute::STREET)) << std::endl;
  auto maybe_name = root->get(TestAttribute::NAME);
  if (maybe_name.has_value()) {
    std::cout << "NAME is: " << maybe_name.value() << " \\0/" << std::endl;
  } else {
    std::cout << "Name node has no value :-(" << std::endl;
    return 1;
  }
  root->append_child(PersonNode::SUPPLIER)
      ->set(TestAttribute::NAME, "Max Mintz")
      ->set(TestAttribute::STREET, "62 Charles Street")
      ->append_sibling(PersonNode::SUPPLIER)
      ->set(TestAttribute::NAME, "Shelley Mintz")
      ->set(TestAttribute::STREET, "131 Woodbridge Avenue");
  std::cout << "After appending, root has " << root->child_count() << " children." << std::endl;
  auto first_child = root->child(0);
  std::cout << "First child NAME: " << attr_value(first_child, TestAttribute::NAME)
      << ", STREET: " << attr_value(first_child, TestAttribute::STREET) << std::endl;
  auto second_child = root->child(1);
  std::cout << "Second child NAME: " << attr_value(second_child,TestAttribute::NAME)
      << ", STREET: " << attr_value(second_child, TestAttribute::STREET) << std::endl;

  AttrNodeDumper on_entry("entry");
  AttrNodeDumper on_exit("exit");
  VisitingParseTree::VisitingTraversal<TestAttrNode> traversal(
      &on_entry,
      &on_exit);
  traversal(root);
  return 0;
}

int main(int ac, char **av) {
  int status = test_node();
  if (0 == status) {
    status = test_host();
  }
  if (0 == status) {
    status = test_attr();
  }
  if (0 == status) {
    std::cout << "Tests passed." << std::endl;
  }
  return status;
}

