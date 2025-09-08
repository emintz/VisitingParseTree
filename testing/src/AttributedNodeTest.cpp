//============================================================================
// Name        : AttributedNodeTest.cpp
// Author      :
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>

#include "gtest/gtest.h"

#include "AttrNodePrinter.h"
#include "AttributedTestNode.h"
#include "TestAttribute.h"
#include "TestTrees.h"

static void creation(void) {
  std::cout << "Creation test" << std::endl;
  auto node = AttributedTestNode::SUPPLIER.make_shared();
  std::cout << "Created a " << node->type_name() << " instance." << std::endl;
  std::cout << "Creation test completed." << std::endl << std::endl;
}

static void attributes(void) {
  std::cout << "Attribute test" << std::endl;
  auto node = AttributedTestNode::SUPPLIER.make_shared();
  std::cout << "Created a " << node->type_name() << " instance." << std::endl;
  node->set(TestAttribute::NAME, "Eric Mintz");
  node->set(TestAttribute::BIRTH_DATE, "1948-11-27");

  std::optional<std::string> birth_date = node->get(TestAttribute::BIRTH_DATE);
  std::optional<std::string> name = node->get(TestAttribute::NAME);

  std::cout << "Node ID: " << node->id() << std::endl;
  std::cout << "Birth date: " << birth_date.value_or("MISSING").c_str() << std::endl;
  std::cout << "Name: " << name.value_or("MISSING").c_str() << std::endl;
}

static void print() {
  auto add_and_multiply = VisitingParseTree::TestTrees::addition_and_multiplication();
  VisitingParseTree::AttrNodePrinter printer;
  std::cout << "Add and multiply:" << std::endl;
  printer.print(add_and_multiply, std::cout);
  std::cout << std::endl;
  auto all_ops = VisitingParseTree::TestTrees::all_operations();
  printer.print(all_ops, std::cout);
  printer.print(VisitingParseTree::TestTrees::complex_tree(), std::cout);
}

int main() {
	std::cout << "!!!Hello World!!!" << std::endl; // prints !!!Hello World!!!
	std::cout << "Attributed node test compiled on " << __DATE__ << " at " __TIME__ << '.' << std::endl;
	testing::InitGoogleTest();
	creation();
	attributes();
	print();
	return RUN_ALL_TESTS();
}
