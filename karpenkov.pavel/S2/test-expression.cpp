#include <boost/test/unit_test.hpp>

#include "exprProcessing.hpp"

#include <limits>
#include <sstream>

BOOST_AUTO_TEST_CASE(addition_test) { BOOST_TEST(eval(2, 3, "+") == 5); }

BOOST_AUTO_TEST_CASE(subtraction_test) { BOOST_TEST(eval(10, 4, "-") == 6); }

BOOST_AUTO_TEST_CASE(multiplication_test) { BOOST_TEST(eval(6, 7, "*") == 42); }

BOOST_AUTO_TEST_CASE(division_test) { BOOST_TEST(eval(20, 5, "/") == 4); }

BOOST_AUTO_TEST_CASE(modulo_test) { BOOST_TEST(eval(10, 3, "%") == 1); }

BOOST_AUTO_TEST_CASE(positive_modulo_test) {
  BOOST_TEST(eval(-249914, 664, "%") == 414);
}

BOOST_AUTO_TEST_CASE(divide_by_zero_test) {
  BOOST_CHECK_THROW(eval(10, 0, "/"), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(addition_overflow_test) {
  BOOST_CHECK_THROW(eval(std::numeric_limits<long long>::max(), 1, "+"),
                    std::overflow_error);
}

BOOST_AUTO_TEST_CASE(subtraction_underflow_test) {
  BOOST_CHECK_THROW(eval(std::numeric_limits<long long>::min(), 1, "-"),
                    std::overflow_error);
}

BOOST_AUTO_TEST_CASE(simple_expression_test) {
  std::stringstream input("1 + 2");

  auto expr = inputCLI(input);
  auto postfix = toPostfix(expr);
  auto result = calculateExpr(postfix);

  BOOST_TEST(result.top() == 3);
}

BOOST_AUTO_TEST_CASE(complex_expression_test) {
  std::stringstream input("( 1 + 2 ) * 3");

  auto expr = inputCLI(input);
  auto postfix = toPostfix(expr);
  auto result = calculateExpr(postfix);

  BOOST_TEST(result.top() == 9);
}

BOOST_AUTO_TEST_CASE(invalid_expression_test) {
  std::stringstream input("1 +");

  auto expr = inputCLI(input);
  auto postfix = toPostfix(expr);

  BOOST_CHECK_THROW(calculateExpr(postfix), std::runtime_error);
}
BOOST_AUTO_TEST_CASE(power_operation) {
  karpenkov::Stack<queueExpr> exprs;
  queueExpr q;

  q.push("2");
  q.push("**");
  q.push("3");

  exprs.push(q);

  auto postfix = toPostfix(exprs);
  auto result = calculateExpr(postfix);

  BOOST_TEST(result.top() == 8);
}
