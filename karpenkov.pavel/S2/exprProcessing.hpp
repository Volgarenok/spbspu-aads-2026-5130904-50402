#ifndef EXPR_PROCESSING_HPP
#define EXPR_PROCESSING_HPP

#include "queue.hpp"
#include "stack.hpp"
#include <iostream>

using queueExpr = karpenkov::Queue<std::string>;

bool isNumber(const std::string &element);
bool isValidOperator(const std::string &element);
size_t priority(const std::string &operation);

karpenkov::Stack<queueExpr> inputCLI(std::istream &in);
karpenkov::Stack<queueExpr> toPostfix(karpenkov::Stack<queueExpr> expressions);

karpenkov::Stack<long long>
calculateExpr(karpenkov::Stack<queueExpr> &postfixExpr);

void queueOutput(std::ostream &out, karpenkov::Stack<long long> result);

long long eval(long long a, long long b, const std::string &operation);

#endif
