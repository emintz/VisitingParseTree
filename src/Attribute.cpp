/*
 * Attribute.cpp
 *
 *  Created on: Jul 20, 2025
 *      Author: Eric Mintz
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

} /* namespace VisitingParseTree */
