/*
 * TestTrees.h
 *
 *  Created on: Sep 3, 2025
 *      Author: Eric Mintz
 *
 * Provides trees for testing
 */

#ifndef TESTTREES_H_
#define TESTTREES_H_

#include <memory>

#include "BaseAttrNode.h"

namespace VisitingParseTree {

class TestTrees {
  TestTrees() = default;
public:
  static std::shared_ptr<BaseAttrNode> simple_addition();
  static std::shared_ptr<BaseAttrNode> bypass_on_entry();
  static std::shared_ptr<BaseAttrNode> bypass_on_exit();
  static std::shared_ptr<BaseAttrNode> cancel_on_entry();
  static std::shared_ptr<BaseAttrNode> cancel_on_exit();
  static std::shared_ptr<BaseAttrNode> addition_and_multiplication();
  static std::shared_ptr<BaseAttrNode> all_operations();
  static std::shared_ptr<BaseAttrNode> complex_tree();
};

} /* namespace VisitingParseTree */

#endif /* TESTTREES_H_ */
