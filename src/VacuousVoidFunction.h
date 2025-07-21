/*
 * VacuousVoidFunction.h
 *
 *  Created on: Jul 20, 2025
 *      Author: Eric Mintz
 */

#ifndef SRC_VACUOUSVOIDFUNCTION_H_
#define SRC_VACUOUSVOIDFUNCTION_H_

#include "VoidFunction.h"

namespace VisitingParseTree {

class VacuousVoidFunction : public VoidFunction {
public:

  static VacuousVoidFunction INSTANCE;

  virtual void operator() (void) override;


};

} /* namespace VisitingParseTree */

#endif /* SRC_VACUOUSVOIDFUNCTION_H_ */
