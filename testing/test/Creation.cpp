/*
 * Creation.cpp
 *
 *  Created on: Aug 18, 2025
 *      Author: Eric Mintz
 *
 * Test node creation.
 */

#include <memory>
#include <vector>

#include "gtest/gtest.h"

#include "AttributedTestNode.h"
#include "Gather.h"
#include "TestAttribute.h"

using namespace std;
using namespace VisitingParseTree;

TEST(Creation, SimpleCreation) {
  auto node = AttributedTestNode::SUPPLIER.make_shared();
  ASSERT_EQ(node, node);
  VisitingParseTree::Supplier<BaseAttrNode>& supplier = node->supplier();
  ASSERT_TRUE(node);
  ASSERT_FALSE(node->parent());
  ASSERT_TRUE(node->is_leaf());
  ASSERT_TRUE(node->is_root());
  ASSERT_FALSE(node->has_children());
  ASSERT_EQ(0, node->attribute_count());
  ASSERT_FALSE(node->has(TestAttribute::BIRTH_DATE));
  ASSERT_FALSE(node->has(TestAttribute::NAME));
  ASSERT_TRUE(node->get(TestAttribute::BIRTH_DATE).empty());
  ASSERT_TRUE(node->get(TestAttribute::NAME).empty());
  ASSERT_EQ(node->child_count(), 0);
  ASSERT_GT(node->id(), 0);
  ASSERT_STREQ("AttributedTestNode", node->type_name().c_str());
  ASSERT_STREQ("AttributedTestNode", supplier.class_name().c_str());
  ASSERT_STREQ("AttributedTestNode", AttributedTestNode::SUPPLIER.class_name().c_str());
}

TEST(Creation, BasicTreeManipulation) {
  Supplier<BaseAttrNode>& supplier = AttributedTestNode::SUPPLIER;
  auto gather = Gather();
  auto root = supplier.make_shared();
  ASSERT_TRUE(root->is_root());
  ASSERT_TRUE(root->is_leaf());
  ASSERT_FALSE(root->has_children());
  ASSERT_EQ(0, root->child_count());
  root->for_each_child(gather.reset());
  ASSERT_TRUE(gather().empty());

  auto first_child = root->append_child(supplier);
  ASSERT_TRUE(first_child);
  ASSERT_STREQ(first_child->type_name().c_str(), supplier.class_name().c_str());
  ASSERT_NE(root->id(), first_child->id());
  ASSERT_NE(root, first_child);
  ASSERT_FALSE(root->is_leaf());
  ASSERT_TRUE(root->is_root());
  ASSERT_TRUE(first_child->is_leaf());
  ASSERT_FALSE(first_child->is_root());
  ASSERT_EQ(root, first_child->parent());
  ASSERT_EQ(first_child, root->child(0));
  auto second_child = first_child->append_sibling(supplier);
  ASSERT_TRUE(second_child);
  ASSERT_NE(second_child->id(), first_child->id());
  ASSERT_NE(second_child->id(), root->id());
  ASSERT_NE(second_child, first_child);
  ASSERT_NE(second_child, root);
  ASSERT_EQ(2, root->child_count());
  ASSERT_EQ(first_child, root->child(0));
  ASSERT_EQ(second_child, root->child(1));
}
