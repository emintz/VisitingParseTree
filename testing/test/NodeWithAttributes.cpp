/*
 * NodeWithAttributes.cpp
 *
 *  Created on: Sep 2, 2025
 *      Author: Eric Mintz
 *
 * Tests attributed nodes.
 */

#include <memory>
#include <string>

#include "gtest/gtest.h"

#include "AttributedTestNode.h"
#include "TestAttribute.h"

using namespace std;
using namespace VisitingParseTree;

TEST(NodeWithAttributes, Empty_Copy) {
  auto root = AttributedTestNode::SUPPLIER.make_shared();
  auto source = root->append_child(AttributedTestNode::SUPPLIER);
  source->append_child(AttributedTestNode::SUPPLIER);
  source->append_child(AttributedTestNode::SUPPLIER);
  source->set(TestAttribute::BIRTH_DATE, "July 16, 1945");
  source->set(TestAttribute::NAME, "Trinity Test");
  auto destination = source->empty_copy();
  ASSERT_EQ(source->supplier(), destination->supplier());
  ASSERT_TRUE(destination->is_root());
  ASSERT_TRUE(destination->is_leaf());
  ASSERT_EQ(0, destination->child_count());
}

TEST(NodeWithAttributes, Clone) {
  auto root = AttributedTestNode::SUPPLIER.make_shared();
  auto source = root->append_child(AttributedTestNode::SUPPLIER);
  source->append_child(AttributedTestNode::SUPPLIER);
  source->append_child(AttributedTestNode::SUPPLIER);
  source->set(TestAttribute::BIRTH_DATE, "July 16, 1945");
  source->set(TestAttribute::NAME, "Trinity Test");
  auto destination = source->clone();
  ASSERT_EQ(source->supplier(), destination->supplier());
  ASSERT_TRUE(destination->is_root());
  ASSERT_TRUE(destination->is_leaf());
  ASSERT_EQ(source->attribute_count(), destination->attribute_count());
  ASSERT_TRUE(destination->has(TestAttribute::BIRTH_DATE));
  ASSERT_TRUE(destination->has(TestAttribute::NAME));
  ASSERT_STREQ("July 16, 1945", destination->get(TestAttribute::BIRTH_DATE).c_str());
  ASSERT_STREQ("Trinity Test", destination->get(TestAttribute::NAME).c_str());
}
