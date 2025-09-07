/*
 * AttrNodePrinter.cpp
 *
 *  Created on: Sep 6, 2025
 *      Author: Eric Mintz
 */

#include <vector>

#include "AttributeFunction.h"
#include "AttrNodePrinter.h"
#include "NodeAction.h"
#include "Traversal.h"
#include "VoidFunction.h"

namespace VisitingParseTree {

namespace TreePrinter {

class Level {
  friend class AscendAction;
  friend class Context;
  friend class DescendAction;
  const int node_count_;  // Number of nodes in this level
  int node_index_;  // 0-based node index
  bool at_top_;

  void set_top(bool at_top) {
    at_top_ = at_top;
  }

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
  Level(int node_count) :
    node_count_(node_count),
    node_index_(0),
    at_top_(true) {
  }

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
