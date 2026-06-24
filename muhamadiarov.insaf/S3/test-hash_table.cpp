#include <boost/test/unit_test.hpp>
#include <string>
#include "hashTable.hpp"
#include "hasher.hpp"
#include <functional>

using namespace muhamadiarov;

BOOST_AUTO_TEST_SUITE(HashTableTest)

using TestHash = HashTable< int, int, SipHash< int >, std::equal_to< int > >;

BOOST_AUTO_TEST_CASE(test_constructor)
{
  TestHash ht(4, 3);
  BOOST_CHECK(ht.size() == 0);
  BOOST_CHECK(ht.capacity() == 16);
}

BOOST_AUTO_TEST_CASE(test_add)
{
  TestHash ht(4, 3);
  ht.add(5, 10);
  BOOST_CHECK(ht.size() == 1);
  ht.add(6, 11);
  BOOST_CHECK(ht.size() == 2);
}

BOOST_AUTO_TEST_CASE(test_has)
{
  TestHash ht(4, 3);
  ht.add(1, 1);
  ht.add(2, 2);

  BOOST_CHECK(ht.has(1));
  BOOST_CHECK(ht.has(2));
  BOOST_CHECK(!ht.has(3));
}

BOOST_AUTO_TEST_CASE(test_get)
{
  TestHash ht(4, 3);
  ht.add(1, 9);
  ht.add(2, 10);

  BOOST_CHECK_EQUAL(ht.get(1), 9);
  BOOST_CHECK_EQUAL(ht.get(2), 10);
  ht.get(1) = 11;
  BOOST_CHECK_EQUAL(ht.get(1), 11);
}

BOOST_AUTO_TEST_CASE(test_drop)
{
  TestHash ht(5, 5);
  ht.add(1, 1);
  ht.add(2, 2);

  int val = ht.drop(1);
  BOOST_CHECK_EQUAL(val, 1);
  BOOST_CHECK_EQUAL(ht.size(), 1);
  BOOST_CHECK(!ht.has(1));
  BOOST_CHECK(ht.has(2));
}

BOOST_AUTO_TEST_CASE(test_add_update_existing)
{
  TestHash ht(5, 5);
  ht.add(10, 1);
  ht.add(10, 2);

  BOOST_CHECK_EQUAL(ht.size(), 1);
  BOOST_CHECK_EQUAL(ht.get(10), 2);
}

BOOST_AUTO_TEST_CASE(test_clear)
{
  TestHash ht(5, 5);
  ht.add(1, 1);
  ht.add(2, 2);
  ht.add(3, 3);

  ht.clear();
  BOOST_CHECK_EQUAL(ht.size(), 0);
  BOOST_CHECK(!ht.has(1));
}

BOOST_AUTO_TEST_CASE(test_copy_constructor)
{
  TestHash ht1(5, 5);
  ht1.add(1, 1);
  ht1.add(2, 2);

  TestHash ht2 = ht1;

  BOOST_CHECK_EQUAL(ht2.size(), 2);
  BOOST_CHECK(ht2.has(1));
  BOOST_CHECK(ht2.has(2));
  BOOST_CHECK_EQUAL(ht2.get(1), 1);

  ht2.get(1) = 100;
  BOOST_CHECK_EQUAL(ht2.get(1), 100);
  BOOST_CHECK_EQUAL(ht1.get(1), 1);
}
BOOST_AUTO_TEST_CASE(test_move_constructor)
{
  TestHash ht1(4, 3);
  ht1.add(1, 1);
  ht1.add(2, 2);

  TestHash ht2 = std::move(ht1);

  BOOST_CHECK_EQUAL(ht2.size(), 2);
  BOOST_CHECK(ht2.has(1));
  BOOST_CHECK(ht2.has(2));
}

BOOST_AUTO_TEST_CASE(test_rehash)
{
  TestHash ht(10, 5);
  ht.add(1, 1);
  ht.add(2, 2);
  ht.add(3, 3);
  ht.add(4, 4);
  ht.add(5, 5);

  ht.rehash(15, 7);
  BOOST_CHECK_EQUAL(ht.size(), 5);
  BOOST_CHECK(ht.has(1));
  BOOST_CHECK(ht.has(2));
  BOOST_CHECK(ht.has(3));
  BOOST_CHECK(ht.has(4));
  BOOST_CHECK(ht.has(5));
}

BOOST_AUTO_TEST_SUITE_END()
