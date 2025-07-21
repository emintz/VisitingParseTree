/*
 * IllegalOnRoot.h
 *
 *  Created on: Jul 19, 2025
 *      Author: Eric Mintz
 */

#ifndef ILLEGALONROOT_H_
#define ILLEGALONROOT_H_

#include <stdexcept>
#include <string>

namespace VisitingParseTree {

class IllegalOnRoot : public std::runtime_error {

public:
  inline IllegalOnRoot(const char *message) :
      std::runtime_error(message) {
  }

  inline IllegalOnRoot(const std::string& message) :
      std::runtime_error(message) {
  }

  inline IllegalOnRoot(const IllegalOnRoot& other) :
      std::runtime_error(other) {
  }
};

} /* namespace VisitingParseTree */

#endif /* ILLEGALONROOT_H_ */
