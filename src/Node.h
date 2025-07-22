/*
 * Node.h
 *
 *  Created on: Jul 19, 2025
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

#ifndef NODE_H_
#define NODE_H_

#include "IllegalOnRoot.h"
#include "NodeAction.h"
#include "TraversalStatus.h"
#include "TreeCorruptError.h"

#include <concepts>
#include <iterator>
#include <memory>
#include <utility>
#include <vector>

namespace VisitingParseTree {

template <typename T> class Supplier;

/*
 * Base class of all nodes. Note that implementations MUST
 * be either abstract or concrete. Concrete superclasses
 * are FORBIDDEN.
 */
template <typename T> class Node :
    public std::enable_shared_from_this<T> {
//  static_assert(std::is_base_of_v<Node, T> == true);
  Node(Node&) = delete;
  Node(const Node&) = delete;
  Node& operator=(Node&) = delete;
  Node& operator=(const Node&) = delete;

  std::weak_ptr<T> parent_;
  std::vector<std::shared_ptr<T>> children_;

  void disconnect_from_parent(
      typename std::vector<std::shared_ptr<T>>::const_iterator position) {
    parent_->children_.erase(position);
    parent_.reset();
  }


protected:
  Node() :
    parent_(),
    children_() {
  }

public:
  virtual ~Node() {
  }

  std::shared_ptr<T> append_child(Supplier<T>& supplier) {
    auto new_child = supplier.make_shared();
    children_.push_back(new_child);
    new_child->parent_ = std::enable_shared_from_this<T>::weak_from_this();
    return new_child;
  }

  std::shared_ptr<T> append_sibling(Supplier<T>& supplier) {
    if (!parent_.use_count()) {
      throw IllegalOnRoot("Cannot append a sibling to a root node.");
    }
    return  parent()->append_child(supplier);
  }

  /*
   * Returns the child at the specified index or an empty
   * pointer if there is none.
   *
   *
   * Parameters:
   * ----------
   *
   * Name                     Contents
   * -----------------        --------------------------------------------
   * index                    Child index, which SHOULD be in
   *                          [0 .. child_count)
   */

  std::shared_ptr<T> child(size_t index) {
    return index < child_count()
        ? children_[index]
        : std::shared_ptr<T>();
  }

  /*
   * Returns the number of this node's children. Will return 0
   * for a leaf node.
   */
  inline size_t child_count(void) const {
    return children_.size();
  }

  /*
   * Detaches this node from its parent retaining
   * its children. Does nothing when called on a
   * root node.
   */
  void detach() {
    if (parent_.use_count()) {
      disconnect_from_parent(find_in_parent());
    }
  }

  /*
   * Detaches this node from its parent, moving its
   * children into its former location in the child
   * vector.
   *
   */
  void excise(void) {
    if (parent_.use_count()) {
      auto my_parent = parent();
      auto it = find_in_parent();
      auto inserted_at = my_parent->children_.insert(
          std::next(it), children_.begin(), children_.end());
      children_.clear();
      disconnect_from_parent(std::prev(inserted_at));
    } else {
      throw IllegalOnRoot(
          "Cannot invoke excise() on a root node");
    }
  }

  /*
   * Finds this node in its parent child vector.
   *
   * Returns: an iterator pointing to this node
   *
   * Throws: IllegalOnRoot if invoked on a root node
   *         TreeCorruptError if this node is not a root
   *         and is not in its parent's child vector
   */
  typename std::vector<std::shared_ptr<T>>::iterator
      find_in_parent(void) {
    auto my_parent = parent();
    if (!my_parent) {
      throw IllegalOnRoot(
          "Cannot find a root in its parent.");
    }
    for (auto it = my_parent->children_.begin();
        it != my_parent->children_.end();
        ++it) {
      if (same_node_as(*it)) {
        return it;
      }
    }
    throw TreeCorruptError("Child not found in parent.");
  }

  /*
   * Applies the specified action to this node's children.
   * Application is in order. Note that the action can
   * cancel the traversal.
   *
   * Parameters:
   * ----------
   *
   * Name                     Contents
   * -----------------        --------------------------------------------
   * action                   Action to apply.
   *
   * Returns: TraversalStatus::CANCEL when the action cancels traversal;
   *          TraversalStatus::CONTINUE otherwise. Note that the invocation
   *          NEVER returns TraversalStatus::BYPASS_CHILDREN even when
   *          the traversal does so, as bypassing children is internal
   *          to the traversal.
   */
  TraversalStatus for_each_child(NodeAction<T>& action) {
    auto result = TraversalStatus::CONTINUE;

    for (auto current = children_.begin();
        TraversalStatus::CANCEL != result && current != children_.end();
        ++current) {
      if (!(*current)) {
        throw TreeCorruptError("NULL child node encountered.");
      }
      result = action(*current);
    }

    return TraversalStatus::BYPASS_CHILDREN == result
        ? TraversalStatus::CONTINUE
        : result;
  }


  inline bool has_children(void) const {
    return !children_.empty();
  }

  /*
   * A convenience method that returns true if and only if
   * this node is a leaf, i.e. if it has no children.
   * Equivalent to 0 == child_count()
   */
  inline bool is_leaf(void) const {
    return children_.empty();
  }

  /*
   * Returns true if and only if this node is at the
   * root of its containing tree. Equivalent to
   * ! static_cast<bool>(parent()).
   */
  inline bool is_root(void) const {
    return !parent_.use_count();
  }

  virtual bool operator==(const std::shared_ptr<T>& that) final {
    return same_node_as(that);
  }

  virtual bool operator!=(const std::shared_ptr<T>& that) final {
    return !same_node_as(that);
  }

  /*
   * Returns a shared pointer to this node's parent. The pointer
   * will be empty if this is a root node.
   */
  inline typename std::shared_ptr<T> parent(void) {
    return parent_.lock();
  }

  /*
   * Tests if the specified node is exactly the same instance as
   * this node.
   *
   * Parameters:
   * ----------
   *
   * Name                     Contents
   * -----------------        --------------------------------------------
   * that                     Points to the node to compare with this.
   *                          Note that the method returns false if
   *                          that is empty.
   */
  inline bool same_node_as(const std::shared_ptr<T> that) const {
    return this == that.get();
  }

  /*
   * Returns this node type's supplier. To be implemented
   * ONLY by concrete classes.
   */
  virtual Supplier<T>& supplier(void) = 0;

  inline const char *type_name(void) {
    return supplier().class_name();
  }
};

}  /* namespace VisitingParseTree */

#endif /* NODE_H_ */
