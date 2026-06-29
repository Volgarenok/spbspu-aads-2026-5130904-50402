#include <boost/test/unit_test.hpp>
#include "eval.hpp"

using namespace novikov;

BOOST_AUTO_TEST_SUITE(XorTest)

BOOST_AUTO_TEST_CASE(simple_xor)
{
  BOOST_CHECK_EQUAL(eval("2 ^ 3"), 1);
  BOOST_CHECK_EQUAL(eval("5 ^ 3"), 6);
  BOOST_CHECK_EQUAL(eval("0 ^ 5"), 5);
  BOOST_CHECK_EQUAL(eval("7 ^ 0"), 7);
}

BOOST_AUTO_TEST_CASE(xor_with_negative)
{
  BOOST_CHECK_EQUAL(eval("-1 ^ 1"), -2);
  BOOST_CHECK_EQUAL(eval("5 ^ -3"), -8);
}

BOOST_AUTO_TEST_CASE(xor_precedence)
{
  BOOST_CHECK_EQUAL(eval("1 + 2 ^ 3"), 1 + (2 ^ 3));
  BOOST_CHECK_EQUAL(eval("1 ^ 2 + 3"), (1 ^ 2) + 3);
}

BOOST_AUTO_TEST_CASE(xor_with_parentheses)
{
  BOOST_CHECK_EQUAL(eval("( 1 ^ 2 ) ^ 3"), (1 ^ 2) ^ 3);
  BOOST_CHECK_EQUAL(eval("1 ^ ( 2 ^ 3 )"), 1 ^ (2 ^ 3));
}

BOOST_AUTO_TEST_CASE(xor_with_different_types)
{
  BOOST_CHECK_EQUAL(eval("10 ^ 6"), 12);
  BOOST_CHECK_EQUAL(eval("15 ^ 9"), 6);
  BOOST_CHECK_EQUAL(eval("255 ^ 128"), 127);
}

BOOST_AUTO_TEST_CASE(xor_in_complex_expression)
{
  BOOST_CHECK_EQUAL(eval("( 12 * 7 ) ^ ( 12 + 5 )"), (84 ^ 17));
  BOOST_CHECK_EQUAL(eval("( 10 ^ 2 ) + ( 9 ^ 4 )"), (8 + 13));
}

BOOST_AUTO_TEST_SUITE_END()
