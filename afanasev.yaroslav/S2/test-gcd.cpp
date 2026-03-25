#include <boost/test/unit_test.hpp>
#include "calc.hpp"
#include "queue.hpp"
#include "input.hpp"

using namespace afanasev;

BOOST_AUTO_TEST_SUITE(gcdTest)

BOOST_AUTO_TEST_CASE(gcd_positive_numbers)
{
  BOOST_CHECK_EQUAL(gcd(12, 18), 6);
  BOOST_CHECK_EQUAL(gcd(48, 60), 12);
  BOOST_CHECK_EQUAL(gcd(7, 13), 1);
  BOOST_CHECK_EQUAL(gcd(0, 5), 5);
  BOOST_CHECK_EQUAL(gcd(5, 0), 5);
}

BOOST_AUTO_TEST_CASE(gcd_negative_numbers)
{
  BOOST_CHECK_EQUAL(gcd(-12, 18), 6);
  BOOST_CHECK_EQUAL(gcd(12, -18), 6);
  BOOST_CHECK_EQUAL(gcd(-12, -18), 6);
}

BOOST_AUTO_TEST_CASE(gcd_with_zero)
{
  BOOST_CHECK_EQUAL(gcd(0, 0), 0);
  BOOST_CHECK_EQUAL(gcd(0, 10), 10);
  BOOST_CHECK_EQUAL(gcd(10, 0), 10);
}

BOOST_AUTO_TEST_CASE(gcd_in_expression)
{
  std::istringstream is("36 gcd 24");
  Queue< Queue< std::string > > expressions = input(is);
  BOOST_REQUIRE_EQUAL(expressions.size(), 1);
  Queue< std::string > infix = expressions.get();
  Queue< std::string > postfix = convert(infix);
  long long result = calcExpr(postfix);
  BOOST_CHECK_EQUAL(result, 12);
}

BOOST_AUTO_TEST_CASE(gcd_with_other_operators)
{
  std::istringstream is("12 gcd 18 + 2");
  Queue< Queue< std::string > > expressions = input(is);
  BOOST_REQUIRE_EQUAL(expressions.size(), 1);
  Queue< std::string > infix = expressions.get();
  Queue< std::string > postfix = convert(infix);
  long long result = calcExpr(postfix);

  BOOST_CHECK_EQUAL(result, 8);
}

BOOST_AUTO_TEST_CASE(complex_gcd_expression)
{
  std::istringstream is("( 36 gcd 24 ) * ( 10 - 4 )");
  Queue< Queue< std::string > > expressions = input(is);
  BOOST_REQUIRE_EQUAL(expressions.size(), 1);
  Queue< std::string > infix = expressions.get();
  Queue< std::string > postfix = convert(infix);
  long long result = calcExpr(postfix);

  BOOST_CHECK_EQUAL(result, 72);
}

BOOST_AUTO_TEST_SUITE_END()
