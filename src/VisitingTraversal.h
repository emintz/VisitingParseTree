/*
 * VisitingTraversal.h
 *
 *  Created on: Jul 20, 2025
 *      Author: Eric Mintz
 */

#ifndef SRC_VISITINGTRAVERSAL_H_
#define SRC_VISITINGTRAVERSAL_H_

#include "VisitingAction.h"
#include "Traversal.h"
#include "VacuousVoidFunction.h"
#include "Visitor.h"
#include "VoidFunction.h"

#include <memory>

namespace VisitingParseTree {

/*
 * Convenience class that traverses a tree, applying
 * entry and exit visitors, and optional notifying
 * the caller of descent and ascent.
 *
 * Note that T must extend Host. This is not yet enforced.
 *
 * TODO: enforce.
 */
template <typename T> class VisitingTraversal {
  VisitingAction<T> on_entry_;
  VisitingAction<T> on_exit_;

  Traversal<T> traversal_;

public:

  VisitingTraversal(
      Visitor *on_entry,
      Visitor *on_exit,
      VoidFunction& after_descent = VacuousVoidFunction::INSTANCE,
      VoidFunction& before_ascent = VacuousVoidFunction::INSTANCE) :
          on_entry_(on_entry),
          on_exit_(on_exit),
          traversal_(
              on_entry_,
              on_exit_,
              after_descent,
              before_ascent) {

  }

  virtual TraversalStatus operator() (std::shared_ptr<T> root) {
    return traversal_(root);
  }
};

} /* namespace VisitingParseTree */

#endif /* SRC_VISITINGTRAVERSAL_H_ */
