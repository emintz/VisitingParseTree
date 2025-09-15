/*
 * VacuousVoidFunction.h
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
 * @file VacuousVoidFunction.h
 *
 * @brief Vacuous (i.e. do-nothing) \c VoidFunction implementation
 */

#ifndef SRC_VACUOUSVOIDFUNCTION_H_
#define SRC_VACUOUSVOIDFUNCTION_H_

#include "VoidFunction.h"

/*
 * A VoidFunction implementation that does absolutely
 * nothing whatsoever.
 */
namespace VisitingParseTree {

/**
 * @brief A \c VoidFunction implementation that does absolutely
 *        nothing.
 *
 * An empty, do-nothing \c VoidFunction, a place holder value.
 */
class VacuousVoidFunction : public VoidFunction {
public:

  /**
   * The sole \c VacuousVoidFunction instance. Since the function
   * is completely stateless, there's no point in instantiating
   * it multiple times.
   */
  static VacuousVoidFunction INSTANCE;

  virtual ~VacuousVoidFunction();

  virtual void operator() (void) override;
};

} /* namespace VisitingParseTree */

#endif /* SRC_VACUOUSVOIDFUNCTION_H_ */
