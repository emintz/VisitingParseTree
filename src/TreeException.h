/*
 * TreeException.h
 *
 *  Created on: Aug 18, 2025
 *      Author: Eric Mintz
 */

#pragma once

namespace VisitingParseTree {

#include <stdexcept>
#include <string>

class TreeException : public std::runtime_error {
protected:
  TreeException(const std::string& message) :
    std::runtime_error(message) {}
};

} // namespace VisitingParseTree
