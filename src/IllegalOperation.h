/*
 * IllegalOperation.h
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
 * @file IllegalOperation.h
 *
 * General purpose illegal operation exception
 */
#pragma once

#include <string>

#include "TreeException.h"

namespace VisitingParseTree {

/**
 * @brief Thrown when an application makes an invalid call
 *        e.g. a call that violates preconditions.
 */
class IllegalOperation : public TreeException {
public:
  IllegalOperation(const std::string& message) :
    TreeException(message) {}
};

}
