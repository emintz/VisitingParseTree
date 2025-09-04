/*
 * BaseSupplier.cpp
 *
 *  Created on: Sep 4, 2025
 *      Author: Eric Mintz
 */

#include <atomic>

#include "BaseSupplier.h"

namespace VisitingParseTree {

BaseSupplier::BaseSupplier() :
    id_(next_id()) {
}

int BaseSupplier::next_id() {
  static std::atomic_int id_source = 0;
  return id_source++;
}

} /* namespace VisitingParseTree */
