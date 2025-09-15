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

/**
 * @file Node.h
 *
 * @brief The base of all node classes
 *
 * The node template is the base of the parse tree node class
 * hierarchy. It cannot be instantiated (it has a \c protected
 * constructor) so it can only serve \b as a base class.
 *
 * The \c Node template is the first in a sequence of node
 * templates, each of which adds functionality. This design
 * allows users to select the exact functionality needed
 * for their use cases.
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
/**
 * @brief The base class for all parse tree nodes
 *
 * The \c Node template implements an N-ary node that can be used
 * in parse trees. In addition to setting the tree topology,
 * it supports tree manipulations including node addition, removal,
 * and excision. It also binds node creation to suppliers, which
 * \b must subclass the \c Supplier template. This is required
 * by the node generator, another component of this project.
 *
 * @tparam T subtype. When a subtype inherits directly from the \c Node
 *         template, \c T must be set to the subtype. If, for example,
 *         \c MyNode inherits \c Node, \c T must be \c MyNode.
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

  /**
   * @brief expands this nodes child vector to hold additional children
   *
   * @param number_of_additional_children the number of new child nodes
   *        that the expanded child list must be able to hold \a without
   *        requiring expansion
   */
  void accommodate_additional_children(size_t number_of_additional_children) {
    children_.reserve(children_.size() + number_of_additional_children);
  }

  /**
   * @brief Disconnects this node and its children from its parent. Does
   *        nothing if invoked on a root.
   *
   * @param position
   */
  void disconnect_from_parent(
      std::vector<std::shared_ptr<T>>::const_iterator position) {
    if (auto p = parent()) {
      p->children_.erase(position);
      parent_.reset();
    }
  }

  /**
   * @brief Validates that a node is a root
   *
   * @param node node to validate
   * @return \c node for chaining
   *
   * \throws IllegalOperation if \c node is not a root
   */
  std::shared_ptr<T> must_be_root(std::shared_ptr<T> node) {
    if (!node->is_root()) {
      throw new IllegalOperation("Node must be a root.");
    }
    return node;
  }

protected:
  Node() = default;

  /**
   * @brief Dummy constructor parameter type for concrete generated
   *        nodes.
   *
   * A generated node's constructor will resemble:
   *
   *      protected:
   *      // ...
   *      FooNode(forbid_public_access) {  // No name --> no unused var warning
   *        // Code does not access the parameter.
   *      }
   *
   * This is a hack to forbid application code from creating
   * nodes directly while allowing concrete node classes to
   * have public constructors as required by \c std::make_shared
   * and related functions.
   */
  enum class forbid_public_access { here } ;/**< here */

  /**
   * @brief Inserts a list of nodes at a specified location
   * in this node's child vector
   *
   * The nodes to insert \b must \b not be in use, i.e. they \b cannot
   * belong to a parse tree. If they are, this method will corrupt both
   * the containing tree and the tree that contains this node.
   *
   * @param insert_start an iterator that points to the insert point.
   *        This \b must be a valid iterator pointing into \c children_.
   *        This method \b might invalidate \c instert_start. Do not
   *        use it after this method returns.
   * @param to_insert \c vector of nodes to insert. May be empty,
   *        in which case this method becomes an expensive no-op.
   * @return an iterator pointing to the first inserted element. Prefer
   *         the returned iterator to \c insert_start, as the former
   *         is guaranteed to be valid and the latter is not.
   */
  std::vector<std::shared_ptr<T>>::iterator insert_before(
      std::vector<std::shared_ptr<T>>::iterator insert_start,
      const std::vector<std::shared_ptr<T>>& to_insert) {
    const auto w = std::enable_shared_from_this<T>::weak_from_this();
    std::for_each(
        to_insert.begin(),
        to_insert.end(),
        [w](std::shared_ptr<T> t) { t->parent_ = w; });
    auto p = children_.insert(insert_start, to_insert.begin(), to_insert.end());
    return p;
  }

