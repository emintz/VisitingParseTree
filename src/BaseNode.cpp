/*
 * BaseNode.cpp
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
 * Base node logic
 */
#include "BaseNode.h"

namespace VisitingParseTree {

int BaseNode::next_id() {
  static std::atomic_int id_source = 0;
  return id_source++;
}

BaseNode::BaseNode() :
    node_id(next_id()) {}

} /* namespace VisitingParseTree */
