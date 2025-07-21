/*
 * TreeCorruptError.h
 *
 *  Created on: Jul 19, 2025
 *      Author: Eric Mintz
 */

#ifndef TREECORRUPTERROR_H_
#define TREECORRUPTERROR_H_

#include <stdexcept>
#include <string>

namespace VisitingParseTree {

/*
 * Thrown when an operation detects tree corruption, e.g.
 * broken parent/child linkages.
 */
class TreeCorruptError : public std::runtime_error {

public:
  inline TreeCorruptError(const char *message) :
      std::runtime_error(message) {
  }

  inline TreeCorruptError(const std::string& message) :
      std::runtime_error(message) {
  }

  inline TreeCorruptError(const TreeCorruptError& other) :
      std::runtime_error(other) {
  }
};

} /* namespace VisitingParseTree */



#endif /* TREECORRUPTERROR_H_ */
