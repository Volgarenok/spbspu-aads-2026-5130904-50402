#include <functional>
#include "hasher.hpp"
#define BOOST_TEST_MODULE S1
#include <boost/test/included/unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include "HashTable.hpp"

BOOST_AUTO_TEST_CASE(default_constructor)
{
  shirokov::HashTable< int, int, shirokov::SHA1< int >, std::equal_to< int > > ht;
  BOOST_TEST(ht.size() == 0);
  BOOST_TEST(ht.slotsCount() == 0);
  BOOST_TEST(ht.empty());
}
