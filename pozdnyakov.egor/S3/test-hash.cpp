#include <stdexcept>
#include <string>
#include <boost/test/unit_test.hpp>
#include "hashTable.hpp"

using namespace pozdnyakov;

struct CollisionHash
{
  std::size_t operator()(const std::string &key) const
  {
    return 0;
  }
};

BOOST_AUTO_TEST_SUITE(HashTableTestSuite)

BOOST_AUTO_TEST_CASE(basicOperations)
{
  HashTable< std::string, int, CollisionHash > table;

  BOOST_CHECK(table.empty());
  BOOST_CHECK_EQUAL(table.size(), 0);

  table.add("key1", 100);
  BOOST_CHECK(!table.empty());
  BOOST_CHECK_EQUAL(table.size(), 1);
  BOOST_CHECK(table.has("key1"));
  BOOST_CHECK_EQUAL(table.at("key1"), 100);
}

BOOST_AUTO_TEST_CASE(collisionsAndUpdates)
{
  HashTable< std::string, int, CollisionHash > table;

  table.add("A", 10);
  table.add("B", 20);
  table.add("C", 30);

  BOOST_CHECK_EQUAL(table.size(), 3);
  BOOST_CHECK(table.has("A"));
  BOOST_CHECK(table.has("B"));
  BOOST_CHECK(table.has("C"));

  table.add("B", 25);
  BOOST_CHECK_EQUAL(table.size(), 3);
  BOOST_CHECK_EQUAL(table.at("B"), 25);
}

BOOST_AUTO_TEST_CASE(dropOperations)
{
  HashTable< std::string, int, CollisionHash > table;

  table.add("X", 1);
  table.add("Y", 2);

  bool isDropped = table.drop("X");
  BOOST_CHECK(isDropped);
  BOOST_CHECK_EQUAL(table.size(), 1);
  BOOST_CHECK(!table.has("X"));
  BOOST_CHECK(table.has("Y"));

  BOOST_CHECK(!table.drop("Z"));
}

BOOST_AUTO_TEST_SUITE_END()
