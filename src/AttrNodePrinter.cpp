/*
 * AttrNodePrinter.cpp
 *
 *  Created on: Sep 6, 2025
 *      Author: Eric Mintz
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

/**
 * @file AttrNodePrinter.cpp
 *
 * @brief Pretty print logic
 */

#include <vector>

#include "AttributeFunction.h"
#include "AttrNodePrinter.h"
#include "NodeAction.h"
#include "Traversal.h"
#include "VoidFunction.h"

namespace VisitingParseTree {

namespace TreePrinter {

/**
 * Tracks state during recursive descent
 *
 * Level-specific state must be preserved as the print traversal
 * descends. Level-specific state includes
 *
 * * Top of stack
 * * Number of nodes at this level
 * * Current position
 *
 * The state determines how to prefix the current node.
 *
 * Note that application code \b SHOULD not use this class, and that
 * all logic is private to prevent meddling.
 */
class Level {
  friend class AscendAction;
  friend class Context;
  friend class DescendAction;
  const int node_count_;  /** Number of nodes in this level */
  int node_index_;  /** 0-based node index */
  bool at_top_; /** \c true if and only if this is the top level */

  /**
   * Sets the top of stack indicator. The indicator should be
   * set \c true
   *
   * * on construction, where the new instance is placed on the stack
   * * when a stack pop reveals this \c Level
   *
   * @param at_top[in] \c true if and only if this \c Level
   *                   is at the top of the \c Level stack.
   */
  void set_top(bool at_top) {
    at_top_ = at_top;
  }

  /**
   * Prints a print level prefix to the specified \c stream
   *
   * @param stream receives pretty printed output
   */
  void print_prefix(std::ostream& stream) {
    if (at_top_) {
      ++node_index_;
      stream << " +--";
    } else {
      stream <<
          (node_index_ < node_count_
              ? " |  "
              : "    ");
    }
  }

public:
  /**
   * Constructor
   *
   * @param node_count the number of nodes at this traversal level
   */
  Level(int node_count) :
    node_count_(node_count),
    node_index_(0),
    at_top_(true) {
  }

  /**
   * Move constructor
   *
   * @param from source
   */
  Level(Level&& from) :
    node_count_(std::move(from.node_count_)),
    node_index_(std::move(from.node_index_)),
    at_top_(std::move(from.at_top_)) {
  }
};


class Context {
  friend class AscendAction;
  friend class VisitingParseTree::AttrNodePrinter;
  friend class DescendAction;
  friend class OnEntry;
  std::vector<Level> levels_;
  int current_child_count_;
  Context() :
    current_child_count_(0) {
  }

  void descend() {
    if (!levels_.empty()) {
      levels_.back().set_top(false);
    }
    levels_.emplace_back(current_child_count_);
  }

  void ascend() {
    levels_.pop_back();
    if (!levels_.empty()) {
      levels_.back().set_top(true);
    }
  }

  void current_child_count(int child_count) {
    current_child_count_ = child_count;
  }

public:
  std::ostream&  operator<< (std::ostream& stream) {
    for (
        auto iter = levels_.begin();
        iter != levels_.end();
        ++iter) {
      iter->print_prefix(stream);
    }
    return stream;
  }
};

std::ostream& operator<<(std::ostream& stream, Context& context) {
  return context.operator<<(stream);
}

class DescendAction : public VoidFunction {
  friend class VisitingParseTree::AttrNodePrinter;
  Context& context_;
  DescendAction(Context& context) :
    context_(context) {
  }

public:
  virtual void operator() () override {
    context_.descend();
  }
};

class AttributePrinter : public AttributeFunction {
  friend class OnEntry;

  std::ostream& stream_;

  AttributePrinter(std::ostream& stream) :
    stream_(stream) {
  }

public:
  virtual void operator() (
      const Attribute* const& attribute,
      const std::string& value) override {
    stream_ << attribute->name() << "->" << value << " ";
  }
};

class AscendAction : public VoidFunction {
  friend class VisitingParseTree::AttrNodePrinter;
  Context& context_;
  AscendAction(Context& context) :
    context_(context) {
  }

public:
  virtual void operator() () override {
    context_.ascend();
  }
};

class OnEntry : public NodeAction<BaseAttrNode> {
  friend class VisitingParseTree::AttrNodePrinter;
  Context& context_;
  std::ostream& stream_;

  OnEntry(Context& context, std::ostream& stream) :
    context_(context),
    stream_(stream) {
  }
public:
  virtual TraversalStatus operator()(std::shared_ptr<BaseAttrNode> node) override {
    context_.current_child_count(node->child_count());
    stream_ << context_ << node->type_name() << " [";
    AttributePrinter attribute_printer(stream_);
    node->for_all_attributes(attribute_printer);

    stream_ << ']' << std::endl;
    return TraversalStatus::CONTINUE;
  }
};

class OnExit : public NodeAction<BaseAttrNode> {
  friend class VisitingParseTree::AttrNodePrinter;
  OnExit() = default;
public:
  virtual TraversalStatus operator()(std::shared_ptr<BaseAttrNode> node) {
    return TraversalStatus::CONTINUE;
  }
};

} /* namespace TreePrinter */

using namespace TreePrinter;


void AttrNodePrinter::print(std::shared_ptr<BaseAttrNode> root, std::ostream& output_stream) {
  Context context;
  DescendAction descend_action(context);
  AscendAction ascend_action(context);
  OnEntry on_entry(context, output_stream);
  OnExit on_exit;
  Traversal traversal(on_entry, on_exit, descend_action, ascend_action);
  traversal(root);

}

} /* namespace VisitingParseTree */
