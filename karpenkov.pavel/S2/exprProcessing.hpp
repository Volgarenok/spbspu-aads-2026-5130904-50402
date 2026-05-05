#ifndef EXPR_PROCESSING_HPP
#define EXPR_PROCESSING_HPP

#include <iostream>
#include "queue.hpp"
#include "stack.hpp"

using queueExpr = karpenkov::Queue< std::string >;

bool isNumber(const std::string &element);
bool isValidOperator(const std::string &element);
size_t priority(const std::string &operation);

karpenkov::Stack< queueExpr > inputCLI(std::istream &in);
karpenkov::Stack< queueExpr > toPostfix(karpenkov::Stack< queueExpr > expressions);

karpenkov::Stack< int > calculateExpr(karpenkov::Stack< queueExpr > &postfixExpr);

void queueOutput(std::ostream &out, karpenkov::Stack< int > result);

int eval(int a, int b, const std::string &operation);

#endif
