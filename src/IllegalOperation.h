/*
 * IllegalOperation.h
 *
 *  Created on: Aug 18, 2025
 *      Author: Eric Mintz
 */

#pragma once

#include <string>

#include "TreeException.h"

namespace VisitingParseTree {

// Thrown when the application violates API semantics by,
// e.g. violating preconditions
class IllegalOperation : public TreeException {
public:
  IllegalOperation(const std::string& message) :
    TreeException(message) {}
};

}
