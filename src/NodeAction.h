/*
 * NodeAction.h
 *
 *  Created on: Jul 19, 2025
 *      Author: Eric Mintz
 */

#ifndef NODEACTION_H_
#define NODEACTION_H_

#include "TraversalStatus.h"

#include <memory>

namespace VisitingParseTree {

/*
 * Base type for an action that can be applied to
 * a node of type T. Note that Node must be a base
 * class of T. This is not yet enforced.
 *
 * TODO: find a way to enforce base type.
 */
template <typename T> class NodeAction {
protected:
  NodeAction(void) {
  }

public:
  virtual ~NodeAction() {
  }

  virtual TraversalStatus operator()(std::shared_ptr<T> node) = 0;
};

} /* namespace VisitingParseTree */

#endif /* NODEACTION_H_ */
