#include "credentials.hpp"
#include <boost/test/unit_test.hpp>
#include <sstream>

BOOST_AUTO_TEST_CASE(test_test)
{
  std::ostringstream out;
  karpenkov::out_credentials(out);
  BOOST_TEST(out.std() == "karpenkov.pavel");
}
