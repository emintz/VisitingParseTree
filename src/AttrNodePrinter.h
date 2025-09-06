/*
 * AttrNodePrinter.h
 *
 *  Created on: Sep 6, 2025
 *      Author: Eric Mintz
 *
 * Prints an Attributed Node tree to an output stream. Note that this
 * printer only accepts BaseAttrNode and its subtypes.
 */

#ifndef SRC_ATTRNODEPRINTER_H_
#define SRC_ATTRNODEPRINTER_H_

#include <iostream>
#include <memory>

#include "BaseAttrNode.h"

namespace VisitingParseTree {

class AttrNodePrinter {
public:
  AttrNodePrinter() = default;
  virtual ~AttrNodePrinter() = default;
  AttrNodePrinter(const AttrNodePrinter &other) = delete;
  AttrNodePrinter(AttrNodePrinter &&other) = delete;
  AttrNodePrinter& operator=(const AttrNodePrinter &other) = delete;
  AttrNodePrinter& operator=(AttrNodePrinter &&other) = delete;

public:

  void print(std::shared_ptr<BaseAttrNode> root, std::ostream& output_stream);
};

} /* namespace VisitingParseTree */

#endif /* SRC_ATTRNODEPRINTER_H_ */
