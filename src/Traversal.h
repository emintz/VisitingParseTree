/*
 * Traversal.h
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
 * @file Traversal.h
 *
 * @brief Basic depth-first in-order tree traversal
 */
#ifndef SRC_TRAVERSAL_H_
#define SRC_TRAVERSAL_H_

#include <memory>

#include "NodeAction.h"
#include "TraversalStatus.h"
#include "VoidFunction.h"

namespace VisitingParseTree {

/*
 * Base class for all traversals. Note that T must be a subtype
 * of Node. This is not yet enforced.
 *
 * TODO: enforce
 */
/**
 * @brief Basic depth-first in-order tree traversal for \t Node and its
 *        subclasses.
 *
 * A traversal recurses a parse tree depth first, entering a node, traversing
 * its children, then exiting the previously entered node. It is bound to
 * the following actions:
 *
 *   - Node entry, a \c NodeAction that the traversal applies to a
 *     newly entered node.
 *   - Before descent, a \c VoidFinction that the traversal invokes
 *     before it traverses the entered node's children
 *   - After ascent, a \c VoidFunction that the traversal invokes
 *     after it traverses the entered node's children
 *   - Node exit, a \c NodeAction that the traversal applies after
 *     it processes a node's children.
 *
 * Note that the traversal applies the node entry and node exit actions
 * to a leaf node, but \b does \b not invoke the before decent or after ascent
 * action. Similarly, the traversal \b does \b not invoke the descent
 * action before processing the root, nor the ascent action afterward.
 *
 * @tparam T node type being traversed. All traversed nodes must inherit
 *         this class directly or indirectly
 */
template <typename T> class Traversal : public NodeAction<T> {
  NodeAction<T>& on_entry_;
  NodeAction<T>& on_exit_;
  VoidFunction& after_descent_;
  VoidFunction& before_ascent_;

public:

  /**
   * Destructor
   */
  virtual ~Traversal() = default;

  /**
   * Creates a \c Traversal bound to the specified action
   *
   * @param on_entry applied to a newly entered node
   * @param on_exit applied after traversing a node's children
   * @param after_descent invoked between node entry application and
   *        traversing the node's children. Not invoked on leaf nodes.
   * @param before_ascent applied after processing a node's children and
   *        before invoking the exit action on the parent node. Not
   *        invoked on leaf nodes.
   */
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

  /**
   * @brief Processes this node and its children
   *
   * Recursive descent that processes a tree of \c Node<T>, depth-first,
   * in-order. Note that this method is recursive.
   *
   * @param node current node to process.
   * @return status that governs the traversal
   *
   * \see TraversalStatus for return value semantics
   */
  virtual TraversalStatus operator() (std::shared_ptr<T> node) override {
    auto status = on_entry_(node);
    if (TraversalStatus::CONTINUE == status && node->has_children()) {
      after_descent_();
      status = node->for_each_child(*this);
      before_ascent_();
    }
    if (TraversalStatus::CANCEL != status) {
      status = on_exit_(node);
    }
    return TraversalStatus::BYPASS_CHILDREN != status
        ? status
        : TraversalStatus::CONTINUE;
  }
};

} /* namespace VisitingParseTree */

#endif /* SRC_TRAVERSAL_H_ */
