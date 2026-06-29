#include <boost/test/unit_test.hpp>
#include <stdexcept>
#include "calculator.hpp"

BOOST_AUTO_TEST_SUITE(calculator_tests)

BOOST_AUTO_TEST_CASE(simple_expressions)
{
  BOOST_TEST(samarin::calculateExpression("1 + 3") == 4);
  BOOST_TEST(samarin::calculateExpression("4 * 7 - 3") == 25);
  BOOST_TEST(samarin::calculateExpression("( 1 + 2 ) * ( 3 - 4 )") == -3);
}

BOOST_AUTO_TEST_CASE(precedence_and_parentheses)
{
  BOOST_TEST(samarin::calculateExpression("2 + 2 * 2") == 6);
  BOOST_TEST(samarin::calculateExpression("( 10 / ( 2 + 3 ) % 4 )") == 2);
}

BOOST_AUTO_TEST_CASE(division_by_zero_throws)
{
  BOOST_CHECK_THROW(samarin::calculateExpression("5 / 0"), std::logic_error);
}

BOOST_AUTO_TEST_CASE(bad_operand_throws)
{
  BOOST_CHECK_THROW(samarin::calculateExpression("1 + x"), std::logic_error);
}

BOOST_AUTO_TEST_CASE(mismatched_parentheses_throws)
{
  BOOST_CHECK_THROW(samarin::calculateExpression("( 1 + 2"), std::logic_error);
}

BOOST_AUTO_TEST_SUITE_END()
