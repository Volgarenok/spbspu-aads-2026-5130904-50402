#include <boost/test/tools/old/interface.hpp>
#define BOOST_TEST_MODULE S3
#include <functional>
#include <boost/test/included/unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include "HashTable.hpp"
#include "hasher.hpp"

BOOST_AUTO_TEST_CASE(default_constructor)
{
  shirokov::HashTable< int, int, shirokov::SHA1< int >, std::equal_to< int > > ht;
  BOOST_TEST(ht.slotsCount() == 0);
  BOOST_TEST(ht.empty());
}

BOOST_AUTO_TEST_CASE(insert_value_make_non_empty)
{
  shirokov::HashTable< int, int, shirokov::SHA1< int >, std::equal_to< int > > ht;
  BOOST_REQUIRE(ht.insert(0, 1));
  BOOST_TEST(ht.slotsCount() == 1);
  BOOST_TEST(!ht.empty());
}
