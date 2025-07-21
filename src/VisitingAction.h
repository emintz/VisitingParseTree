/*
 * VisitingAction.h
 *
 *  Created on: Jul 20, 2025
 *      Author: eric
 */

#ifndef SRC_VISITINGACTION_H_
#define SRC_VISITINGACTION_H_

#include "NodeAction.h"
#include "TraversalStatus.h"
#include "TreeCorruptError.h"
#include "Visitor.h"

namespace VisitingParseTree {

/*
 * An action that applies a visitor to nodes. The visitor
 * is set at construction. Note that T MUST extend Host.
 */
template <typename T> class VisitingAction : public NodeAction<T> {
  Visitor *visitor_;

public:
  VisitingAction(Visitor *visitor) :
      visitor_(visitor) {
  }

  virtual TraversalStatus operator() (std::shared_ptr<T> node) override {
    if (!node) {
      throw TreeCorruptError("Null node encountered during traversal.");
    }
    return node->accept(visitor_);
  }
};

} /* namespace VisitingParseTree */

#endif /* SRC_VISITINGACTION_H_ */
