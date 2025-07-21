/*
 * Visitor.h
 *
 *  Created on: Jul 20, 2025
 *      Author: Eric Mintz
 */

#ifndef VISITOR_H_
#define VISITOR_H_

#include "TraversalStatus.h"

namespace VisitingParseTree {

/*
 * Marker interface for node visitors
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
