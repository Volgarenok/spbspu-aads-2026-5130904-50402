#include <string>
#include <boost/test/tools/old/interface.hpp>
#include <boost/test/unit_test.hpp>
#include "calc.hpp"

BOOST_AUTO_TEST_SUITE(lcmTtest)

BOOST_AUTO_TEST_CASE(lcm)
{
  long long res = dirko::calc("lcm", 3, 4);
  BOOST_CHECK_EQUAL(res, 12);
}

BOOST_AUTO_TEST_SUITE_END()
