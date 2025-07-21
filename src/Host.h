/*
 * Host.h
 *
 *  Created on: Jul 20, 2025
 *      Author: Eric Mintz
 */

#ifndef HOST_H_
#define HOST_H_

#include "Node.h"
#include "Supplier.h"
#include "TraversalStatus.h"
#include "Visitor.h"

#include <memory>

namespace VisitingParseTree {

template<typename T> class HostVisitor;

/*
 * Base class for nodes that support visitation. Note
 * that T must descend from Host. This is not yet
 * enforced.
 *
 * TODO: enforce
 */
template <typename T> class Host : public Node<T> {
protected:
  Host(void) {
  }

public:
  virtual ~Host() {
  }

  virtual TraversalStatus accept(Visitor *visitor) = 0;
};

}  /* namespace VisitingParseTree */
#endif /* HOST_H_ */
