/*
 * TestTrees.cpp
 *
 *  Created on: Sep 3, 2025
 *      Author: Eric Mintz
 */

#include "TestTrees.h"

#include "IntegerNode.h"
#include "DivNode.h"
#include "MinusNode.h"
#include "PlusNode.h"
#include "RootNode.h"
#include "TestAttribute.h"
#include "TimesNode.h"

namespace VisitingParseTree {

std::shared_ptr<BaseAttrNode> TestTrees::simple_addition() {
  return RootNode::SUPPLIER.make_shared()
      ->set(TestAttribute::SERIAL_NO, "1")
      ->append_child(PlusNode::SUPPLIER)
          ->set(TestAttribute::SERIAL_NO, "2")
              ->append_child(IntegerNode::SUPPLIER)
                  ->set(TestAttribute::SERIAL_NO, "3")
                  ->set(TestAttribute::VALUE, "137")
              ->append_sibling(IntegerNode::SUPPLIER)
                  ->set(TestAttribute::SERIAL_NO, "4")
                  ->set(TestAttribute::VALUE, "314")
          ->parent()
      ->parent();
}

std::shared_ptr<BaseAttrNode> TestTrees::bypass_on_entry() {
  return RootNode::SUPPLIER.make_shared()
      ->set(TestAttribute::SERIAL_NO, "1")
      ->append_child(PlusNode::SUPPLIER)
          ->set(TestAttribute::SERIAL_NO, "2")
              ->append_child(IntegerNode::SUPPLIER)
                ->set(TestAttribute::SERIAL_NO, "3")
                ->set(TestAttribute::VALUE, "137")
              ->append_sibling(MinusNode::SUPPLIER)
                ->set(TestAttribute::SERIAL_NO, "4")
                ->set(TestAttribute::BYPASS_CHILDREN_ON_ENTRY, "Yes")
                  ->append_child(IntegerNode::SUPPLIER)
                    ->set(TestAttribute::SERIAL_NO, "5")
                    ->set(TestAttribute::VALUE, "17")
                  ->append_sibling(IntegerNode::SUPPLIER)
                  ->set(TestAttribute::SERIAL_NO, "6")
                  ->set(TestAttribute::VALUE, "7")
              ->parent()
              ->append_sibling(IntegerNode::SUPPLIER)
                ->set(TestAttribute::SERIAL_NO, "8")
                ->set(TestAttribute::VALUE, "2")
          ->parent()
      ->parent();
}

std::shared_ptr<BaseAttrNode> TestTrees::bypass_on_exit() {
  return RootNode::SUPPLIER.make_shared()
      ->set(TestAttribute::SERIAL_NO, "1")
      ->append_child(PlusNode::SUPPLIER)
          ->set(TestAttribute::SERIAL_NO, "2")
          ->set(TestAttribute::BYPASS_CHILDREN_ON_EXIT, "Yes")
              ->append_child(IntegerNode::SUPPLIER)
                  ->set(TestAttribute::SERIAL_NO, "3")
                  ->set(TestAttribute::VALUE, "137")
              ->append_sibling(IntegerNode::SUPPLIER)
                  ->set(TestAttribute::SERIAL_NO, "4")
                  ->set(TestAttribute::VALUE, "314")
          ->parent()
      ->parent();
}

std::shared_ptr<BaseAttrNode> TestTrees::cancel_on_entry() {
  return RootNode::SUPPLIER.make_shared()
      ->set(TestAttribute::SERIAL_NO, "1")
      ->append_child(PlusNode::SUPPLIER)
        ->set(TestAttribute::SERIAL_NO, "2")
          ->append_child(IntegerNode::SUPPLIER)
            ->set(TestAttribute::SERIAL_NO, "3")
            ->set(TestAttribute::VALUE, "1")
          ->append_sibling(MinusNode::SUPPLIER)
            ->set(TestAttribute::SERIAL_NO, "4")
            ->set(TestAttribute::CANCEL_ON_ENTRY, "Yes")
              ->append_child(IntegerNode::SUPPLIER)
                ->set(TestAttribute::SERIAL_NO, "5")
                ->set(TestAttribute::VALUE, "137")
              ->append_sibling(IntegerNode::SUPPLIER)
                ->set(TestAttribute::SERIAL_NO, "6")
                ->set(TestAttribute::VALUE, "17")
              ->parent()
          ->parent()
      ->parent();
}

std::shared_ptr<BaseAttrNode> TestTrees::cancel_on_exit() {
  return RootNode::SUPPLIER.make_shared()
      ->set(TestAttribute::SERIAL_NO, "1")
      ->append_child(PlusNode::SUPPLIER)
        ->set(TestAttribute::SERIAL_NO, "2")
          ->append_child(IntegerNode::SUPPLIER)
            ->set(TestAttribute::SERIAL_NO, "3")
            ->set(TestAttribute::VALUE, "1")
          ->append_sibling(MinusNode::SUPPLIER)
            ->set(TestAttribute::SERIAL_NO, "4")
            ->set(TestAttribute::CANCEL_ON_EXIT, "Yes")
              ->append_child(IntegerNode::SUPPLIER)
                ->set(TestAttribute::SERIAL_NO, "5")
                ->set(TestAttribute::VALUE, "137")
              ->append_sibling(IntegerNode::SUPPLIER)
                ->set(TestAttribute::SERIAL_NO, "6")
                ->set(TestAttribute::VALUE, "17")
              ->parent()
          ->parent()
      ->parent();
}

std::shared_ptr<BaseAttrNode> TestTrees::addition_and_multiplication() {
  return RootNode::SUPPLIER.make_shared()
      ->set(TestAttribute::SERIAL_NO, "1")
      ->append_child(PlusNode::SUPPLIER)
          ->set(TestAttribute::SERIAL_NO, "2")
            ->append_child(IntegerNode::SUPPLIER)
                ->set(TestAttribute::SERIAL_NO, "3")
                ->set(TestAttribute::VALUE, "137")
            ->append_sibling(TimesNode::SUPPLIER)
                ->set(TestAttribute::SERIAL_NO, "4")
                ->set(TestAttribute::VALUE, "314")
                  ->append_child(IntegerNode::SUPPLIER)
                    ->set(TestAttribute::SERIAL_NO, "5")
                    ->set(TestAttribute::VALUE, "3")
                  ->append_sibling(IntegerNode::SUPPLIER)
                    ->set(TestAttribute::SERIAL_NO, "6")
                    ->set(TestAttribute::VALUE, "5")
                  ->parent()
            ->parent()
      ->parent();
}

std::shared_ptr<BaseAttrNode> TestTrees::all_operations() {
  return RootNode::SUPPLIER.make_shared()
      ->append_child(PlusNode::SUPPLIER)
          ->append_child(TimesNode::SUPPLIER)
              ->append_child(IntegerNode::SUPPLIER)
                ->set(TestAttribute::VALUE, "3")
              ->append_sibling(IntegerNode::SUPPLIER)
                ->set(TestAttribute::VALUE, "4")
          ->parent()
      ->parent()
          ->append_child(MinusNode::SUPPLIER)
              ->append_child(IntegerNode::SUPPLIER)
                ->set(TestAttribute::VALUE, "5")
              ->append_sibling(DivNode::SUPPLIER)
                  ->append_child(IntegerNode::SUPPLIER)
                    ->set(TestAttribute::VALUE, "6")
                  ->append_sibling(IntegerNode::SUPPLIER)
                    ->set(TestAttribute::VALUE, "3")
              ->parent()
          ->parent()
      ->parent()
  ->parent()
  ;
}

std::shared_ptr<BaseAttrNode> TestTrees::complex_tree() {
  return RootNode::SUPPLIER.make_shared()
      ->append_child(PlusNode::SUPPLIER)
          ->append_child(TimesNode::SUPPLIER)
              ->append_child(TimesNode::SUPPLIER)
                  ->append_child(IntegerNode::SUPPLIER)
                    ->set(TestAttribute::VALUE, "17")
                  ->append_sibling(IntegerNode::SUPPLIER)
                    ->set(TestAttribute::VALUE, "19")
                  ->parent()
              ->append_sibling(IntegerNode::SUPPLIER)
                ->set(TestAttribute::VALUE, "4")
          ->parent()
      ->parent()
          ->append_child(MinusNode::SUPPLIER)
              ->append_child(IntegerNode::SUPPLIER)
                ->set(TestAttribute::VALUE, "5")
              ->append_sibling(DivNode::SUPPLIER)
                  ->append_child(IntegerNode::SUPPLIER)
                    ->set(TestAttribute::VALUE, "6")
                  ->append_sibling(IntegerNode::SUPPLIER)
                    ->set(TestAttribute::VALUE, "3")
              ->parent()
          ->parent()
      ->parent()
  ->parent()
  ;
}

} /* namespace VisitingParseTree */
