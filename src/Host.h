/*
 * Host.h
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
 * @file Host.h
 *
 * @brief extends the \c Node template to support runtime type
 *        identification-based double dispatch
 */

#ifndef HOST_H_
#define HOST_H_

#include "Node.h"
#include "Supplier.h"
#include "TraversalStatus.h"
#include "Visitor.h"

#include <memory>

namespace VisitingParseTree {

template<typename T> class HostVisitor;

/*
 * Base class for nodes that support visitation. Note
 * that T must descend from Host. This is not yet
 * enforced.
 *
 * TODO: enforce
 */
/**
 * @brief Adds runtime type identification-based double dispatch to \c Node.
 *
 * @tparam T subtype. When the subtype inherits directly from \c Host,
 *         T must be that subtype. For example, if \c MyHost is a direct
 *         descendant of \c Host, it must inherit from \c Host<MyHost>.
 */
template <typename T> class Host : public Node<T> {
protected:
  Host() = default;

public:
  virtual ~Host() = default;

  /**
   * @brief Applies the specified \c visitor to this host, if applicable.
   *
   * Every class inheriting \c Host \b must have its own, class-specific
   * visitor interface. This holds for both abstract and concrete classes.
   * Visitor interfaces declare a virtual method that accepts a pointer
   * to an instance of its associated node type. Here is the generated
   * visitor interface for a \c FooNode, which inherits \c BaseAttrNode
   * directly:
   *
   *     class FooVisitor {
   *     public:
   *      virtual VisitingParseTree::TraversalStatus processFoo(Foo *host) = 0;
   *     };
   *
   * \c FooNode::accept() then becomes:
   *
   *     VisitingParseTree::TraversalStatus Foo::accept(
   *         VisitingParseTree::Visitor *visitor) {
   *       auto concrete_visitor = dynamic_cast<FooVisitor *>(visitor);
   *       return concrete_visitor
   *           ? concrete_visitor->processFoo(this)
   *           : VisitingParseTree::BaseAttrNode::accept(visitor);
   *     }
   *
   * Visitation proceeds as follows:
   *
   *   -# If \c visitor inherits \c FooVisitor, \c dynamic_cast will
   *      return a valid (i.e. non-null) pointer. Otherwise it returns
   *      \c NULL.
   *   -# If \c concrete_visitor is valid, \c accept() passes \c this to
   *      the visitor's \c processFoo implementation for processing.
   *   -# Otherwise, \c concrete_visitor is \c NULL, meaning that \c visitor
   *      does not implement \c FooVisitor::processFoo(). It \c might,
   *      however, implement the visitor associated with its superclass,
   *      \c BaseAttrNodeVisitor, the "last chance" visitor, so it
   *      passes \c visitor to \c BaseAttrNode::accept() for possible
   *      handling.
   *
   * The node generator emits all required classes and logic automatically.
   *
   * @param visitor a class that inherits \c Visitor along with an arbitrary,
   *        possibly empty set of visitor class implementations.
   *
   * @return a \c TraversalStatus that determines subsequent traversal
   *         behavior
   *
   * \see \c TraversalStatus for a detailed description of status codes
   */
  virtual TraversalStatus accept(Visitor *visitor) = 0;
};

}  /* namespace VisitingParseTree */
#endif /* HOST_H_ */
