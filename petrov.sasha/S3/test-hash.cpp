#include <boost/test/unit_test.hpp>
#include "HashTable.hpp"
#include "SHA1Hash.hpp"
#include <string>

BOOST_AUTO_TEST_SUITE(HashTableTests)

BOOST_AUTO_TEST_CASE(test_add_and_get)
{
  petrov::HashTable< int, std::string, petrov::SHA1Hasher< int >, petrov::DefaultEqual< int > > table(10);
  
  table.add(1, "one");
  table.add(2, "two");
  
  BOOST_CHECK_EQUAL(table.get(1), "one");
  BOOST_CHECK_EQUAL(table.get(2), "two");
  BOOST_CHECK_EQUAL(table.size(), 2);
}

BOOST_AUTO_TEST_CASE(test_has)
{
  petrov::HashTable< int, std::string, petrov::SHA1Hasher< int >, petrov::DefaultEqual< int > > table(10);
  
  table.add(5, "five");
  
  BOOST_CHECK(table.has(5));
  BOOST_CHECK(!table.has(10));
}

BOOST_AUTO_TEST_CASE(test_drop)
{
  petrov::HashTable< int, std::string, petrov::SHA1Hasher< int >, petrov::DefaultEqual< int > > table(10);
  
  table.add(3, "three");
  BOOST_CHECK(table.has(3));
  
  table.drop(3);
  BOOST_CHECK(!table.has(3));
  BOOST_CHECK_EQUAL(table.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_drop_nonexistent)
{
  petrov::HashTable< int, std::string, petrov::SHA1Hasher< int >, petrov::DefaultEqual< int > > table(10);
  
  BOOST_CHECK_THROW(table.drop(999), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(test_get_nonexistent)
{
  petrov::HashTable< int, std::string, petrov::SHA1Hasher< int >, petrov::DefaultEqual< int > > table(10);
  
  BOOST_CHECK_THROW(table.get(999), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(test_collision_handling)
{
  petrov::HashTable< int, std::string, petrov::SHA1Hasher< int >, petrov::DefaultEqual< int > > table(5);
  
  table.add(1, "one");
  table.add(6, "six");
  table.add(11, "eleven");
  
  BOOST_CHECK_EQUAL(table.get(1), "one");
  BOOST_CHECK_EQUAL(table.get(6), "six");
  BOOST_CHECK_EQUAL(table.get(11), "eleven");
}

BOOST_AUTO_TEST_CASE(test_rehash)
{
  petrov::HashTable< int, std::string, petrov::SHA1Hasher< int >, petrov::DefaultEqual< int > > table(5);
  
  table.add(1, "one");
  table.add(2, "two");
  table.add(3, "three");
  
  table.rehash(20);
  
  BOOST_CHECK_EQUAL(table.get(1), "one");
  BOOST_CHECK_EQUAL(table.get(2), "two");
  BOOST_CHECK_EQUAL(table.get(3), "three");
  BOOST_CHECK_EQUAL(table.size(), 3);
}

BOOST_AUTO_TEST_CASE(test_copy_constructor)
{
  petrov::HashTable< int, std::string, petrov::SHA1Hasher< int >, petrov::DefaultEqual< int > > table1(10);
  table1.add(1, "one");
  table1.add(2, "two");
  
  petrov::HashTable< int, std::string, petrov::SHA1Hasher< int >, petrov::DefaultEqual< int > > table2(table1);
  
  BOOST_CHECK_EQUAL(table2.get(1), "one");
  BOOST_CHECK_EQUAL(table2.get(2), "two");
  BOOST_CHECK_EQUAL(table2.size(), 2);
}

BOOST_AUTO_TEST_CASE(test_move_constructor)
{
  petrov::HashTable< int, std::string, petrov::SHA1Hasher< int >, petrov::DefaultEqual< int > > table1(10);
  table1.add(1, "one");
  table1.add(2, "two");
  
  petrov::HashTable< int, std::string, petrov::SHA1Hasher< int >, petrov::DefaultEqual< int > > table2(std::move(table1));
  
  BOOST_CHECK_EQUAL(table2.get(1), "one");
  BOOST_CHECK_EQUAL(table2.get(2), "two");
  BOOST_CHECK_EQUAL(table2.size(), 2);
  BOOST_CHECK(table1.empty());
}

BOOST_AUTO_TEST_CASE(test_iterator)
{
  petrov::HashTable< int, std::string, petrov::SHA1Hasher< int >, petrov::DefaultEqual< int > > table(10);
  table.add(1, "one");
  table.add(2, "two");
  table.add(3, "three");
  
  int count = 0;
  for (auto it = table.begin(); it != table.end(); ++it) {
    ++count;
  }
  
  BOOST_CHECK_EQUAL(count, 3);
}

BOOST_AUTO_TEST_CASE(test_empty)
{
  petrov::HashTable< int, std::string, petrov::SHA1Hasher< int >, petrov::DefaultEqual< int > > table(10);
  
  BOOST_CHECK(table.empty());
  
  table.add(1, "one");
  BOOST_CHECK(!table.empty());
  
  table.drop(1);
  BOOST_CHECK(table.empty());
}

BOOST_AUTO_TEST_CASE(test_string_keys)
{
  petrov::HashTable< std::string, int, petrov::SHA1Hasher< std::string >, petrov::DefaultEqual< std::string > > table(10);
  
  table.add("hello", 1);
  table.add("world", 2);
  
  BOOST_CHECK_EQUAL(table.get("hello"), 1);
  BOOST_CHECK_EQUAL(table.get("world"), 2);
}

BOOST_AUTO_TEST_CASE(test_pair_keys)
{
  using KeyType = std::pair< std::string, std::string >;
  petrov::HashTable< KeyType, int, petrov::SHA1Hasher< KeyType >, petrov::DefaultEqual< KeyType > > table(10);
  
  table.add({"a", "b"}, 100);
  table.add({"c", "d"}, 200);
  
  BOOST_CHECK_EQUAL(table.get({"a", "b"}), 100);
  BOOST_CHECK_EQUAL(table.get({"c", "d"}), 200);
}

BOOST_AUTO_TEST_SUITE_END()

