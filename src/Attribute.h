/*
 * Attribute.h
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

  virtual bool operator==(const Attribute& that) const final;

  virtual bool operator!=(const Attribute& that) const final;

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
