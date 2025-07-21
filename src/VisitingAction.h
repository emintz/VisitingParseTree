/*
 * VisitingAction.h
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

#ifndef SRC_VISITINGACTION_H_
#define SRC_VISITINGACTION_H_

#include "NodeAction.h"
#include "TraversalStatus.h"
#include "TreeCorruptError.h"
#include "Visitor.h"

namespace VisitingParseTree {

/*
 * An action that applies a visitor to nodes. The visitor
 * is set at construction. Note that T MUST extend Host.
 */
template <typename T> class VisitingAction : public NodeAction<T> {
  Visitor *visitor_;

public:
  VisitingAction(Visitor *visitor) :
      visitor_(visitor) {
  }

  virtual TraversalStatus operator() (std::shared_ptr<T> node) override {
    if (!node) {
      throw TreeCorruptError("Null node encountered during traversal.");
    }
    return node->accept(visitor_);
  }
};

} /* namespace VisitingParseTree */

#endif /* SRC_VISITINGACTION_H_ */
