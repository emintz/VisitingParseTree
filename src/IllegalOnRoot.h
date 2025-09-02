/*
 * IllegalOnRoot.h
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

#ifndef ILLEGALONROOT_H_
#define ILLEGALONROOT_H_

#include <string>

#include "TreeException.h"

namespace VisitingParseTree {

/*
 * Thrown when a user attempts an unsupported operation
 * (generally an operation involving the parent) on a
 * root node.
 */
class IllegalOnRoot : public TreeException {

public:

  explicit IllegalOnRoot(const std::string& message) :
      TreeException(message) {
  }
};

} /* namespace VisitingParseTree */

#endif /* ILLEGALONROOT_H_ */
