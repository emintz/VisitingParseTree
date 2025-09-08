/*
 * TreeConstruction.cpp
 *
 *  Created on: Aug 19, 2025
 *      Author: Eric Mintz
 */

#include <memory>
#include <string>
#include <vector>

#include "gtest/gtest.h"

#include "AttributedTestNode.h"
#include "Gather.h"
#include "TestAttribute.h"

using namespace std;
using namespace VisitingParseTree;

TEST(TreeConstruction, AddFromSupplier) {
  auto root = AttributedTestNode::SUPPLIER.make_shared();
  Gather gather;
  ASSERT_TRUE(root);
  ASSERT_TRUE(root->is_root());
  ASSERT_TRUE(root->is_leaf());
  ASSERT_EQ(0, root->child_count());

  auto first_child = root->append_child(AttributedTestNode::SUPPLIER);
  ASSERT_TRUE(first_child);
  ASSERT_TRUE(root->is_root());
  ASSERT_FALSE(root->is_leaf());
  ASSERT_FALSE(first_child->is_root());
  ASSERT_TRUE(first_child->is_leaf());
  ASSERT_EQ(1, root->child_count());
  ASSERT_EQ(0, first_child->child_count());
  ASSERT_EQ(first_child, root->child(0));
  ASSERT_EQ(root, first_child->parent());
  root->for_each_child(gather.reset());
  ASSERT_EQ(1, gather().size());
  ASSERT_EQ(first_child, gather()[0]);

  auto second_child = first_child->append_sibling(AttributedTestNode::SUPPLIER);
  ASSERT_TRUE(second_child);
  ASSERT_TRUE(root->is_root());
  ASSERT_FALSE(root->is_leaf());
  ASSERT_FALSE(first_child->is_root());
  ASSERT_TRUE(first_child->is_leaf());
  ASSERT_FALSE(second_child->is_root());
  ASSERT_TRUE(second_child->is_leaf());
  ASSERT_TRUE(first_child->is_leaf());
  ASSERT_EQ(2, root->child_count());
  ASSERT_EQ(0, first_child->child_count());
  ASSERT_EQ(first_child, root->child(0));
  ASSERT_EQ(root, first_child->parent());
  ASSERT_EQ(0, second_child->child_count());
  ASSERT_EQ(second_child, root->child(1));
  ASSERT_EQ(root, second_child->parent());
  root->for_each_child(gather.reset());
  ASSERT_EQ(2, gather().size());
  ASSERT_EQ(first_child, gather()[0]);
  ASSERT_EQ(second_child, gather()[1]);
}

TEST(TreeConstruction, AddPreExisting) {
  auto root = AttributedTestNode::SUPPLIER.make_shared();
  auto first_child = AttributedTestNode::SUPPLIER.make_shared();
  auto second_child = AttributedTestNode::SUPPLIER.make_shared();
  Gather gather;

  ASSERT_TRUE(root);
  ASSERT_TRUE(root->is_root());
  ASSERT_TRUE(root->is_leaf());
  ASSERT_EQ(0, root->child_count());

  ASSERT_EQ(first_child, root->append_child(first_child));
  ASSERT_TRUE(root->is_root());
  ASSERT_FALSE(root->is_leaf());
  ASSERT_FALSE(first_child->is_root());
  ASSERT_TRUE(first_child->is_leaf());
  ASSERT_EQ(1, root->child_count());
  ASSERT_EQ(0, first_child->child_count());
  ASSERT_EQ(first_child, root->child(0));
  ASSERT_EQ(root, first_child->parent());
  root->for_each_child(gather.reset());
  ASSERT_EQ(1, gather().size());
  ASSERT_EQ(first_child, gather()[0]);

  ASSERT_EQ(second_child, first_child->append_sibling(second_child));
  ASSERT_TRUE(root->is_root());
  ASSERT_FALSE(root->is_leaf());
  ASSERT_FALSE(first_child->is_root());
  ASSERT_TRUE(first_child->is_leaf());
  ASSERT_FALSE(second_child->is_root());
  ASSERT_TRUE(second_child->is_leaf());
  ASSERT_TRUE(first_child->is_leaf());
  ASSERT_EQ(2, root->child_count());
  ASSERT_EQ(0, first_child->child_count());
  ASSERT_EQ(first_child, root->child(0));
  ASSERT_EQ(root, first_child->parent());
  ASSERT_EQ(0, second_child->child_count());
  ASSERT_EQ(second_child, root->child(1));
  ASSERT_EQ(root, second_child->parent());
  root->for_each_child(gather.reset());
  ASSERT_EQ(2, gather().size());
  ASSERT_EQ(first_child, gather()[0]);
  ASSERT_EQ(second_child, gather()[1]);
}

