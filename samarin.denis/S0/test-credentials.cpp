#include <boost/test/unit_test.hpp>
#include <sstream>
#include "credentials.hpp"

BOOST_AUTO_TEST_CASE(test_credentials)
{
  std::ostringstream out;
  samarin::out_credentials(out);
  BOOST_TEST(out.str() == "samarin.denis");
}
