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

#ifndef BASEATTRNODE_H_
#define BASEATTRNODE_H_

#include "AttrNode.h"
#include "Supplier.h"
#include "TraversalStatus.h"
#include "Visitor.h"

namespace VisitingParseTree {

class BaseAttrNode;

class BaseAttrNodeVisitor {
public:
  virtual ~BaseAttrNodeVisitor();

  virtual TraversalStatus processBaseAttrNode(BaseAttrNode *host) = 0;
};

class BaseAttrNode: public AttrNode<BaseAttrNode> {
  friend class BaseAttrNodeSupplier;

  BaseAttrNode(const BaseAttrNode &other) = delete;
  BaseAttrNode(BaseAttrNode &&other) = delete;
  BaseAttrNode& operator=(const BaseAttrNode &other) = delete;
  BaseAttrNode& operator=(BaseAttrNode &&other) = delete;

protected:
  inline BaseAttrNode() {
  }

  class BaseAttrNodeSupplier : public Supplier<BaseAttrNode> {
  protected:
    BaseAttrNodeSupplier(const char *class_name) :
      Supplier<BaseAttrNode>(class_name) {}
  };

public:

  virtual ~BaseAttrNode();

  virtual TraversalStatus accept(Visitor *visitor) override;
};

} /* namespace VisitingParseTree */

#endif /* BASEATTRNODE_H_ */
