/*
 * BaseSupplier.h
 *
 *  Created on: Sep 4, 2025
 *      Author: Eric Mintz
 *
 * Holds a node supplier's serial number. The serial numbers
 * are used to compare suppliers for equality.
 */

#ifndef SRC_BASESUPPLIER_H_
#define SRC_BASESUPPLIER_H_

namespace VisitingParseTree {

class BaseSupplier {
  static int next_id();
  const int id_;
protected:
  BaseSupplier();
public:
  virtual ~BaseSupplier() = default;
  BaseSupplier(const BaseSupplier &other) = delete;
  BaseSupplier(BaseSupplier &&other) = delete;
  BaseSupplier& operator=(const BaseSupplier &other) = delete;
  BaseSupplier& operator=(BaseSupplier &&other) = delete;

  int id() const {
    return id_;
  }
};

} /* namespace VisitingParseTree */

#endif /* SRC_BASESUPPLIER_H_ */
