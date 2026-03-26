#include <boost/test/unit_test.hpp>
#include <stdexcept>
#include "calculator.hpp"

using namespace pozdnyakov;

BOOST_AUTO_TEST_SUITE(CalculatorTests)

BOOST_AUTO_TEST_CASE(testMultiDigitNumbersAndSpaces)
{
  BOOST_CHECK_EQUAL(evaluateExpression("  123   +   456 "), 579);
  BOOST_CHECK_EQUAL(evaluateExpression("1000/10"), 100);
}

BOOST_AUTO_TEST_CASE(testStandardOperatorPrecedence)
{
  BOOST_CHECK_EQUAL(evaluateExpression("2 + 2 * 2"), 6);
  BOOST_CHECK_EQUAL(evaluateExpression("10 - 4 / 2"), 8);

  BOOST_CHECK_EQUAL(evaluateExpression("(2 + 2) * 2"), 8);
  BOOST_CHECK_EQUAL(evaluateExpression("10 - (2 * (3 + 4))"), -4);
}

BOOST_AUTO_TEST_CASE(testBitwiseAndPrecedence)
{
  BOOST_CHECK_EQUAL(evaluateExpression("6 & 3"), 2);

  BOOST_CHECK_EQUAL(evaluateExpression("1 & 2 + 3"), 1);

  BOOST_CHECK_EQUAL(evaluateExpression("3 + 5 & 2"), 0);

  BOOST_CHECK_EQUAL(evaluateExpression("(1 & 2) + 3"), 3);
}

BOOST_AUTO_TEST_CASE(testExceptionsAndInvalidExpressions)
{
  BOOST_CHECK_THROW(evaluateExpression("10 / 0"), std::runtime_error);
  BOOST_CHECK_THROW(evaluateExpression("10 % 0"), std::runtime_error);

  BOOST_CHECK_THROW(evaluateExpression("2 + (3 * 4"), std::runtime_error);
  BOOST_CHECK_THROW(evaluateExpression("2 + 3 * 4)"), std::runtime_error);
  BOOST_CHECK_THROW(evaluateExpression(")2 + 2("), std::runtime_error);

  BOOST_CHECK_THROW(evaluateExpression("2 ^ 3"), std::runtime_error);
  BOOST_CHECK_THROW(evaluateExpression("2 + a"), std::runtime_error);

  BOOST_CHECK_THROW(evaluateExpression("+ 2"), std::runtime_error);
  BOOST_CHECK_THROW(evaluateExpression("2 +"), std::runtime_error);

  BOOST_CHECK_THROW(evaluateExpression("2 2 + 3"), std::runtime_error);
}

BOOST_AUTO_TEST_SUITE_END()
