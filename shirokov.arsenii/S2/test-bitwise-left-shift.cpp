#include <boost/test/unit_test.hpp>
#include "eval.hpp"

BOOST_AUTO_TEST_CASE(bitwise_left_shift_test)
{
  BOOST_TEST(shirokov::eval("1 << 1") == 2);
  BOOST_TEST(shirokov::eval("12 << 4") == 192);
  BOOST_TEST(shirokov::eval("25 << 8") == 6400);
}
