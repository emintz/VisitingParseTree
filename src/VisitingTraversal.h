/*
 * VisitingTraversal.h
 *
 *  Created on: Jul 20, 2025
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
 * @file VisitingTraversal.h
 *
 * A tree traversal that applies visitors to encountered nodes
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
/**
 * @brief traverses a tree of \c Host<T> applying entry and exit
 *        visitors to all encountered nodes
 *
 * @tparam T node class, which must inherit \c Host<T>
 */
template <typename T> class VisitingTraversal {
  VisitingAction<T> on_entry_;
  VisitingAction<T> on_exit_;

  Traversal<T> traversal_;

public:

  /**
   * Constructor
   *
   * @param on_entry \c Visitor to apply on node entry. Does nothing
   *                 if \c NULL.
   * @param on_exit \c Visitor to apply on node exit. Does nothing if
   *                \c NULL.
   * @param before_descent action to take after applying the entry visitor
   *                       and before processing the first child.
   * @param after_ascent action to take between processing a node's last
   *                     and applying the exit visitor.
   */
  VisitingTraversal(
      Visitor *on_entry,
      Visitor *on_exit,
      VoidFunction& before_descent = VacuousVoidFunction::INSTANCE,
      VoidFunction& after_ascent = VacuousVoidFunction::INSTANCE) :
          on_entry_(on_entry),
          on_exit_(on_exit),
          traversal_(
              on_entry_,
              on_exit_,
              before_descent,
              after_ascent) {
  }

  virtual ~VisitingTraversal() = default;

  /**
   * @brief traverses a tree
   *
   * @param root start of the traversal; root of the traversed tree.
   *             Note that a traversal can start at any node; the
   *             starting node could be an interior node of a containing
   *             tree.
   *
   * @return traversal status
   */
  virtual TraversalStatus operator() (std::shared_ptr<T> root) {
    return traversal_(root);
  }
};

} /* namespace VisitingParseTree */

#endif /* SRC_VISITINGTRAVERSAL_H_ */
