/*
 * Attributes.cpp
 *
 *  Created on: Aug 19, 2025
 *      Author: Eric Mintz
 *
 * Test attribute manipulation
 */


#include <memory>
#include <string>
#include <vector>

#include "gtest/gtest.h"

#include "AttributeFunction.h"
#include "AttributedTestNode.h"
#include "GatherAttributes.h"
#include "TestAttribute.h"

using namespace std;
using namespace VisitingParseTree;

TEST(Attributes, Empty) {
  auto node = AttributedTestNode::SUPPLIER.make_shared();
  auto gather = GatherAttributes();
  ASSERT_EQ(0, node->attribute_count());
  ASSERT_FALSE(node->has(TestAttribute::BIRTH_DATE));
  ASSERT_FALSE(node->has(TestAttribute::NAME));
  ASSERT_TRUE(node->get(TestAttribute::BIRTH_DATE).empty());
  ASSERT_TRUE(node->get(TestAttribute::NAME).empty());
  node->for_all_attributes(gather.reset());
  ASSERT_TRUE(gather.attributes().empty());
}

TEST(Attributes, Vacuous_Add) {
  auto node = AttributedTestNode::SUPPLIER.make_shared();
  ASSERT_EQ(node, node->set(TestAttribute::NAME, ""));
  ASSERT_FALSE(node->has(TestAttribute::BIRTH_DATE));
  ASSERT_FALSE(node->has(TestAttribute::NAME));
  ASSERT_EQ("", node->get(TestAttribute::NAME));
}

TEST(Attributes, Add) {
  auto node = AttributedTestNode::SUPPLIER.make_shared();
  auto gather = GatherAttributes();
  ASSERT_EQ(node, node->set(TestAttribute::NAME, "Unary Test"));
  ASSERT_EQ(1, node->attribute_count());
  ASSERT_FALSE(node->has(TestAttribute::BIRTH_DATE));
  ASSERT_TRUE(node->has(TestAttribute::NAME));
  ASSERT_STREQ("Unary Test", node->get(TestAttribute::NAME).c_str());
  ASSERT_TRUE(node->get(TestAttribute::BIRTH_DATE).empty());
  node->for_all_attributes(gather.reset());
  ASSERT_EQ(1, gather.attributes().size());
  GatherAttributes::Entry binary_test = {&TestAttribute::NAME, "Unary Test"};
  ASSERT_EQ(binary_test, gather.attributes()[0]);

  ASSERT_EQ(node, node->set(TestAttribute::NAME, "Binary Test"));
  ASSERT_EQ(1, node->attribute_count());
  ASSERT_FALSE(node->has(TestAttribute::BIRTH_DATE));
  ASSERT_TRUE(node->has(TestAttribute::NAME));
  ASSERT_STREQ("Binary Test", node->get(TestAttribute::NAME).c_str());
  ASSERT_TRUE(node->get(TestAttribute::BIRTH_DATE).empty());
  node->for_all_attributes(gather.reset());
  ASSERT_EQ(1, gather.attributes().size());
  GatherAttributes::Entry unary_test = {&TestAttribute::NAME, "Binary Test"};
  ASSERT_EQ(unary_test, gather.attributes()[0]);
  ASSERT_EQ(node, node->erase(TestAttribute::NAME));
  ASSERT_EQ(0, node->attribute_count());
  ASSERT_TRUE(node->get(TestAttribute::BIRTH_DATE).empty());
  ASSERT_TRUE(node->get(TestAttribute::NAME).empty());

  ASSERT_EQ(0, node->attribute_count());

  ASSERT_EQ(node, node->set(TestAttribute::NAME, "Quaternary Test"));
  ASSERT_EQ(1, node->attribute_count());
  ASSERT_FALSE(node->has(TestAttribute::BIRTH_DATE));
  ASSERT_TRUE(node->has(TestAttribute::NAME));
  ASSERT_STREQ("Quaternary Test", node->get(TestAttribute::NAME).c_str());

  ASSERT_EQ(node, node->set(TestAttribute::NAME, "Trinity Test"));
  ASSERT_EQ(1, node->attribute_count());
  ASSERT_FALSE(node->has(TestAttribute::BIRTH_DATE));
  ASSERT_TRUE(node->has(TestAttribute::NAME));
  ASSERT_TRUE(node->get(TestAttribute::BIRTH_DATE).empty());
  ASSERT_STREQ("Trinity Test", node->get(TestAttribute::NAME).c_str());

  node->set(TestAttribute::BIRTH_DATE, "July 16, 1945");
  ASSERT_EQ(2, node->attribute_count());
  ASSERT_TRUE(node->has(TestAttribute::BIRTH_DATE));
  ASSERT_TRUE(node->has(TestAttribute::NAME));
  ASSERT_STREQ("July 16, 1945", node->get(TestAttribute::BIRTH_DATE).c_str());
  ASSERT_STREQ("Trinity Test", node->get(TestAttribute::NAME).c_str());
  GatherAttributes::Entry expected_trinity_date = {&TestAttribute::BIRTH_DATE, "July 16, 1945"};
  GatherAttributes::Entry expected_trinity_name = {&TestAttribute::NAME, "Trinity Test"};
  node->for_all_attributes(gather.reset());
  ASSERT_EQ(expected_trinity_date, gather.attributes()[0]);
  ASSERT_EQ(expected_trinity_name, gather.attributes()[1]);
}

TEST(Attributes, Copy) {
  auto destination = AttributedTestNode::SUPPLIER.make_shared();
  auto source = AttributedTestNode::SUPPLIER.make_shared();
  source->set(TestAttribute::BIRTH_DATE, "July 16, 1945");
  source->set(TestAttribute::NAME, "Trinity Test");
  auto populated = source->copy_attributes_to(destination);
  ASSERT_EQ(destination, populated);
  ASSERT_EQ(2, source->attribute_count());
  ASSERT_TRUE(source->has(TestAttribute::BIRTH_DATE));
  ASSERT_TRUE(source->has(TestAttribute::NAME));
  ASSERT_STREQ("July 16, 1945", source->get(TestAttribute::BIRTH_DATE).c_str());
  ASSERT_STREQ("Trinity Test", source->get(TestAttribute::NAME).c_str());
  ASSERT_EQ(2, destination->attribute_count());
  ASSERT_TRUE(destination->has(TestAttribute::BIRTH_DATE));
  ASSERT_TRUE(destination->has(TestAttribute::NAME));
  ASSERT_STREQ("July 16, 1945", destination->get(TestAttribute::BIRTH_DATE).c_str());
  ASSERT_STREQ("Trinity Test", destination->get(TestAttribute::NAME).c_str());
}
