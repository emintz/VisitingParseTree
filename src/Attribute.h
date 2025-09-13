/*
 * Attribute.h
 *
 *  Created on: Jul 20, 2025
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

/*******************************
 * @file Attribute.h
 *
 * @brief Base class for attribute key classes. Subtype instances are
 *        used to identify attributes within unordered maps of
 *        <Attribute, std::string> pairs.
 ********************************/

#pragma once

#include <cstdint>
#include <string>

#include "BaseAttribute.h"

namespace VisitingParseTree {

/**
 * @brief Base class for attribute types, which are also used
 * as keys in an std::unordered:map. Note that subclass
 * MUST be static const and allocated during declaration.
 *
 * TODO: move hashing to base class (low priority)
 */
class Attribute : public BaseAttribute {

  const size_t hash_;  /** Attribute hash value. TODO: move to base class. */
  const std::string name_;  /** Fully qualified node class name. */

protected:
  /**
   * Construct an Attribute instance
   *
   * @param name the instance's name, which __should__ be globally unique
   */
  Attribute(const char *name);

public:
  Attribute(const Attribute&) = delete;
  Attribute& operator=(const Attribute&) = delete;
  virtual ~Attribute() = default;

  /**
   *
   * @return this attribute's memoized hash, whose value is
   *         set at construction.
   */
  size_t hash(void) const {
    return hash_;
  }

  /**
   *
   * @return this attributes name, which is set at construction
   */
  const std::string& name(void) const {
    return name_;
  }
};

} /* namespace VisitingParseTree */

namespace std {

/**
 * Attribute-specific hash function. Providing an attribute-specific
 * std::hash implementation allows attributes to be used to key an
 * unordered map, which is how attributed nodes store their attribute
 * values.
 */
template<>
class hash<VisitingParseTree::Attribute> {
public:
  /**
   * Attribute hashing algorithm
   *
   * @param attribute the attribute to hash
   * @return the attribute's hash code. Note that the attribute
   *         memoizes its hash code upon construction.
   */
  std::uint64_t operator() (const VisitingParseTree::Attribute& attribute) const {
    return attribute.hash();
  }
};

}