TEST(TreeConstruction, TewoLevel) {
  auto root = AttributedTestNode::SUPPLIER.make_shared();
  Gather gather;
  ASSERT_TRUE(root);
  ASSERT_TRUE(root->is_root());
  ASSERT_TRUE(root->is_leaf());
  ASSERT_EQ(0, root->child_count());

  auto level1_0_0 = root->append_child(AttributedTestNode::SUPPLIER);
  ASSERT_TRUE(level1_0_0);
  ASSERT_TRUE(root->is_root());
  ASSERT_FALSE(root->is_leaf());
  ASSERT_FALSE(level1_0_0->is_root());
  ASSERT_TRUE(level1_0_0->is_leaf());
  ASSERT_EQ(1, root->child_count());
  ASSERT_EQ(0, level1_0_0->child_count());
  ASSERT_EQ(level1_0_0, root->child(0));
  ASSERT_EQ(root, level1_0_0->parent());
  root->for_each_child(gather.reset());
  ASSERT_EQ(1, gather().size());
  ASSERT_EQ(level1_0_0, gather()[0]);

  auto level1_0_1 = level1_0_0->append_sibling(AttributedTestNode::SUPPLIER);
  ASSERT_TRUE(level1_0_1);
  ASSERT_TRUE(root->is_root());
  ASSERT_FALSE(root->is_leaf());
  ASSERT_FALSE(level1_0_0->is_root());
  ASSERT_TRUE(level1_0_0->is_leaf());
  ASSERT_FALSE(level1_0_1->is_root());
  ASSERT_TRUE(level1_0_1->is_leaf());
  ASSERT_TRUE(level1_0_0->is_leaf());
  ASSERT_EQ(2, root->child_count());
  ASSERT_EQ(0, level1_0_0->child_count());
  ASSERT_EQ(level1_0_0, root->child(0));
  ASSERT_EQ(root, level1_0_0->parent());
  ASSERT_EQ(0, level1_0_1->child_count());
  ASSERT_EQ(level1_0_1, root->child(1));
  ASSERT_EQ(root, level1_0_1->parent());
  root->for_each_child(gather.reset());
  ASSERT_EQ(2, gather().size());
  ASSERT_EQ(level1_0_0, gather()[0]);
  ASSERT_EQ(level1_0_1, gather()[1]);

  auto level2_0_0 = level1_0_0->append_child(AttributedTestNode::SUPPLIER);
  ASSERT_TRUE(level2_0_0);
  ASSERT_FALSE(level2_0_0->is_root());
  ASSERT_TRUE(level2_0_0->is_leaf());
  ASSERT_TRUE(root->is_root());
  ASSERT_FALSE(root->is_leaf());
  ASSERT_FALSE(level1_0_0->is_root());
  ASSERT_FALSE(level1_0_0->is_leaf());
  ASSERT_FALSE(level1_0_1->is_root());
  ASSERT_TRUE(level1_0_1->is_leaf());
  ASSERT_FALSE(level1_0_0->is_leaf());
  ASSERT_EQ(2, root->child_count());
  ASSERT_EQ(1, level1_0_0->child_count());
  ASSERT_EQ(level1_0_0, root->child(0));
  ASSERT_EQ(root, level1_0_0->parent());
  ASSERT_EQ(0, level1_0_1->child_count());
  ASSERT_EQ(level1_0_1, root->child(1));
  ASSERT_EQ(root, level1_0_1->parent());
  root->for_each_child(gather.reset());
  ASSERT_EQ(2, gather().size());
  ASSERT_EQ(level1_0_0, gather()[0]);
  ASSERT_EQ(level1_0_1, gather()[1]);
}

TEST(TreeConstruction, Detach) {
  auto root = AttributedTestNode::SUPPLIER.make_shared();
  auto first_child = root->append_child(AttributedTestNode::SUPPLIER);
  auto middle_child = root->append_child(AttributedTestNode::SUPPLIER);
  auto last_child = root->append_child(AttributedTestNode::SUPPLIER);
  ASSERT_EQ(3, root->child_count());
  ASSERT_EQ(first_child, root->child(0));
  ASSERT_EQ(middle_child, root->child(1));
  ASSERT_EQ(last_child, root->child(2));
  ASSERT_TRUE(first_child->is_leaf());
  ASSERT_FALSE(first_child->is_root());
  ASSERT_TRUE(middle_child->is_leaf());
  ASSERT_FALSE(middle_child->is_root());
  ASSERT_TRUE(last_child->is_leaf());
  ASSERT_FALSE(last_child->is_root());

  middle_child->detach();
  ASSERT_EQ(2, root->child_count());
  ASSERT_EQ(first_child, root->child(0));
  ASSERT_EQ(last_child, root->child(1));
  ASSERT_TRUE(first_child->is_leaf());
  ASSERT_FALSE(first_child->is_root());
  ASSERT_TRUE(middle_child->is_leaf());
  ASSERT_TRUE(middle_child->is_root());
  ASSERT_TRUE(last_child->is_leaf());
  ASSERT_FALSE(last_child->is_root());
}

TEST(TreeConstruction, Excise) {
  auto root = AttributedTestNode::SUPPLIER.make_shared()
      ->set(TestAttribute::NAME, "Root")
      ->append_child(AttributedTestNode::SUPPLIER)
        ->set(TestAttribute::NAME, "Oldest Child")
      ->append_sibling(AttributedTestNode::SUPPLIER)
        ->set(TestAttribute::NAME, "Middle Child")
        ->append_child(AttributedTestNode::SUPPLIER)
          ->set(TestAttribute::NAME, "First Grandchild")
        ->append_sibling(AttributedTestNode::SUPPLIER)
          ->set(TestAttribute::NAME, "Second Grandchild")
        ->parent()
      ->append_sibling(AttributedTestNode::SUPPLIER)
        ->set(TestAttribute::NAME, "Youngest Child")
      ->parent();
  ASSERT_STREQ("Root", root->get(TestAttribute::NAME).c_str());
  ASSERT_EQ(3, root->child_count());
  auto middle_child = root->child(1);
  ASSERT_STREQ("Middle Child", middle_child->get(TestAttribute::NAME).c_str());
  middle_child->excise();
  ASSERT_EQ(4, root->child_count());
  ASSERT_STREQ("Oldest Child", root->child(0)->get(TestAttribute::NAME).c_str());
  ASSERT_STREQ("First Grandchild", root->child(1)->get(TestAttribute::NAME).c_str());
  ASSERT_STREQ("Second Grandchild", root->child(2)->get(TestAttribute::NAME).c_str());
  ASSERT_STREQ("Youngest Child", root->child(3)->get(TestAttribute::NAME).c_str());
}
