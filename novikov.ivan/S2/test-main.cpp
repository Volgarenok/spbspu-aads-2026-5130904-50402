#define BOOST_TEST_MODULE S2
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE(bitwise_or_test)
{
  BOOST_TEST(novikov::eval("1 | 1") == 1);
  BOOST_TEST(novikov::eval("5 | 10") == 15);
  BOOST_TEST(novikov::eval("10 | 34") == 42);
  BOOST_TEST(novikov::eval("34 | 10") == 42);
}
