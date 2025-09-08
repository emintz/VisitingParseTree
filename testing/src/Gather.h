/*
 * Gather.h
 *
 *  Created on: Aug 19, 2025
 *      Author: cEric Mintz */

#pragma once


#include "AttributedTestNode.h"

using namespace std;
using namespace VisitingParseTree;


class Gather : public NodeAction<BaseAttrNode> {
  vector<std::shared_ptr<BaseAttrNode>> gathered_;

public:
  TraversalStatus operator()(std::shared_ptr<BaseAttrNode> node) {
    gathered_.push_back(node);
    return TraversalStatus::CONTINUE;
  }

  const std::vector<std::shared_ptr<BaseAttrNode>>& operator()() {
    return gathered_;
  }

  Gather& reset() {
    gathered_.clear();
    return *this;
  }
};
