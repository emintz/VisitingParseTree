/*
 * BaseAttrNodeTraversal.h
 *
 *  Created on: Sep 3, 2025
 *      Author: Eric Mintz
 *
 * Specialization of the Traversal template for use with
 * the BaseAttrNode.
 */

#ifndef SRC_BASEATTRNODETRAVERSAL_H_
#define SRC_BASEATTRNODETRAVERSAL_H_

#include "BaseAttrNode.h"
#include "NodeAction.h"
#include "Traversal.h"
#include "TraversalStatus.h"
#include "VoidFunction.h"

namespace VisitingParseTree {

class BaseAttrNodeTraversal : public Traversal<BaseAttrNode> {
public:
  BaseAttrNodeTraversal(
      NodeAction<BaseAttrNode>& on_entry,
      NodeAction<BaseAttrNode>& on_exit,
      VoidFunction& after_descent,
      VoidFunction& before_ascent) :
        Traversal<BaseAttrNode>(
            on_entry,
            on_exit,
            after_descent,
            before_ascent) {}
};

} /* namespace VisitingParseTree */

#endif /* SRC_BASEATTRNODETRAVERSAL_H_ */
