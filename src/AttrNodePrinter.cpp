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

/**
 * @brief Holds state during printing
 */
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

  /**
   * @brief Processes a descent
   *
   * Adjusts the context after a node has been entered and before
   * its first tree is processed. The descent processor delegates
   * to this method.
   *
   * \c descend() \b must \b not be
   * invoked after a leaf has been entered. Fortunately, this
   * behavior is baked into the traversal.
   */
  void descend() {
    if (!levels_.empty()) {
      levels_.back().set_top(false);
    }
    levels_.emplace_back(current_child_count_);
  }

  /**
   * @brief Processes an ascent
   *
   * Adjusts a context after a node's children has been processed and
   * before the node is exited. The ascent processor delegates to this method.
   *
   * \c ascend() \b must \b not be
   * invoked before exiting a leaf. Fortunately, this is baked
   * into the traversal.
   */
  void ascend() {
    levels_.pop_back();
    if (!levels_.empty()) {
      levels_.back().set_top(true);
    }
  }

  /**
   * @brief Stores the current node's number of children
   *
   * Note that in the following tree print:
   *
   *        RootNode []
   *         +--PlusNode []
   *             +--TimesNode []
   *             |   +--TimesNode []
   *             |   |   +--IntegerNode [VALUE->17 ]
   *             |   |   +--IntegerNode [VALUE->19 ]
   *             |   +--IntegerNode [VALUE->4 ]
   *             +--MinusNode []
   *                 +--IntegerNode [VALUE->5 ]
   *                 +--DivNode []
   *                     +--IntegerNode [VALUE->6 ]
   *                     +--IntegerNode [VALUE->3 ]
   *
   * \c TimesNode children are prefixed differently than \c MinusNode
   * children. That's because the \c MinusNode is the \b last child of
   * its parent \c PlusNode. The traversal uses a node's child count
   * to control this.
   *
   * @param child_count the number of children owned by the
   *        node being printed.
   */
  void current_child_count(int child_count) {
    current_child_count_ = child_count;
  }

public:
  /**
   * @brief prints this context
   *
   * Prints the prefix for the current node. Note that this operation
   * does \b not print the node itself.
   *
   * @param stream std::ostream to receive the output
   * @return \c stream, for chaining
   */
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

/**
 * @brief Standard print operation
 *
 * Invokes \c Context::operator<<(std::stream) to print a node's
 * prefix.
 *
 * @param stream receives the output
 * @param context the context to print
 * @return \b stream
 */
std::ostream& operator<<(std::ostream& stream, Context& context) {
  return context.operator<<(stream);
}

/**
 * @brief Traversal descent action
 */
class DescendAction : public VoidFunction {
  friend class VisitingParseTree::AttrNodePrinter;

  /**
   * @brief Traversal state
   *
   * \c context_ actually handles descent
   */
  Context& context_;

  DescendAction(Context& context) :
    context_(context) {
  }

public:
  /**
   * @brief invoked between entry and child processing unless the
   *        entered node is a leaf.
   *
   * Delegates to the associated \b Context. \c () must not be invoked
   * for leaves. Fortunately, tree traversals comply as a matter of course.
   */
  virtual void operator() () override {
    context_.descend();
  }
};

/**
 * @brief Ascent action
 */
class AscendAction : public VoidFunction {
  friend class VisitingParseTree::AttrNodePrinter;
  Context& context_;
  AscendAction(Context& context) :
    context_(context) {
  }

public:
  /**
   * @brief Invoked between exiting the last child node and exiting
   *        its parent.
   *
   * Delegates to the traversal context. \c () must not be invoked for
   * leaves. Fortunately, tree traversals comply as a matter of course.
   */
  virtual void operator() () override {
    context_.ascend();
  }
};

/**
 * @brief Attribute printer
 *
 * Passed to \c AttrNode::for_all_attributes() to print the invoked
 * node's attributes.
 */
class AttributePrinter : public AttributeFunction {
  friend class OnEntry;

  std::ostream& stream_;

  /**
   * @brief Constructor
   *
   * @param stream destination for the attribute values
   */
  AttributePrinter(std::ostream& stream) :
    stream_(stream) {
  }

public:
  /**
   * @brief Prints an attribute, value pair
   *
   * @param attribute attribute time
   * @param value attribute's \c std::string value
   */
  virtual void operator() (
      const Attribute* const& attribute,
      const std::string& value) override {
    stream_ << attribute->name() << "->" << value << " ";
  }
};

/**
 * @brief node entry processor
 *
 * Writes a formatted representation of the entered node to
 * an output stream
 */
class OnEntry : public NodeAction<BaseAttrNode> {
  friend class VisitingParseTree::AttrNodePrinter;

  /**
   * @brief traversal state
   */
  Context& context_;

  /**
   * @brief receives the formatted output
   */
  std::ostream& stream_;

  /**
   * @brief Constructor
   *
   * @param context traversal context
   * @param stream receives the formatted output
   */
  OnEntry(Context& context, std::ostream& stream) :
    context_(context),
    stream_(stream) {
  }
public:
  /**
   * @brief Node entry processor
   *
   * Prints the newly encountered node
   *
   * @param node the node that the traversal has entered
   * @return \c TraversalStatus::CONTINUE, always
   */
  virtual TraversalStatus operator() (
      std::shared_ptr<BaseAttrNode> node) override {
    context_.current_child_count(node->child_count());
    stream_ << context_ << node->type_name() << " [";
    AttributePrinter attribute_printer(stream_);
    node->for_all_attributes(attribute_printer);

    stream_ << ']' << std::endl;
    return TraversalStatus::CONTINUE;
  }
};

/**
 * @brief vacuous (i.e. place holder) node exit processor
 */
class OnExit : public NodeAction<BaseAttrNode> {
  friend class VisitingParseTree::AttrNodePrinter;
  OnExit() = default;
public:
  /**
   * @brief Vacuous (i.e. no-op) node exit processor
   *
   * Does nothing other than returning \c TraversalStatus::CONTINUE
   *
   * @param node exited node
   * @return \c TraversalStatus::CONTINUE always
   */
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
