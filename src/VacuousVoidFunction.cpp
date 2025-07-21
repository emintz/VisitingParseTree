/*
 * VacuousVoidFunction.cpp
 *
 *  Created on: Jul 20, 2025
 *      Author: Eric Mintz
 */

#include "VacuousVoidFunction.h"

namespace VisitingParseTree {

VacuousVoidFunction VacuousVoidFunction::INSTANCE;

void VacuousVoidFunction::operator ()(void) {
  // Do nothing.
}

} /* namespace VisitingParseTree */
