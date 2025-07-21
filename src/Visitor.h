/*
 * Visitor.h
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

#ifndef VISITOR_H_
#define VISITOR_H_

#include "TraversalStatus.h"

namespace VisitingParseTree {

/*
 * Marker interface for node visitors. All node visitors
 * MUST inherit frm this class.
 */
class Visitor {
protected:
  Visitor(void) {
  }

public:
  virtual ~Visitor() {
  }
};

}  /* namespace VisitingParseTree */
#endif /* VISITOR_H_ */
