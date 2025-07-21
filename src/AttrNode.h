/*
 * AttrNode.h
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

#ifndef ATTRNODE_H_
#define ATTRNODE_H_

#include "Attribute.h"
#include "Host.h"

#include <unordered_map>

/*
 * Base class for nodes containing string-valued attributes.
 *
 * Note that T MUST be a subclass of VisitingParseTree. This
 * is not yet enforced.
 *
 * TODO: enforce
 */

#include "Attribute.h"

#include <optional>
#include <string>
#include <unordered_map>

namespace VisitingParseTree {

template <typename T> class AttrNode : public Host<T> {
  std::unordered_map<const Attribute*, std::string> attributes_;
protected:
  AttrNode(void) :
      attributes_() {
  }

public:
  virtual ~AttrNode() {
  }

  int attribute_count(void) const {
    return attributes_.size();
  }

  std::optional<const std::string> get(const Attribute& attribute) {
    return has(attribute)
        ? std::optional<const std::string>(attributes_.at(&attribute))
        : std::nullopt;
  }

  inline bool has(const Attribute& attribute) {
    return attributes_.contains(&attribute);
  }

  std::shared_ptr<T> set(const Attribute& attribute, const std::string value) {
    attributes_.insert_or_assign(&attribute, value);
    return std::enable_shared_from_this<T>::shared_from_this();
  }
};

} /* namespace VisitingParseTree */

#endif /* ATTRNODE_H_ */
