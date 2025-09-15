/*
 * BaseAttrNode.h
 *
 *  Created on: Jul 28, 2025
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
 * @file BaseAttrNode.h
 *
 * Base class for generated node classes together with support classes
 */

#ifndef BASEATTRNODE_H_
#define BASEATTRNODE_H_

#include "AttrNode.h"
#include "Supplier.h"
#include "TraversalStatus.h"
#include "Visitor.h"

namespace VisitingParseTree {

class BaseAttrNode;
class BaseAttrNodeVisitor {

/**
 * @brief Visitor API for \c BaseAttrNode
 *
 * If the \c Visitor passed to \c BaseAttrNode::accept() implements
 * \c BaseAttrNodeVisitor, \c BaseAttrNode::accept() will pass itself
 * (i.e. \this) to the visitor's \c processBaseNode implementation. Otherwise
 * \c BaseAttrNode::accept() does nothing.
 * \c BaseAttrNodeVisitor::processBaseAttrNode supports a processor of
 * last resort, a method to invoke if no node type-specific methods
 * exist.
 *
 */
public:
  virtual ~BaseAttrNodeVisitor() = default;

  /**
   * Base node processor
   *
   * @param host the \c BaseAttrNode to process
   *
   * @return traversal control indication
   *
   * \see TraversalStatus
   */
  virtual TraversalStatus processBaseAttrNode(BaseAttrNode *host) = 0;
};

/**
 * @brief Root of the generated node hierarchy
 *
 * The node generator produces nodes in a single-rooted inheritance hierarchy
 * rooted at \c BaseAttrNode.
 */
class BaseAttrNode: public AttrNode<BaseAttrNode> {
  friend class BaseAttrNodeSupplier;

  BaseAttrNode(const BaseAttrNode &other) = delete;
  BaseAttrNode(BaseAttrNode &&other) = delete;
  BaseAttrNode& operator=(const BaseAttrNode &other) = delete;
  BaseAttrNode& operator=(BaseAttrNode &&other) = delete;

protected:
  BaseAttrNode() = default;

  /**
   * @brief Base class for the concrete generated node factories
   */
  class BaseAttrNodeSupplier : public Supplier<BaseAttrNode> {
  protected:
    BaseAttrNodeSupplier(const char *class_name) :
      Supplier<BaseAttrNode>(class_name) {}
  };

public:

  virtual ~BaseAttrNode() = default;

  virtual TraversalStatus accept(Visitor *visitor) override;
};

} /* namespace VisitingParseTree */

#endif /* BASEATTRNODE_H_ */
