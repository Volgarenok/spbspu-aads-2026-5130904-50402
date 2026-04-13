#include <boost/test/unit_test.hpp>
#include <string>
#include "action.hpp"
#include "queque.hpp"

BOOST_AUTO_TEST_SUITE(xorTest)
BOOST_AUTO_TEST_CASE(xcor)
{
  int result = muhamadiarov::calc(1, 2, '^');
  BOOST_CHECK_EQUAL(result, 3);
}

BOOST_AUTO_TEST_CASE(priorityXor)
{
  muhamadiarov::Queque< std::string > expr;
  expr.push("12");
  expr.push("/");
  expr.push("6");
  expr.push("^");
  expr.push("2");
  BOOST_CHECK_EQUAL(calcExpr(expr), 0); 
}
BOOST_AUTO_TEST_SUITE_END()
