#include <string>
#include <boost/test/tools/old/interface.hpp>
#include <boost/test/unit_test.hpp>
#include "calc.hpp"
#include "queue.hpp"

BOOST_AUTO_TEST_SUITE(lcmTtest)

BOOST_AUTO_TEST_CASE(lcm)
{
  long long res = dirko::calc("lcm", 3, 4);
  BOOST_CHECK_EQUAL(res, 12);
}

BOOST_AUTO_TEST_CASE(lcm_priority)
{
  dirko::Queue< std::string > expretion, postf;
  expretion.push("12");
  expretion.push("/");
  expretion.push("3");
  expretion.push("lcm");
  expretion.push("4");
  postf = dirko::convert(expretion);
  BOOST_CHECK_EQUAL(dirko::calcExpr(postf), 1);
}

BOOST_AUTO_TEST_SUITE_END()
