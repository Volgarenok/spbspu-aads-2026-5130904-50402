#define BOOST_TEST_MODULE S3
#include <functional>
#include <stdexcept>
#include <boost/test/included/unit_test.hpp>
#include "HashTable.hpp"
#include "hasher.hpp"

BOOST_AUTO_TEST_CASE(constructor)
{
  novikov::HashTable< int, int, novikov::SipHash< int >, std::equal_to< int > > ht;
  BOOST_TEST(ht.slotsCount() == 0);
  BOOST_TEST(ht.empty());
}

BOOST_AUTO_TEST_CASE(copy_constructor)
{
  novikov::HashTable< int, int, novikov::SipHash< int >, std::equal_to< int > > ht1;
  ht1.insert(0, 3);
  ht1.insert(6, 4);
  ht1.insert(8, 5);
  novikov::HashTable< int, int, novikov::SipHash< int >, std::equal_to< int > > ht2(ht1);
  BOOST_TEST(ht2.at(0) == 3);
  BOOST_TEST(ht2.at(6) == 4);
  BOOST_TEST(ht2.at(8) == 5);
}

BOOST_AUTO_TEST_CASE(move_constructor)
{
  novikov::HashTable< int, int, novikov::SipHash< int >, std::equal_to< int > > ht1;
  ht1.insert(0, 3);
  ht1.insert(6, 4);
  ht1.insert(8, 5);
  novikov::HashTable< int, int, novikov::SipHash< int >, std::equal_to< int > > ht2(std::move(ht1));
  BOOST_TEST(ht2.at(0) == 3);
  BOOST_TEST(ht2.at(6) == 4);
  BOOST_TEST(ht2.at(8) == 5);
}

BOOST_AUTO_TEST_CASE(copy_assignment_operator)
{
  novikov::HashTable< int, int, novikov::SipHash< int >, std::equal_to< int > > ht1;
  ht1.insert(0, 3);
  ht1.insert(6, 4);
  ht1.insert(8, 5);
  novikov::HashTable< int, int, novikov::SipHash< int >, std::equal_to< int > > ht2;
  ht2.insert(6, 10);
  ht2 = ht1;
  BOOST_TEST(ht2.at(0) == 3);
  BOOST_TEST(ht2.at(6) == 4);
  BOOST_TEST(ht2.at(8) == 5);
}

BOOST_AUTO_TEST_CASE(move_assignment_operator)
{
  novikov::HashTable< int, int, novikov::SipHash< int >, std::equal_to< int > > ht1;
  ht1.insert(0, 3);
  ht1.insert(6, 4);
  ht1.insert(8, 5);
  novikov::HashTable< int, int, novikov::SipHash< int >, std::equal_to< int > > ht2;
  ht2.insert(6, 10);
  ht2 = std::move(ht1);
  BOOST_TEST(ht2.at(0) == 3);
  BOOST_TEST(ht2.at(6) == 4);
  BOOST_TEST(ht2.at(8) == 5);
}

BOOST_AUTO_TEST_CASE(ht_non_empty_after_insert)
{
  novikov::HashTable< int, int, novikov::SipHash< int >, std::equal_to< int > > ht;
  BOOST_REQUIRE(ht.insert(0, 10));
  BOOST_TEST(ht.slotsCount() == 1);
  BOOST_TEST(!ht.empty());
}

BOOST_AUTO_TEST_CASE(contains_after_insert)
{
  novikov::HashTable< int, int, novikov::SipHash< int >, std::equal_to< int > > ht;
  BOOST_REQUIRE(ht.insert(0, 10));
  BOOST_TEST(ht.contains(0));
}

BOOST_AUTO_TEST_CASE(insert_duplicate)
{
  novikov::HashTable< int, int, novikov::SipHash< int >, std::equal_to< int > > ht;
  BOOST_REQUIRE(ht.insert(0, 1));
  BOOST_REQUIRE(ht.insert(1, 5));
  size_t oldSlotsCount = ht.slotsCount();
  BOOST_TEST(!ht.insert(0, 2));
  BOOST_TEST(ht.slotsCount() == oldSlotsCount);
}

BOOST_AUTO_TEST_CASE(at)
{
  novikov::HashTable< int, int, novikov::SipHash< int >, std::equal_to< int > > ht;
  BOOST_REQUIRE(ht.insert(0, 10));
  BOOST_TEST(ht.at(0) == 10);
}

BOOST_AUTO_TEST_CASE(change_at_value)
{
  novikov::HashTable< int, int, novikov::SipHash< int >, std::equal_to< int > > ht;
  BOOST_REQUIRE(ht.insert(0, 10));
  ht.at(0) = 2;
  BOOST_TEST(ht.at(0) == 2);
}

BOOST_AUTO_TEST_CASE(at_out_of_range)
{
  novikov::HashTable< int, int, novikov::SipHash< int >, std::equal_to< int > > ht;
  ht.insert(0, 10);
  BOOST_CHECK_THROW(ht.at(8), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(read_test)
{
  novikov::HashTable< int, int, novikov::SipHash< int >, std::equal_to< int > > ht;
  BOOST_REQUIRE(ht.insert(0, 10));
  BOOST_TEST(ht[0] == 10);
}

BOOST_AUTO_TEST_CASE(write_existing_value)
{
  novikov::HashTable< int, int, novikov::SipHash< int >, std::equal_to< int > > ht;
  BOOST_REQUIRE(ht.insert(0, 1));
  ht[0] = 10;
  BOOST_TEST(ht[0] == 10);
}

BOOST_AUTO_TEST_CASE(write_non_existent_value)
{
  novikov::HashTable< int, int, novikov::SipHash< int >, std::equal_to< int > > ht;
  ht[0] = 10;
  BOOST_TEST(ht[0] == 10);
}

BOOST_AUTO_TEST_CASE(erase_existing_value)
{
  novikov::HashTable< int, int, novikov::SipHash< int >, std::equal_to< int > > ht;
  BOOST_REQUIRE(ht.insert(0, 10));
  ht.insert(3, 8);
  BOOST_REQUIRE(ht.erase(0));
  BOOST_TEST(!ht.contains(0));
}

BOOST_AUTO_TEST_CASE(erase_non_existent_value)
{
  novikov::HashTable< int, int, novikov::SipHash< int >, std::equal_to< int > > ht;
  ht.insert(0, 10);
  ht.insert(3, 8);
  BOOST_TEST(!ht.erase(5));
}

BOOST_AUTO_TEST_CASE(swap)
{
  novikov::HashTable< int, int, novikov::SipHash< int >, std::equal_to< int > > ht1, ht2;
  ht1.insert(0, 10);
  ht1.insert(3, 8);
  ht2.insert(9, 21);
  ht2.insert(15, 45);
  ht1.swap(ht2);
  BOOST_TEST(ht1.at(9) == 21);
  BOOST_TEST(ht1.at(15) == 45);
  BOOST_TEST(ht2.at(0) == 10);
  BOOST_TEST(ht2.at(3) == 8);
}

BOOST_AUTO_TEST_CASE(rehash)
{
  novikov::HashTable< int, int, novikov::SipHash< int >, std::equal_to< int > > ht1;
  ht1.insert(0, 10);
  ht1.insert(3, 8);
  ht1.insert(9, 21);
  size_t oldSize = ht1.size();
  ht1.rehash(ht1.size() * 2);
  BOOST_TEST(ht1.size() == oldSize * 2);
  BOOST_TEST(ht1.at(0) == 10);
  BOOST_TEST(ht1.at(3) == 8);
  BOOST_TEST(ht1.at(9) == 21);
}
