#define BOOST_TEST_MODULE S3
#include <boost/test/included/unit_test.hpp>
#include "HashTableImpl.hpp"

size_t hashFunc(int key)
{
  boost::hash2::siphash_64 hash;
  hash.update(&key, sizeof(key));
  return hash.result();
}

bool equal(int a, int b)
{
  return a == b;
}

using table_t = vishnevskiy::HashTable<int, int, std::size_t(*)(int), bool(*)(int, int)>;

BOOST_AUTO_TEST_CASE(constructor_test)
{
  table_t constructTable(9, hashFunc, equal);
  BOOST_CHECK_EQUAL(constructTable.getSize(), 0);
  BOOST_CHECK_EQUAL(constructTable.getCapacity(), 9);
}

BOOST_AUTO_TEST_CASE(add_test)
{
  table_t addTable(9, hashFunc, equal);
  addTable.add(1, 15);
  addTable.add(2, 25);
  addTable.add(3, 27);
  BOOST_CHECK(addTable.has(1));
  BOOST_CHECK(addTable.has(2));
  BOOST_CHECK(addTable.has(3));
}

BOOST_AUTO_TEST_CASE(drop_test)
{
  table_t dropTable(9, hashFunc, equal);
  dropTable.add(1, 15);
  dropTable.add(2, 25);
  dropTable.add(3, 27);
  BOOST_CHECK_EQUAL(dropTable.drop(2), 25);
  BOOST_CHECK(dropTable.has(1));
  BOOST_CHECK(!dropTable.has(2));
  BOOST_CHECK(dropTable.has(3));
}

BOOST_AUTO_TEST_CASE(rehash_test)
{
  table_t rehashTable(9, hashFunc, equal);
  rehashTable.add(1, 15);
  rehashTable.add(2, 25);
  rehashTable.add(3, 27);
  rehashTable.rehash(5);
  BOOST_CHECK(rehashTable.has(1));
  BOOST_CHECK(rehashTable.has(2));
  BOOST_CHECK(rehashTable.has(3));
  BOOST_CHECK_EQUAL(rehashTable.getSize(), 3);
  BOOST_CHECK_EQUAL(rehashTable.getCapacity(), 5);
}
