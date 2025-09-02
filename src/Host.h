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
template <typename T> class Host : public Node<T> {
protected:
  Host() = default;

public:
  virtual ~Host() = default;

  virtual TraversalStatus accept(Visitor *visitor) = 0;
};

}  /* namespace VisitingParseTree */
#endif /* HOST_H_ */
