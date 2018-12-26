#pragma once

#include <functional>

#include "expr.h"

/**
 * x<varid>-differentiates expression represented by node,
 * i.e. computes d/dxi expr, where xi is varid-th variable
 * and expr is the expression represented by given tree (node)
 */
Tree::NodePtr differentiate(Tree::NodePtr n, int varid);

std::pair<Tree::NodePtr, bool> simplify(Tree::NodePtr n);
