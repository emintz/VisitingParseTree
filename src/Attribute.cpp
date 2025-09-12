/*
 * Attribute.cpp
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
#include <functional>

#include "Attribute.h"

namespace VisitingParseTree {

/**
 * Constructor
 *
 * @param name[in] attribute name, which \b SHOULD be globally
 *                 should be globally unique
 */
Attribute::Attribute(const char *name) :
    BaseAttribute(),
    hash_(id_hash(id())),
    name_(name) {
}

} /* namespace VisitingParseTree */
