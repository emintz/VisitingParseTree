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

#include <algorithm>
#include <concepts>
#include <iterator>
#include <memory>
#include <utility>
#include <vector>

#include "BaseNode.h"
#include "IllegalOperation.h"
#include "IllegalOnRoot.h"
#include "NodeAction.h"
#include "TraversalStatus.h"
#include "TreeCorruptError.h"

namespace VisitingParseTree {

template <typename T> class Supplier;

/*
 * Base class of all nodes. Note that implementations MUST
 * be either abstract or concrete. Concrete superclasses
 * are FORBIDDEN.
 */
template <typename T> class Node :
    public BaseNode,
    public std::enable_shared_from_this<T> {
//  static_assert(std::is_base_of_v<BaseNode, T>);
  Node(Node&) = delete;
  Node(const Node&) = delete;
  Node& operator=(Node&) = delete;
  Node& operator=(const Node&) = delete;

  std::weak_ptr<T> parent_;
  std::vector<std::shared_ptr<T>> children_;

  /** Expands this nodes child vector to hold additional children
   *
   */
  void accommodate_additional_children(size_t number_of_additional_children) {
    children_.reserve(children_.size() + number_of_additional_children);
  }

  /* Disconnect this node from its parent.
   *
   * Disconnects this node from its parent. Note that
   * this node **must** have a parent for this method to
   * work properly.
   *
   * Parameters:
   * ----------
   *
   * Name                     Contents
   * -----------------        --------------------------------------------
   * position                 An iterator that points to this node in
   *                          the parent's child list. Cannot be NULL.
   */
  void disconnect_from_parent(
      std::vector<std::shared_ptr<T>>::const_iterator position) {
    auto p = parent();
    p->children_.erase(position);
    parent_.reset();
  }


protected:
  Node() = default;

  /* Add to a method's parameter list to prevent public access
   *
   * This is typically done to hide constructors from application
   * code without hiding them from std::make_shared;
   */
  enum class forbid_public_access { here } ;

  // Inserts a sequence of nodes immediately before a specified
  // location in this node's child vector
  //
  // Parameters:
  // ----------
  //
  // Name                     Contents
  // -----------------        --------------------------------------------
  // insert_start             The start point for the insert. Children
  //                          are inserted **before** the specified
  //                          location.
  // to_insert                Nodes to be inserted. Nodes in the list
  //                          **MUST** be roots (this is not checked),
  //                          and will become this node's children.
  //
  // Return: an iterator pointing to the first inserted element.
  std::vector<std::shared_ptr<T>>::iterator insert_before(
      std::vector<std::shared_ptr<T>>::iterator insert_start,
      const std::vector<std::shared_ptr<T>>& to_insert) {
    const auto w = std::enable_shared_from_this<T>::weak_from_this();
    std::for_each(to_insert.begin(), to_insert.end(), [w](std::shared_ptr<T> t) { t->parent_ = w; });
    auto p = children_.insert(insert_start, to_insert.begin(), to_insert.end());
    return p;
  }

public:

  virtual ~Node() = default;

  /* Appends the specified node to this node's child list.
   *
   * Parameters:
   * ----------
   *
   * Name                     Contents
   * -----------------        --------------------------------------------
   * new_child                The node to append
   *
   * Return: the newly appended child. Note that its parent will be
   * this.
   */
  std::shared_ptr<T> append_child(std::shared_ptr<T> new_child) {
    children_.push_back(new_child);
    new_child->parent_ = std::enable_shared_from_this<T>::weak_from_this();
    return new_child;
  }

  /* Adds a newly allocated node as a child
   *
   * Gets a new node from the specified supplier and appends
   * it to this node's child list.
   *
   * Parameters:
   * ----------
   *
   * Name                     Contents
   * -----------------        --------------------------------------------
   * supplier                 The Supplier (i.e. factory) to provide the
   *                          new child node.
   *
   * Return: the newly appended child. Note that its parent will be
   * this.
   */
  std::shared_ptr<T> append_child(Supplier<T>& supplier) {
    return append_child(supplier.make_shared());
  }

  /* Appends the specified node as a child of this node's parent.
   *
   * Note that this node MUST NOT be a root node.
   *
   * Parameters:
   * ----------
   *
   * Name                     Contents
   * -----------------        --------------------------------------------
   * supplier                 The Supplier (i.e. factory) to provide the
   *                          new child node.
   *
   * Return: the newly appended child.
   */
  std::shared_ptr<T> append_sibling(std::shared_ptr<T> new_sibling) {
    if (auto p = parent()) {
      return p->append_child(new_sibling);
    } else {
      throw IllegalOnRoot("Cannot append a sibling to a root node.");
    }
  }

  /* Appends a newly created child as this node's youngest sibling
   *
   * Gets a new node from the specified supplier and appends
   * it to the child list of this node's parent. Note that
   * this node MUST NOT be a root node.
   *
   * Parameters:
   * ----------
   *
   * Name                     Contents
   * -----------------        --------------------------------------------
   * supplier                 The Supplier (i.e. factory) to provide the
   *                          new child node.
   *
   * Return: the newly appended child.
   */
  std::shared_ptr<T> append_sibling(Supplier<T>& supplier) {
    if (auto p = parent()) {
      return p->append_child(supplier);
    } else {
      throw IllegalOnRoot("Cannot append a sibling to a root node.");
    }
  }

  /* Retrieves the child at the specified index
   *
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
   *
   * Returns: a shared_ptr to the child if index is valid, an
   *          a vacuous (i.e. empty) shared_ptr otherwise.
   */

  std::shared_ptr<T> child(size_t index) {
    return 0 <= index && index < child_count()
        ? children_[index]
        : std::shared_ptr<T>();
  }

  /* This node's child count
   *
   * Returns the number of this node's children. Will return 0
   * for a leaf node.
   */
  size_t child_count() const {
    return children_.size();
  }

  /* Detaches this node and its children from the containing tree
   *
   * Detaches this node from its parent retaining
   * its children. Does nothing when called on a
   * root node.
   */
  void detach() {
    if (auto p = parent()) {
      disconnect_from_parent(find_in_parent());
    }
  }

  // Disconnects all of this node's children and moves them
  // into the specified destination. The result is a vector
  // of unrelated root nodes.
  //
  // Return: a vector containing the disconnected children.
  std::vector<std::shared_ptr<T>> disconnect_all_children() {
    std::vector<std::shared_ptr<T>> destination(children_);
    children_.clear();
    std::for_each(
        destination.begin(),
        destination.end(),
        [](std::shared_ptr<T>& n){ n->parent_.reset(); });
    return destination;
  }

  /* Excises this node
   *
   * Detaches this node from its parent, moving its
   * children into its former location in the child
   * vector.
   *
   */
  void excise() {
    if (auto my_parent = parent()) {
      if (has_children()) {
        size_t child_count = children_.size();
        my_parent->accommodate_additional_children(child_count);
        auto insert_point = find_in_parent();
        std::vector<std::shared_ptr<T>> children_to_promote = disconnect_all_children();
        my_parent->insert_before(insert_point, children_to_promote);
      }
      detach();
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
  std::vector<std::shared_ptr<T>>::iterator find_in_parent() {
    auto my_parent = parent();
    if (!my_parent) {
      throw IllegalOnRoot(
          "Cannot find a root in its parent.");
    }
    for (auto it = my_parent->children_.begin();
        it != my_parent->children_.end();
        ++it) {
      if (this == (*it).get()) {
        return it;
      }
    }
    throw TreeCorruptError("Child not found in parent.");
  }

  /* Applies an action to children
   *
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


    bool has_children() const {
    return !children_.empty();
  }

  /*
   * A convenience method that returns true if and only if
   * this node is a leaf, i.e. if it has no children.
   * Equivalent to 0 == child_count()
   */
  bool is_leaf() const {
    return children_.empty();
  }

  /*
   * Returns true if and only if this node is at the
   * root of its containing tree. Equivalent to
   * ! static_cast<bool>(parent()).
   */
  bool is_root() const {
    return !parent_.lock();
  }

  /*
   * Returns a shared pointer to this node's parent. The pointer
   * will be empty if this is a root node.
   */
  std::shared_ptr<T> parent() {
    return parent_.lock();
  }

  /*
   * Returns this node type's supplier. To be implemented
   * ONLY by concrete classes.
   */
  virtual Supplier<T>& supplier() = 0;

  const std::string& type_name() {
    return supplier().class_name();
  }
};

}  /* namespace VisitingParseTree */

#endif /* NODE_H_ */
