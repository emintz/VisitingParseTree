/*
 * BaseNode.h
 *
 *  Created on: Aug 18, 2025
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
 * @file BaseNode.h
 *
 * Base class for all \c Node implementations
 */
#pragma once

#include <atomic>

namespace VisitingParseTree {

class BaseNode {

  /**
   * Thread-safe globally unique node identifier provider
   *
   * @return the next available globally unique node identifier
   */
  static int next_id();
  const int node_id;  /** Globally unique node identifier */

public:
  BaseNode(const BaseNode &other) = delete;
  BaseNode(BaseNode &&other) = delete;
  BaseNode& operator=(const BaseNode &other) = delete;
  BaseNode& operator=(BaseNode &&other) = delete;

  /**
   * @brief Constructor
   *
   * Assigns a globally unique node identifier to the newly constructed
   * node
   */
  BaseNode();

  /**
   * @return this node's globally unique identifier
   */
  int id() const {
    return node_id;
  }
};

} /* namespace VisitingParseTree */
