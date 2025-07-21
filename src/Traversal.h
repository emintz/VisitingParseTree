/*
 * Traversal.h
 *
 *  Created on: Jul 20, 2025
 *      Author: Eric Mintz
 */

#ifndef SRC_TRAVERSAL_H_
#define SRC_TRAVERSAL_H_

#include "NodeAction.h"
#include "TraversalStatus.h"
#include "VoidFunction.h"

#include <memory>

namespace VisitingParseTree {

/*
 * Base class for all traversals. Note that T must be a subtype
 * of Node. This is not yet enforced.
 *
 * TODO: enforce
 */
template <typename T> class Traversal : public NodeAction<T> {
  NodeAction<T>& on_entry_;
  NodeAction<T>& on_exit_;
  VoidFunction& after_descent_;
  VoidFunction& before_ascent_;

public:

  virtual ~Traversal() {}

  Traversal(
      NodeAction<T>& on_entry,
      NodeAction<T>& on_exit,
      VoidFunction& after_descent,
      VoidFunction& before_ascent) :
          on_entry_(on_entry),
          on_exit_(on_exit),
          after_descent_(after_descent),
          before_ascent_(before_ascent) {
  }

  virtual TraversalStatus operator() (std::shared_ptr<T> node) override {
    auto status = on_entry_(node);
    if (TraversalStatus::CONTINUE == status && node->has_children()) {
      after_descent_();
      status = node->for_each_child(*this);
    }
    if (TraversalStatus::CANCEL != status) {
      before_ascent_();
      status = on_exit_(node);
    }
    return TraversalStatus::BYPASS_CHILDREN != status
        ? status
        : TraversalStatus::CONTINUE;
  }
};

} /* namespace VisitingParseTree */

#endif /* SRC_TRAVERSAL_H_ */
