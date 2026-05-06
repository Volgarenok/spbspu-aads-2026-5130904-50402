#include <boost/test/unit_test.hpp>
#include "math_op.hpp"

BOOST_AUTO_TEST_SUITE(xorSuite)

BOOST_AUTO_TEST_CASE(xorBasicOperation)
{
  BOOST_CHECK_EQUAL(karpovich::calculateOperation("^", 5, 3), 6);
  BOOST_CHECK_EQUAL(karpovich::calculateOperation("^", 0, 0), 0);
  BOOST_CHECK_EQUAL(karpovich::calculateOperation("^", 7, 7), 0);
  BOOST_CHECK_EQUAL(karpovich::calculateOperation("^", 15, 9), 6);
}

BOOST_AUTO_TEST_CASE(xorSimpleExpression)
{
  karpovich::Queue< std::string > infix;
  infix.push("5");
  infix.push("^");
  infix.push("3");

  karpovich::Queue< std::string > postfix = karpovich::convertToPostfix(infix);
  BOOST_CHECK_EQUAL(karpovich::calculatePostfix(postfix), 6);
}

BOOST_AUTO_TEST_CASE(xorWithParentheses)
{
  karpovich::Queue< std::string > infix;
  infix.push("(");
  infix.push("1");
  infix.push("+");
  infix.push("2");
  infix.push(")");
  infix.push("^");
  infix.push("(");
  infix.push("4");
  infix.push("-");
  infix.push("2");
  infix.push(")");

  karpovich::Queue< std::string > postfix = karpovich::convertToPostfix(infix);
  BOOST_CHECK_EQUAL(karpovich::calculatePostfix(postfix), 1);
}

BOOST_AUTO_TEST_CASE(xorChained)
{
  karpovich::Queue< std::string > infix;
  infix.push("1");
  infix.push("^");
  infix.push("2");
  infix.push("^");
  infix.push("3");

  karpovich::Queue< std::string > postfix = karpovich::convertToPostfix(infix);
  BOOST_CHECK_EQUAL(karpovich::calculatePostfix(postfix), 0);
}

BOOST_AUTO_TEST_SUITE_END()
