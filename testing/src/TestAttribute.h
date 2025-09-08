/*
 * TestAttribute.h
 *
 *  Created on: Jul 14, 2025
 *      Author: Eric Mintz
 */

#ifndef TESTATTRIBUTE_H_
#define TESTATTRIBUTE_H_

#include <Attribute.h>

class TestAttribute : public VisitingParseTree::Attribute {
  TestAttribute(const char *name);

public:
  virtual ~TestAttribute();

  static TestAttribute BIRTH_DATE;
  static TestAttribute BYPASS_CHILDREN_ON_ENTRY;  // Return TraversalStatus::BYPASS_CHILDREN on entry if set
  static TestAttribute BYPASS_CHILDREN_ON_EXIT;  // Return TraversalStatus::BYPASS_CHILDREN on exit if set
  static TestAttribute CANCEL_ON_ENTRY;  // Return TraversalStatus::CANCEL on entry if set
  static TestAttribute CANCEL_ON_EXIT;  // Return TraversalStatus::CANCEL on exit if set
  static TestAttribute NAME;
  static TestAttribute SERIAL_NO;
  static TestAttribute URL;
  static TestAttribute VALUE;
};

#endif /* TESTATTRIBUTE_H_ */
