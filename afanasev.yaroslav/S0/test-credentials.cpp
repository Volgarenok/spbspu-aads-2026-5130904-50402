#define BOOST_TEST_MODULE Test credentials
#include <boost/test/included/unit_test.hpp>
#include <sstream>
#include "credentials.hpp"

BOOST_AUTO_TEST_CASE(credentials_test)
{
  std::ostringstream out;
  afanasev::out_credentials(out);
  BOOST_TEST(out.str() == "afanasev.yaroslav");
}
