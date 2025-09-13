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

/**
 * @file Supplier.h
 *
 * Node factory API
 */
#ifndef SUPPLIER_H_
#define SUPPLIER_H_

#include <concepts>
#include <memory>
#include <string>

#include "BaseSupplier.h"
#include "Node.h"

namespace VisitingParseTree {

/*
 * Node factory class API. Note that VisitingParseTree::Node
 * MUST be a base class of T.
 *
 * TODO: find a way to enforce the base class requirement.
 */
/**
 * @brief Base class for all node factories
 *
 * Provides an API for allocating nodes. The node generator
 * will emit one \c Supplier subtype for each generated concrete
 * node type, and will provide the nodes fully qualified class name
 * to its constructor. Concrete node classes will contain a static
 * instance of their suppliers, which \b must be the \b only instance
 * of the supplier subtype,
 *
 * @tparam T node type. See Node documentation for restrictions.
 */
template <typename T> class Supplier : public BaseSupplier {
//  static_assert(std::is_base_of_v<Node<T>, T> == true);
  Supplier(Supplier&) = delete;
  Supplier(const Supplier&) = delete;
  Supplier& operator=(Supplier&) = delete;
  Supplier& operator=(const Supplier&) = delete;

  std::string class_name_; /** Generated node class name */

protected:
  /**
   * Constructor
   *
   * @param class_name fully qualifhed supplied node class name
   */
  Supplier(const std::string& class_name) :
    class_name_(class_name) {
  }

public:
  virtual ~Supplier() {
  }

  /**
   *
   * @return the fully qualified class name of the generated node. This
   *         is set at construction.
   */
  const std::string& class_name() {
    return class_name_;
  }

  /**
   * @brief Creates a node and returns it wrapped in a shared pointer.
   *
   * @return the returned shared pointer as described above.
   */
  virtual std::shared_ptr<T> make_shared() = 0;
};

} /* namespace VisitingParseTree */

#endif /* SUPPLIER_H_ */
