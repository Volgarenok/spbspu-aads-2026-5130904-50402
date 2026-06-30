#define BOOST_TEST_MAIN
#include <functional>
#include <stdexcept>
#include <boost/test/included/unit_test.hpp>
#include "HashTable.hpp"
#include "hasher.hpp"

BOOST_AUTO_TEST_CASE(default_constructor)
{
  alisov::HashTable< int, int, alisov::SHA1< int >, std::equal_to< int > > ht;
  BOOST_TEST(ht.slotsCount() == 0);
  BOOST_TEST(ht.empty());
}

BOOST_AUTO_TEST_CASE(insert_value_make_non_empty)
{
  alisov::HashTable< int, int, alisov::SHA1< int >, std::equal_to< int > > ht;
  BOOST_REQUIRE(ht.insert(0, 1));
  BOOST_TEST(ht.slotsCount() == 16);
  BOOST_TEST(!ht.empty());
}

BOOST_AUTO_TEST_CASE(contains_after_insert)
{
  alisov::HashTable< int, int, alisov::SHA1< int >, std::equal_to< int > > ht;
  BOOST_REQUIRE(ht.insert(0, 1));
  BOOST_TEST(ht.contains(0));
}

BOOST_AUTO_TEST_CASE(insert_duplicate)
{
  alisov::HashTable< int, int, alisov::SHA1< int >, std::equal_to< int > > ht;
  BOOST_REQUIRE(ht.insert(0, 1));
  BOOST_REQUIRE(ht.insert(1, 5));
  size_t oldSlotsCount = ht.slotsCount();
  BOOST_TEST(!ht.insert(0, 2));
  BOOST_TEST(ht.slotsCount() == oldSlotsCount);
}

BOOST_AUTO_TEST_CASE(valid_at)
{
  alisov::HashTable< int, int, alisov::SHA1< int >, std::equal_to< int > > ht;
  BOOST_REQUIRE(ht.insert(0, 1));
  BOOST_TEST(ht.at(0) == 1);
}

BOOST_AUTO_TEST_CASE(change_value_of_at)
{
  alisov::HashTable< int, int, alisov::SHA1< int >, std::equal_to< int > > ht;
  BOOST_REQUIRE(ht.insert(0, 1));
  ht.at(0) = 2;
  BOOST_TEST(ht.at(0) == 2);
}

BOOST_AUTO_TEST_CASE(invalid_at)
{
  alisov::HashTable< int, int, alisov::SHA1< int >, std::equal_to< int > > ht;
  ht.insert(0, 1);
  BOOST_CHECK_THROW(ht.at(5), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(read_value)
{
  alisov::HashTable< int, int, alisov::SHA1< int >, std::equal_to< int > > ht;
  BOOST_REQUIRE(ht.insert(0, 1));
  BOOST_TEST(ht[0] == 1);
}

BOOST_AUTO_TEST_CASE(write_existing_value)
{
  alisov::HashTable< int, int, alisov::SHA1< int >, std::equal_to< int > > ht;
  BOOST_REQUIRE(ht.insert(0, 1));
  ht[0] = 2;
  BOOST_TEST(ht[0] == 2);
}

BOOST_AUTO_TEST_CASE(write_non_existent_value)
{
  alisov::HashTable< int, int, alisov::SHA1< int >, std::equal_to< int > > ht;
  ht[0] = 1;
  BOOST_TEST(ht[0] == 1);
}

BOOST_AUTO_TEST_CASE(erase_existing_value)
{
  alisov::HashTable< int, int, alisov::SHA1< int >, std::equal_to< int > > ht;
  BOOST_REQUIRE(ht.insert(0, 1));
  ht.insert(2, 5);
  BOOST_REQUIRE(ht.erase(0));
  BOOST_TEST(!ht.contains(0));
}

BOOST_AUTO_TEST_CASE(erase_non_existent_value)
{
  alisov::HashTable< int, int, alisov::SHA1< int >, std::equal_to< int > > ht;
  ht.insert(0, 1);
  ht.insert(2, 5);
  BOOST_TEST(!ht.erase(10));
}

BOOST_AUTO_TEST_CASE(copy_constructor_test)
{
  alisov::HashTable< int, int, alisov::SHA1< int >, std::equal_to< int > > ht1;
  ht1.insert(0, 1);
  ht1.insert(2, 5);
  ht1.insert(35, 2);
  alisov::HashTable< int, int, alisov::SHA1< int >, std::equal_to< int > > ht2(ht1);
  BOOST_TEST(ht2.at(0) == 1);
  BOOST_TEST(ht2.at(2) == 5);
  BOOST_TEST(ht2.at(35) == 2);
}

BOOST_AUTO_TEST_CASE(move_constructor_test)
{
  alisov::HashTable< int, int, alisov::SHA1< int >, std::equal_to< int > > ht1;
  ht1.insert(0, 1);
  ht1.insert(2, 5);
  ht1.insert(35, 2);
  alisov::HashTable< int, int, alisov::SHA1< int >, std::equal_to< int > > ht2(std::move(ht1));
  BOOST_TEST(ht2.at(0) == 1);
  BOOST_TEST(ht2.at(2) == 5);
  BOOST_TEST(ht2.at(35) == 2);
}

BOOST_AUTO_TEST_CASE(swap_test)
{
  alisov::HashTable< int, int, alisov::SHA1< int >, std::equal_to< int > > ht1, ht2;
  ht1.insert(0, 1);
  ht1.insert(2, 4);
  ht2.insert(5, 7);
  ht2.insert(12, 34);
  ht1.swap(ht2);
  BOOST_TEST(ht1.at(5) == 7);
  BOOST_TEST(ht1.at(12) == 34);
  BOOST_TEST(ht2.at(0) == 1);
  BOOST_TEST(ht2.at(2) == 4);
}

BOOST_AUTO_TEST_CASE(copy_assignment_operator_test)
{
  alisov::HashTable< int, int, alisov::SHA1< int >, std::equal_to< int > > ht1;
  ht1.insert(0, 1);
  ht1.insert(2, 5);
  ht1.insert(35, 2);
  alisov::HashTable< int, int, alisov::SHA1< int >, std::equal_to< int > > ht2;
  ht2.insert(2, 4);
  ht2 = ht1;
  BOOST_TEST(ht2.at(0) == 1);
  BOOST_TEST(ht2.at(2) == 5);
  BOOST_TEST(ht2.at(35) == 2);
}

BOOST_AUTO_TEST_CASE(move_assignment_operator_test)
{
  alisov::HashTable< int, int, alisov::SHA1< int >, std::equal_to< int > > ht1;
  ht1.insert(0, 1);
  ht1.insert(2, 5);
  ht1.insert(35, 2);
  alisov::HashTable< int, int, alisov::SHA1< int >, std::equal_to< int > > ht2;
  ht2.insert(2, 4);
  ht2 = std::move(ht1);
  BOOST_TEST(ht2.at(0) == 1);
  BOOST_TEST(ht2.at(2) == 5);
  BOOST_TEST(ht2.at(35) == 2);
}

BOOST_AUTO_TEST_CASE(rehash_test)
{
  alisov::HashTable< int, int, alisov::SHA1< int >, std::equal_to< int > > ht1;
  ht1.insert(0, 1);
  ht1.insert(2, 5);
  ht1.insert(35, 2);
  ht1.rehash(32);
  BOOST_TEST(ht1.slotsCount() == 32);
  BOOST_TEST(ht1.at(0) == 1);
  BOOST_TEST(ht1.at(2) == 5);
  BOOST_TEST(ht1.at(35) == 2);
}
