#define BOOST_TEST_MODULE S1
#include <boost/test/included/unit_test.hpp>
#include "list.hpp"

BOOST_AUTO_TEST_CASE(is_empty)
{
  alisov::BiList< int > v{};
  BOOST_TEST(v.empty());
}