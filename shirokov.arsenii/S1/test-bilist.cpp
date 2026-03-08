#include <boost/test/unit_test.hpp>
#include "bilist.hpp"

BOOST_AUTO_TEST_CASE(is_empty)
{
  shirokov::BiList< int > A{};
  BOOST_TEST(A.empty());
}
