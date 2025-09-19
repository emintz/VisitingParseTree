/*
 * Created on: Jul 19, 2025
 * By:         Eric Mintz
 *
 * Traversal status that determines behavior. See
 * enumerations for details.
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

/**
 * @file TraversalStatus.h
 *
 * Traversal control enumeration
 */
#ifndef TRAVERSAL_STATUS_H_
#define TRAVERSAL_STATUS_H_

namespace VisitingParseTree {

/*
 * Indicates the state of a traversal and the
 * next action that should be taken.
 */
/**
 * @brief Governs traversal behavior
 *
 * Returned by node actions to control the traversal.
 */
enum class TraversalStatus {
  /**
   * Continue running the traversal. Process children
   * after entry; return after exit. Traversal is OK.
   */
  CONTINUE,
  /**
   * Bypass processing children on exit; equivalent to
   * CONTINUE on exit. Traversal is OK.
   */
  BYPASS_CHILDREN,
  /**
   * Cancel the traversal immediately. The traversal stops
   * \b WITHOUT invoking additional actions. In particular,
   * if if returned on entry, the bypass both the node's
   * children AND this node's exit processing. \b MIGHT indicate an
   * error or \b might simply mean that further processing
   * is not required.
   */
  CANCEL,         /** Cancel the traversal immediately without further processing */
};

}  /* Namespace VisitingParseTree */

#endif /* TRAVERSAL_STATUS_H_ */
