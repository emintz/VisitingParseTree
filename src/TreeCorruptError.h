/*
 * TreeCorruptError.h
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

#ifndef TREECORRUPTERROR_H_
#define TREECORRUPTERROR_H_

#include <stdexcept>
#include <string>

namespace VisitingParseTree {

/*
 * Thrown when an operation detects tree corruption, e.g.
 * broken parent/child linkages.
 */
class TreeCorruptError : public std::runtime_error {

public:
  inline TreeCorruptError(const char *message) :
      std::runtime_error(message) {
  }

  inline TreeCorruptError(const std::string& message) :
      std::runtime_error(message) {
  }

  inline TreeCorruptError(const TreeCorruptError& other) :
      std::runtime_error(other) {
  }
};

} /* namespace VisitingParseTree */



#endif /* TREECORRUPTERROR_H_ */
