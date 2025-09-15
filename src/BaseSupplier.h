/*
 * BaseSupplier.h
 *
 *  Created on: Sep 4, 2025
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
 *
 * Holds a node supplier's serial number. The serial numbers
 * are used to compare suppliers for equality.
 */

/**
 * @file BaseSupplier.h
 *
 * Base class for \b Node factories
 */
#ifndef SRC_BASESUPPLIER_H_
#define SRC_BASESUPPLIER_H_

namespace VisitingParseTree {

/**
 * @brief The base class for all Node factories, i.e. \c Supplier
 *        subtypes
 *
 * The \c BaseSupplier class provides each \c Supplier with a globally
 * unique integer identifier.
 */
class BaseSupplier {
  /**
   * Thread-safe supplier identifier provider
   *
   * @return the next available identifier. The returned value is
   *         guaranteed to be globally unique.
   */
  static int next_id();
  const int id_;  /** Globally unique \c Supplier identifier */
protected:
  /**
   * @brief Constructor
   *
   * Assigns globally unique identifiers to new instances.
   */
  BaseSupplier();
public:
  /**
   * @brief Destructor
   */
  virtual ~BaseSupplier() = default;
  BaseSupplier(const BaseSupplier &other) = delete;
  BaseSupplier(BaseSupplier &&other) = delete;
  BaseSupplier& operator=(const BaseSupplier &other) = delete;
  BaseSupplier& operator=(BaseSupplier &&other) = delete;

  /**
   * @return this instance's globally unique identifier
   */
  int id() const {
    return id_;
  }

  /**
   * Comparison
   *
   * @param that node to compare.
   * @return \c true if and only if \c this is the \b same \b instance
   *         as \c that.
   */
  bool operator==(const BaseSupplier& that) const {
    return id_ == that.id_;
  }

  /**
   * Comparison
   *
   * @param that the node to compare
   *
   * @return \c true if and only if \c this is \b not the same instance
   *         as \c that.
   */
  bool operator !=(const BaseSupplier& that) const {
    return id_ != that.id_;
  }
};

} /* namespace VisitingParseTree */

#endif /* SRC_BASESUPPLIER_H_ */
