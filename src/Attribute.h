/*
 * Attribute.h
 *
 *  Created on: Jul 20, 2025
 *      Author: Eric Mintz
 */

#ifndef ATTRIBUTE_H_
#define ATTRIBUTE_H_

#include <cstdint>
#include <string>

namespace VisitingParseTree {

/*
 * Base class for attribute types, which are also used
 * as keys in an std::unordered:map. Note that subclass
 * MUST be static const and allocated during declaration.
 */
class Attribute {
  Attribute(const Attribute&) = delete;
  Attribute& operator=(const Attribute&) = delete;

  const uint64_t hash_;
  const std::string name_;

protected:
  Attribute(const char *name);

public:
  virtual ~Attribute();

  inline size_t hash(void) const {
    return static_cast<size_t>(hash_);
  }

  inline const char *name(void) const {
    return name_.c_str();
  }
};

} /* namespace VisitingParseTree */

namespace std {

template<>
class hash<VisitingParseTree::Attribute> {
public:
  std::uint64_t operator() (const VisitingParseTree::Attribute& attribute) const {
    return attribute.hash();
  }
};

}
#endif /* ATTRIBUTE_H_ */
