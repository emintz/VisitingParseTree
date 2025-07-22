/*
 * AttributeFunction.h
 *
 *  Created on: Jul 22, 2025
 *      Author: Eric Mintz
 */

#ifndef SRC_ATTRIBUTEFUNCTION_H_
#define SRC_ATTRIBUTEFUNCTION_H_

#include "Attribute.h"

#include <string>

namespace VisitingParseTree {

/*
 * Interface for functions to be applied to all attributes
 * contained in a node.
 */
class AttributeFunction {
public:
  virtual ~AttributeFunction();

  virtual void operator() (const Attribute* const&, const std::string&) = 0;
};

} /* namespace VisitingParseTree */

#endif /* SRC_ATTRIBUTEFUNCTION_H_ */
