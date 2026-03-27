#include "expression_utils.hpp"
#include <boost/test/unit_test.hpp>
#include <stdexcept>

namespace saldaev
{

  BOOST_AUTO_TEST_SUITE(operatorTests)

  BOOST_AUTO_TEST_CASE(default_constructor_and_empty)
  {
    BOOST_TEST(apply("!", 100) == -101);
    BOOST_TEST(apply("!", 255) == -256);
    BOOST_TEST(apply("!", 1023) == -1024);
    BOOST_TEST(apply("!", 2047) == -2048);
  }

  BOOST_AUTO_TEST_SUITE_END()

}
