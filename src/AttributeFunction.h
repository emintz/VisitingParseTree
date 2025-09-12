/*
 * AttributeFunction.h
 *
 *  Created on: Jul 22, 2025
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

/*****************************************************************
 * @file AttributeFunction.h
 *
 * @brief Interface for functions to be applied to all attributes
 * contained in a node.
 *****************************************************************/

#ifndef SRC_ATTRIBUTEFUNCTION_H_
#define SRC_ATTRIBUTEFUNCTION_H_

#include "Attribute.h"

#include <string>

namespace VisitingParseTree {
class AttributeFunction {
public:
  virtual ~AttributeFunction() = default;

  /**
   * @brief Attribute processing API. Overrides provide attribute
   *        processing logic, which can cause arbitrary side effects.
   *
   * @param Attribute type, the key in the containing node's attribute map
   * @param Attribute value
   */
  virtual void operator() (const Attribute* const&, const std::string&) = 0;
};

} /* namespace VisitingParseTree */

#endif /* SRC_ATTRIBUTEFUNCTION_H_ */
