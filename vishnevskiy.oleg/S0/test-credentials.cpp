#include <boost/test/unit_test.hpp>
#include <sstream>
#include "credentials.hpp"

BOOST_AUTO_TEST_CASE(test_credentials)
{
  std::ostringstream print;
  vishnevskiy::out_credentials(print);
  BOOST_TEST(print.str() == "vishnevskiy.oleg");
}

