/*
 * AttrNodePrinter.h
 *
 *  Created on: Sep 6, 2025
 *      Author: Eric Mintz
 *
 * Prints an Attributed Node tree to an output stream. Note that this
 * printer only accepts BaseAttrNode and its subtypes.
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
 * @file AttrNodePrinter.h
 *
 * @brief Pretty prints a \c BaseAttrNode tree.
 *
 * The \c AttrNodePrinter class provides a tree traversal that
 * pretty prints a tree as it descends. Output is indented to
 * and annotated to show the tree structure, and includes each
 * node's type and attributes as shown in the following example
 * output.
 *
 *        RootNode []
 *         +--PlusNode []
 *             +--TimesNode []
 *             |   +--TimesNode []
 *             |   |   +--IntegerNode [VALUE->17 ]
 *             |   |   +--IntegerNode [VALUE->19 ]
 *             |   +--IntegerNode [VALUE->4 ]
 *             +--MinusNode []
 *                 +--IntegerNode [VALUE->5 ]
 *                 +--DivNode []
 *                     +--IntegerNode [VALUE->6 ]
 *                     +--IntegerNode [VALUE->3 ]
 *
 * Nodes can be prefixed with a sequence of
 *
 * * Nothing at all, the empty string, for the root
 * * \c +-- for a node at a non-root level
 * * A pipe character to connect parents of the
 *   currently printing node
 * * Spaces when the an ancestor of the currently
 *   printing is last in its child list.
 *
 * It is hoped that the resulting output portrays the tree structure
 * clearly and unambiguously.
 */

/**
 * @file AttrNodePrinter.h
 *
 * @brief Pretty prints a tree of \c BaseAttrNode nodes
 */

#ifndef SRC_ATTRNODEPRINTER_H_
#define SRC_ATTRNODEPRINTER_H_

#include <iostream>
#include <memory>

#include "BaseAttrNode.h"

namespace VisitingParseTree {

/**
 * @brief Pretty prints a tree of \c BaseAttrNode nodes
 *
 * Thin wrappe3r around a \c TreeTraversal that pretty-prints a
 * \c BaseAttrNode tree
 */
class AttrNodePrinter {
public:
  AttrNodePrinter() = default;
  virtual ~AttrNodePrinter() = default;
  AttrNodePrinter(const AttrNodePrinter &other) = delete;
  AttrNodePrinter(AttrNodePrinter &&other) = delete;
  AttrNodePrinter& operator=(const AttrNodePrinter &other) = delete;
  AttrNodePrinter& operator=(AttrNodePrinter &&other) = delete;

public:

  /**
   * @brief pretty prints a \c BaseAttrNode tree
   *
   * @param root[in] the tree to print. Note that printing a tree does
   *                 not change it
   * @param output_stream[in] receives the pretty printed tree
   */
  void print(std::shared_ptr<BaseAttrNode> root, std::ostream& output_stream);
};

} /* namespace VisitingParseTree */

#endif /* SRC_ATTRNODEPRINTER_H_ */
