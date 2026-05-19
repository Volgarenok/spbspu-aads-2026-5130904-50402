#include <boost/test/unit_test_suite.hpp>
#define BOOST_TEST_MODULE S3
#include <functional>
#include <stdexcept>
#include <boost/test/included/unit_test.hpp>
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

BOOST_AUTO_TEST_CASE(contains_after_insert)
{
  shirokov::HashTable< int, int, shirokov::SHA1< int >, std::equal_to< int > > ht;
  BOOST_REQUIRE(ht.insert(0, 1));
  BOOST_TEST(ht.contains(0));
}

BOOST_AUTO_TEST_CASE(insert_duplicate)
{
  shirokov::HashTable< int, int, shirokov::SHA1< int >, std::equal_to< int > > ht;
  BOOST_REQUIRE(ht.insert(0, 1));
  BOOST_REQUIRE(ht.insert(1, 5));
  size_t oldSlotsCount = ht.slotsCount();
  BOOST_TEST(!ht.insert(0, 2));
  BOOST_TEST(ht.slotsCount() == oldSlotsCount);
}

BOOST_AUTO_TEST_CASE(valid_at)
{
  shirokov::HashTable< int, int, shirokov::SHA1< int >, std::equal_to< int > > ht;
  BOOST_REQUIRE(ht.insert(0, 1));
  BOOST_TEST(ht.at(0) == 1);
}

BOOST_AUTO_TEST_CASE(change_value_of_at)
{
  shirokov::HashTable< int, int, shirokov::SHA1< int >, std::equal_to< int > > ht;
  BOOST_REQUIRE(ht.insert(0, 1));
  ht.at(0) = 2;
  BOOST_TEST(ht.at(0) == 2);
}

BOOST_AUTO_TEST_CASE(invalid_at)
{
  shirokov::HashTable< int, int, shirokov::SHA1< int >, std::equal_to< int > > ht;
  ht.insert(0, 1);
  BOOST_CHECK_THROW(ht.at(5), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(read_value)
{
  shirokov::HashTable< int, int, shirokov::SHA1< int >, std::equal_to< int > > ht;
  BOOST_REQUIRE(ht.insert(0, 1));
  BOOST_TEST(ht[0] == 1);
}

BOOST_AUTO_TEST_CASE(write_existing_value)
{
  shirokov::HashTable< int, int, shirokov::SHA1< int >, std::equal_to< int > > ht;
  BOOST_REQUIRE(ht.insert(0, 1));
  ht[0] = 2;
  BOOST_TEST(ht[0] == 2);
}

BOOST_AUTO_TEST_CASE(write_non_existent_value)
{
  shirokov::HashTable< int, int, shirokov::SHA1< int >, std::equal_to< int > > ht;
  ht[0] = 1;
  BOOST_TEST(ht[0] == 1);
}

BOOST_AUTO_TEST_CASE(erase_existing_value)
{
  shirokov::HashTable< int, int, shirokov::SHA1< int >, std::equal_to< int > > ht;
  BOOST_REQUIRE(ht.insert(0, 1));
  ht.insert(2, 5);
  BOOST_REQUIRE(ht.erase(0));
  BOOST_TEST(!ht.contains(0));
}

BOOST_AUTO_TEST_CASE(erase_non_existent_value)
{
  shirokov::HashTable< int, int, shirokov::SHA1< int >, std::equal_to< int > > ht;
  ht.insert(0, 1);
  ht.insert(2, 5);
  BOOST_TEST(!ht.erase(10));
}

BOOST_AUTO_TEST_CASE(copy_constructor_test)
{
  shirokov::HashTable< int, int, shirokov::SHA1< int >, std::equal_to< int > > ht1;
  ht1.insert(0, 1);
  ht1.insert(2, 5);
  ht1.insert(35, 2);
  shirokov::HashTable< int, int, shirokov::SHA1< int >, std::equal_to< int > > ht2(ht1);
  BOOST_TEST(ht2.at(0) == 1);
  BOOST_TEST(ht2.at(2) == 5);
  BOOST_TEST(ht2.at(35) == 2);
}

BOOST_AUTO_TEST_CASE(move_constructor_test)
{
  shirokov::HashTable< int, int, shirokov::SHA1< int >, std::equal_to< int > > ht1;
  ht1.insert(0, 1);
  ht1.insert(2, 5);
  ht1.insert(35, 2);
  shirokov::HashTable< int, int, shirokov::SHA1< int >, std::equal_to< int > > ht2(std::move(ht1));
  BOOST_TEST(ht2.at(0) == 1);
  BOOST_TEST(ht2.at(2) == 5);
  BOOST_TEST(ht2.at(35) == 2);
}
