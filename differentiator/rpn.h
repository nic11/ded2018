#pragma once

#include "expr.h"

/**
 * Takes space-separated tokens in form:
 *  - <number> (const)
 *  - x<number> (variable)
 *  - <operation> from: +, -, *, /, ^, sin, cos, log
 * Sequence of these terms is treated as RPN
 * (Reverse Polish Notation) representing expression.
 * 
 * Returns tree
 */
Tree::NodePtr treeFromRpn(std::string str);

/**
 * Takes tree and returns its RPN (see strToRpn).
 */
std::string treeToRpn(Tree::NodePtr n);