public:

  virtual ~Node() = default;

  /**
   * @brief Adds the specified node as this node's youngest child.
   *
   * Note that the provided node must be a root, otherwise both its
   * containing tree and the tree that contains this node become corrupt.
   * This method \b might invalidate iterators that reference
   * \c children_.
   *
   * @param new_child child node to append.
   * @return the newly appended child. Note that its parent will be
   * set to \c this.
   *
   * @return \c new_child, for chaining
   */
  std::shared_ptr<T> append_child(std::shared_ptr<T> new_child) {
    children_.push_back(new_child);
    new_child->parent_ = std::enable_shared_from_this<T>::weak_from_this();
    return new_child;
  }

  /**
   * @brief Creates a node as this node's youngest child.
   *
   * @param supplier the factory that will allocate the new
   *        child node
   * @return the newly appended child. Note that its parent will be
   *         set to \c this.
   */
  std::shared_ptr<T> append_child(Supplier<T>& supplier) {
    return append_child(supplier.make_shared());
  }

  /**
   * @brief Adds the specified node as this node's youngest
   *        sibling.
   *
   * Note the provided node will be appended as the youngest child of
   * this node's parent. This node \b must \b not be a root. The node
   * to append \b must be a root, otherwise
   * both its containing tree and the tree that contains this node
   * will become corrupt.
   *
   * @param new_sibling node to append to this node's parent
   *
   * @return the newly appended sibling (i.e. \c new_sibling) for chaining.
   *
   * \throws IllegalOnRoot if this node is a root
   * \throws IllegalOperation if \c new_sibling is \b not a root
   */
  std::shared_ptr<T> append_sibling(std::shared_ptr<T> new_sibling) {
    if (auto p = parent()) {
      return p->append_child(must_be_root(new_sibling));
    } else {
      throw IllegalOnRoot("Cannot append a sibling to a root node.");
    }
  }

  /**
   * @brief Creates a new node as this node's youngest sibling
   *
   * Gets a new node from the specified supplier and appends
   * it to the child list of this node's parent. Note that
   * this node MUST NOT be a root node.
   *
   * @param supplier the Supplier (i.e. factory) to provide the
   *                 added node

   * @return the newly appended sibling
   *
   * \throws IllegalOnRoot if this node is a root
   */
  std::shared_ptr<T> append_sibling(Supplier<T>& supplier) {
    if (auto p = parent()) {
      return p->append_child(supplier);
    } else {
      throw IllegalOnRoot("Cannot append a sibling to a root node.");
    }
  }

  /**
   * @brief Retrieves the child at the specified index
   *
   * @param index child index, which \b should be in
   *        [0 .. number of children - 1]
   *
   * @return the child when \c index is valid, a vacuous value
   *         otherwise
   */
  std::shared_ptr<T> child(size_t index) {
    return 0 <= index && index < child_count()
        ? children_[index]
        : std::shared_ptr<T>();
  }

  /**
   * Provides the number of children hald by this node
   *
   * @return the number of children as described above. Will return
   *         0 for a leaf node.
   */
  size_t child_count() const {
    return children_.size();
  }

  /**
   * @brief Detaches this node and its children from the containing tree.
   *        Does nothing when called on a root.
   *
   * This node becomes a root.
   * Its descendants (its children, their children, etc.) will be
   * unchanged and the newly liberated tree remains valid.
   */
  void detach() {
    if (auto p = parent()) {
      disconnect_from_parent(find_in_parent());
    }
  }

  /**
   * @brief Disconnects all of this node's children and makes them
   *        roots
   *
   * @return a \c vector containing the disconnected nodes. The
   *         returned \c vector will be empty when called on a
   *         leaf.
   */
  std::vector<std::shared_ptr<T>> disconnect_all_children() {
    std::vector<std::shared_ptr<T>> destination(children_);
    children_.clear();
    std::for_each(
        destination.begin(),
        destination.end(),
        [](std::shared_ptr<T>& n){ n->parent_.reset(); });
    return destination;
  }

  /**
   * @brief Creates an empty copy of this node
   *
   * Allocates and returns a new instance of this node's type.
   * The new node's comes straight from its supplier. Fields
   * will have their default values the node will have neither
   * parent or children.
   *
   * Note that "empty" is vacuous in this context, but \b might
   * be significant in subclases
   *
   * @return the newly allocated node, as described above
   */
  std::shared_ptr<T> empty_copy() {
    return supplier().make_shared();
  }

  /**
   * @brief Excises this node
   *
   * Replaces this node and its children, turning this node
   * into a root and a leaf.
   *
   * \throws IllegalOnRoot if called on a root
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

  /**
   * @brief Finds this node in its parent
   *
   * @return an iterator pointing to this node within its
   *         parent's child list.
   *
   * \throws IllegalOnRoot if invoked on a root node
   * \throws TreeCorruptError if this node is not a child of its parent
   */

  std::vector<std::shared_ptr<T>>::iterator find_in_parent() {
    auto my_parent = parent();
    if (!my_parent) {
      throw TreeCorruptError(
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

  /**
   * @brief Applies a node action to each of this node's children
   *
   * Applies the specified action to this node's children.
   * Application is in child vector order.
   *
   * \c A \c NodeFunction application can cancel the child traversal
   * by returning  \c TraversalStatus::CANCEL.
   * The caller must deal with any resulting inconsistencies.
   *
   * Note that the method considers the child traversal when the
   * application returns \c TraversalStatus::BYPASS_CHILDREN.
   *
   * @param action \c NodeAction to apply
   * @return \c TraversalStatus::CANCEL when the action cancels traversal;
   *         \c TraversalStatus::CONTINUE otherwise. Note that the invocation
   *         \b never returns \c TraversalStatus::BYPASS_CHILDREN.
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

    /**
     * Indicates if this node has children. This is equivalent to
     *
     *     0 < child_count()
     *
     * and
     *
     *      !is_leaf()
     *
     * @return \c true if and only if this node has one or more children
     */
    bool has_children() const {
    return !children_.empty();
  }

  /*
   * A convenience method that returns true if and only if
   * this node is a leaf, i.e. if it has no children.
   * Equivalent to 0 == child_count()
   */
    /**
     * @brief Indicates that this node is a leaf (i.e. childless) node.
     *
     * This is a convenience function that is equivalent to
     *
     *     0 == child_count()
     *
     * and
     *
     *     !has_children()
     *
     * @return \c true if and only if this is a leaf (i.e. childless) node
     */
  bool is_leaf() const {
    return children_.empty();
  }

  /**
   * @brief Indicates if this is a root (i.e. parentless) node.
   *
   * @return \c true if and only if this node is a root, i.e. without
   *         a parent.
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
  /**
   * @brief Provides this node's \c Supplier (i.e. factory. Concrete node
   *        classes \b must implement this; abstract nodes \b must \b not.
   *
   * @return this node's \c Supplier as described above
   */

  virtual Supplier<T>& supplier() = 0;

  /**
   * @brief Provides this node's type name. Concrete node
   *        classes \b must implement this; abstract nodes \b must \b not.
   *
   * @return this node's type name
   */
  const std::string& type_name() {
    return supplier().class_name();
  }
};

}  /* namespace VisitingParseTree */

#endif /* NODE_H_ */
