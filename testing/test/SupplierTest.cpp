/*
 * SupplierTest.cpp
 *
 *  Created on: Sep 4, 2025
 *      Author: Eric Mintz
 */

#include <memory>
#include <string>
#include <vector>

#include "gtest/gtest.h"

#include "IntegerNode.h"
#include "MinusNode.h"
#include "Supplier.h"

using namespace VisitingParseTree;

TEST(Supplier, Equality) {
  ASSERT_STREQ("MinusNode", MinusNode::SUPPLIER.class_name().c_str());
  ASSERT_EQ(MinusNode::SUPPLIER, MinusNode::SUPPLIER);
  auto node1 = MinusNode::SUPPLIER.make_shared();
  auto node2 = MinusNode::SUPPLIER.make_shared();
  ASSERT_EQ(MinusNode::SUPPLIER, node1->supplier());
  ASSERT_EQ(node1->supplier(), node2->supplier());
  ASSERT_NE(dynamic_cast<MinusNode *>(node1.get()), nullptr);
}

TEST(Supplier, Inequality) {
  ASSERT_NE(MinusNode::SUPPLIER, IntegerNode::SUPPLIER);
  auto minus_node = MinusNode::SUPPLIER.make_shared();
  auto integer_node = IntegerNode::SUPPLIER.make_shared();
  ASSERT_NE(minus_node->supplier(), integer_node->supplier());
}
