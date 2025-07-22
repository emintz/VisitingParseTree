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

#include "Attribute.h"

namespace VisitingParseTree {

/*
 * Bit mixing algorithm nicked from
 *
 * https://stackoverflow.com/questions/664014/what-integer-hash-function-are-good-that-accepts-an-integer-hash-key
 */
static uint64_t mix_bits(uint64_t x) {
  x = (x ^ (x >> 30)) * UINT64_C(0xbf58476d1ce4e5b9);
  x = (x ^ (x >> 27)) * UINT64_C(0x94d049bb133111eb);
  x = x ^ (x >> 31);
  return x;
}

Attribute::Attribute(const char *name) :
    hash_(mix_bits(reinterpret_cast<uint64_t>(this))),
    name_(name) {
}

Attribute::~Attribute() {
}

bool Attribute::operator==(const Attribute& that) const {
  return this == &that;
}

bool Attribute::operator!=(const Attribute& that) const {
  return this != &that;
}

} /* namespace VisitingParseTree */
