/*
 * AttrNode.h
 *
 *  Created on: Jul 20, 2025
 *      Author: Eric Mintz
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
