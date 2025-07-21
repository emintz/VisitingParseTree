/*
 * NodeAction.h
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

#ifndef NODEACTION_H_
#define NODEACTION_H_

#include "TraversalStatus.h"

#include <memory>

namespace VisitingParseTree {

/*
 * Base type for an action that can be applied to
 * a node of type T. Note that Node must be a base
 * class of T. This is not yet enforced.
 *
 * TODO: find a way to enforce base type.
 */
template <typename T> class NodeAction {
protected:
  NodeAction(void) {
  }

public:
  virtual ~NodeAction() {
  }

  virtual TraversalStatus operator()(std::shared_ptr<T> node) = 0;
};

} /* namespace VisitingParseTree */

#endif /* NODEACTION_H_ */
