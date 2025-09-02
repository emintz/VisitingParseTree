/*
 * Supplier.h
 *
 *  Created on: Jul 19, 2025
 *      Author: Eric Mintz
 *
 * Copyright (C) 2025 Eric Mintz
 * All Rights Reserved
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
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
  Supplier(const std::string& class_name) :
    class_name_(class_name) {
  }

public:
  virtual ~Supplier() {
  }

  const std::string& class_name() {
    return class_name_;
  }

  /*
   * Create a node and return it wrapped in a shared pointer.
   */
  virtual std::shared_ptr<T> make_shared() = 0;

  virtual bool operator==(const Supplier& that) const final {
    return this == &that;
  }

  virtual bool operator!=(const Supplier& that) const final {
    return this != &that;
  }
};

} /* namespace VisitingParseTree */

#endif /* SUPPLIER_H_ */
