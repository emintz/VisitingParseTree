/*
 * BaseAttribute.h
 *
 *  Created on: Aug 19, 2025
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
 * @file BaseAttribute.h
 *
 * Base class for all \c Attribute implementations.
 */

#pragma once

#include <atomic>
#include <functional>

namespace VisitingParseTree {

/**
 * @brief Base class for all \c Attribute implementations. The class assigns
 *        each instance a globally unique identifier.
 */
class BaseAttribute {
  const int id_; /** Globally unique attribute identifier */

  /**
   * Thread-safe attribute identifier factory
   *
   * @return the next available globally unique attribute identifier
   */
  static int next_id() {
    static std::atomic_int current_id_(0);
    return ++current_id_;
  }
protected:
  static std::hash<int> id_hash;  /** Attribute identifier hashing function */

public:
  BaseAttribute(const BaseAttribute &other) = delete;
  BaseAttribute(BaseAttribute &&other) = delete;
  BaseAttribute& operator=(const BaseAttribute &other) = delete;
  BaseAttribute& operator=(BaseAttribute &&other) = delete;;

  /**
   * @brief Constructor
   *
   * Assigns the next available globally unique identifier to this instance
   */
  BaseAttribute() :
    id_(next_id()) {
  }
  virtual ~BaseAttribute() = default;

  /**
   * @return this instance's globally unique attribute identifier
   */
  int id() const {
    return id_;
  }
};

} /* namespace VisitingParseTree */

