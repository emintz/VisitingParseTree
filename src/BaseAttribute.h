/*
 * BaseAttribute.h
 *
 *  Created on: Aug 19, 2025
 *      Author: Eric Mintz
 */

#pragma once

#include <atomic>
#include <functional>

namespace VisitingParseTree {

class BaseAttribute {
  const int id_;

  static int next_id() {
    static std::atomic_int current_id_(0);
    return ++current_id_;
  }
protected:
  static std::hash<int> id_hash;

public:
  BaseAttribute(const BaseAttribute &other) = delete;
  BaseAttribute(BaseAttribute &&other) = delete;
  BaseAttribute& operator=(const BaseAttribute &other) = delete;
  BaseAttribute& operator=(BaseAttribute &&other) = delete;;

  BaseAttribute() :
    id_(next_id()) {
  }
  virtual ~BaseAttribute() = default;

  int id() const {
    return id_;
  }
};

} /* namespace VisitingParseTree */

