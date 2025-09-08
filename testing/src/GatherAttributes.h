/*
 * GatherAttributes.h
 *
 *  Created on: Aug 19, 2025
 *      Author: eric
 */

#pragma once

#include <string>
#include <vector>

#include "Attribute.h"
#include "AttributeFunction.h"


class GatherAttributes : public VisitingParseTree::AttributeFunction {

public:
  struct Entry {
    const VisitingParseTree::Attribute* attribute;
    const std::string value;

    bool operator==(const Entry& rhs) const {
      return attribute == rhs.attribute && value == rhs.value;
    }
  };
private:
  std::vector<Entry> attributes_;

public:

  void operator() (const VisitingParseTree::Attribute *const& attribute, const std::string& value) {
    Entry entry = {attribute, value};

    attributes_.push_back(entry);
  }

  const std::vector<GatherAttributes::Entry>& attributes() {
    return attributes_;
  }

  GatherAttributes& reset() {
    attributes_.clear();
    return *this;
  }
};
