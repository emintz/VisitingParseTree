/*
 * TestAttribute.cpp
 *
 *  Created on: Jul 14, 2025
 *      Author: Eric Mintz
 */

#include "TestAttribute.h"

TestAttribute::TestAttribute(const char *name) :
    VisitingParseTree::Attribute(name) {
}

TestAttribute::~TestAttribute() {
}

TestAttribute TestAttribute::BIRTH_DATE("TestAttribute::BIRTH_DATE");
TestAttribute TestAttribute::BYPASS_CHILDREN_ON_ENTRY("TestAttribute::BYPASS_CHILDREN_ON_ENTRY");
TestAttribute TestAttribute::BYPASS_CHILDREN_ON_EXIT("TestAttribute::BYPASS_CHILDREN_ON_EXIT");
TestAttribute TestAttribute::CANCEL_ON_ENTRY("TestAttribute::CANCEL_ON_ENTRY");
TestAttribute TestAttribute::CANCEL_ON_EXIT("TestAttribute::CANCEL_ON_EXIT");
TestAttribute TestAttribute::NAME("TestAttribute::NAME");
TestAttribute TestAttribute::SERIAL_NO("SERIAL_NO");
TestAttribute TestAttribute::URL("URL");
TestAttribute TestAttribute::VALUE("VALUE");
