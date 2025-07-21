/*
 * VoidFunction.h
 *
 *  Created on: Jul 20, 2025
 *      Author: Eric Mintz
 */

#ifndef VOIDFUNCTION_H_
#define VOIDFUNCTION_H_

namespace VisitingParseTree {

class VoidFunction {
public:

  virtual void operator() (void) = 0;
};
} /* namespace VisitingParseTree */
#endif /* VOIDFUNCTION_H_ */
