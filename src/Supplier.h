/*
 * Supplier.h
 *
 *  Created on: Jul 19, 2025
 *      Author: Eric Mintz
 */

#ifndef SUPPLIER_H_
#define SUPPLIER_H_

#include "Node.h"

#include <concepts>
#include <memory>
#include <string>

namespace VisitingParseTree {

/*
 * Node factory class API. Note that VisitingParseTree::Node
 * MUST be a base class of T.
 *
 * TODO: find a way to enforce the base class requirement.
 */
template <typename T> class Supplier {
//  static_assert(std::is_base_of_v<Node<T>, T> == true);
  Supplier(Supplier&) = delete;
  Supplier(const Supplier&) = delete;
  Supplier& operator=(Supplier&) = delete;
  Supplier& operator=(const Supplier&) = delete;

  std::string class_name_;

protected:
  Supplier(const char *class_name) :
    class_name_(class_name) {
  }

public:
  virtual ~Supplier() {
  }

  inline const char *class_name(void) {
    return class_name_.c_str();
  }

  /*
   * Create a node and return it wrapped in a shared pointer.
   */
  virtual std::shared_ptr<T> make_shared(void) = 0;

  bool operator==(const Supplier& that) const {
    return this == &that;
  }

  bool operator!=(const Supplier& that) const {
    return this != &that;
  }
};

} /* namespace VisitingParseTree */



#endif /* SUPPLIER_H_ */
