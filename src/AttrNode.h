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

#include <unordered_map>
#include <utility>

#include "Attribute.h"
#include "AttributeFunction.h"
#include "Host.h"

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

static const std::string empty_string;

template <typename T> class AttrNode : public Host<T> {
  std::unordered_map<const Attribute*, std::string> attributes_;

protected:
  AttrNode() = default;


public:
  virtual ~AttrNode() = default;

  int attribute_count() const {
    return attributes_.size();
  }

  /* Clones this node
   *
   * Allocates a node of the same type as this and copies this
   * node's attributes to it.
   *
   * Returns: the newly allocated node, as described above.
   */
  std::shared_ptr<T> clone() {
    auto copy = AttrNode<T>::empty_copy();
    return copy_attributes_to(copy);
  }

  /* Copies this nodes attributes to another node.
   *
   * Copies every attribute of this node to another node. This
   * node is not altered. The method overwrites attributes in
   * the destination, meaning that the destination's conflicting
   * attributes will be overwritten and non-conflicting
   * attributes will remain unchanged.
   *
   * Returns: a shared pointer to the destination node, for
   *          chaining.
   */
  std::shared_ptr<T> copy_attributes_to(std::shared_ptr<T> that) {
    class : public AttributeFunction {
      std::shared_ptr<T> destination_;

    public:
      void set_destination(std::shared_ptr<T> destination) {
        destination_ = destination;
      }
      virtual void operator() (const Attribute* const& attr, const std::string& value) {
        destination_->set(*attr, value);
      }
    } action;
    action.set_destination(that);
    for_all_attributes(action);
    return that;
  }

  /* Erases the specified attribute
   *
   * Erases the specified attribute if it exists; does nothing if the
   * specified attribute does not exist.
   *
   * Parameters:
   * ----------
   *
   * Name                     Contents
   * -----------------        --------------------------------------------
   * attribute                The attribute to erase
   *
   * Returns: a shared pointer to this node to support chaining
   */
  std::shared_ptr<T> erase(const Attribute& attribute) {
    if (has(attribute)) {
      attributes_.erase(&attribute);
    }
    return std::enable_shared_from_this<T>::shared_from_this();
  }

  void for_all_attributes(AttributeFunction& f) {
    for (const auto&[key, value] : attributes_) {
      f(key, value);
    }
  }

  /** Returns the specified attribute's value
   *
   * Parameters:
   * ----------
   *
   * Name                     Contents
   * -----------------        --------------------------------------------
   * attribute                The attribute to return
   *
   * Returns: the attribute's value if it has been set, an empty string
   *          otherwise. Note that attributes CANNOT be set to an
   *          empty value.
   */
  const std::string& get(const Attribute& attribute) const {
    return has(attribute)
        ? attributes_.at(&attribute)
        : empty_string;
  }

  /** Tests if this node has a specified attribute, i.e. the attribute has a value
   *
   * Parameters:
   * ----------
   *
   * Name                     Contents
   * -----------------        --------------------------------------------
   * attribute                The attribute to set
   *
   * Returns: true if and only if the attribute has been set.
   */
  bool has(const Attribute& attribute) const {
    return attributes_.contains(&attribute);
  }

  /** Sets a non-empty attribute value, erases the attribute if value is empty
   *
   * Parameters:
   * ----------
   *
   * Name                     Contents
   * -----------------        --------------------------------------------
   * attribute                The attribute to set
   * value                    The value to set. SHOULD not be empty,
   *                          as the attribute will be erased if it is.
   *
   * Returns: a shared pointer to this node to support chaining
   */
  std::shared_ptr<T> set(const Attribute& attribute, const std::string value) {
    if (value.empty()) {
      return erase(attribute);
    } else {
      attributes_.insert_or_assign(&attribute, value);
      return std::enable_shared_from_this<T>::shared_from_this();
    }
  }
};

} /* namespace VisitingParseTree */

#endif /* ATTRNODE_H_ */
