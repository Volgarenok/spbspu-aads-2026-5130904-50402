#include <boost/test/unit_test.hpp>
#include <cstddef>
#include <stdexcept>
#include <string>
#include "hash_table.hpp"
#include "hashers.hpp"

BOOST_AUTO_TEST_SUITE(hash_table_tests)

BOOST_AUTO_TEST_CASE(add_and_has)
{
  samarin::HashTable< std::string, int, samarin::StringHash > table;
  table.add("a", 1);
  table.add("b", 2);
  BOOST_TEST(table.has("a"));
  BOOST_TEST(table.has("b"));
  BOOST_TEST(!table.has("c"));
  BOOST_TEST(table.size() == 2u);
}

BOOST_AUTO_TEST_CASE(add_overwrites_value)
{
  samarin::HashTable< std::string, int, samarin::StringHash > table;
  table.add("a", 1);
  table.add("a", 5);
  BOOST_TEST(table.size() == 1u);
  BOOST_TEST(table["a"] == 5);
}

BOOST_AUTO_TEST_CASE(subscript_inserts_default)
{
  samarin::HashTable< std::string, int, samarin::StringHash > table;
  table["x"] += 3;
  BOOST_TEST(table["x"] == 3);
  BOOST_TEST(table.has("x"));
}

BOOST_AUTO_TEST_CASE(drop_returns_value)
{
  samarin::HashTable< std::string, int, samarin::StringHash > table;
  table.add("a", 7);
  BOOST_TEST(table.drop("a") == 7);
  BOOST_TEST(!table.has("a"));
  BOOST_TEST(table.empty());
}

BOOST_AUTO_TEST_CASE(drop_missing_throws)
{
  samarin::HashTable< std::string, int, samarin::StringHash > table;
  BOOST_CHECK_THROW(table.drop("none"), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(rehash_keeps_entries)
{
  samarin::HashTable< std::string, int, samarin::StringHash > table(2);
  table.add("a", 1);
  table.add("b", 2);
  table.rehash(8);
  table.add("c", 3);
  BOOST_TEST(table["a"] == 1);
  BOOST_TEST(table["b"] == 2);
  BOOST_TEST(table["c"] == 3);
  BOOST_TEST(table.size() == 3u);
}

BOOST_AUTO_TEST_CASE(add_after_drop)
{
  samarin::HashTable< std::string, int, samarin::StringHash > table(2);
  table.add("a", 1);
  table.drop("a");
  table.add("b", 2);
  BOOST_TEST(table["b"] == 2);
  BOOST_TEST(!table.has("a"));
}

BOOST_AUTO_TEST_CASE(iteration_visits_all)
{
  samarin::HashTable< std::string, int, samarin::StringHash > table;
  table.add("a", 1);
  table.add("b", 2);
  table.add("c", 3);
  int sum = 0;
  std::size_t count = 0;
  for (auto it = table.cbegin(); it != table.cend(); ++it) {
    sum += it->second;
    ++count;
  }
  BOOST_TEST(sum == 6);
  BOOST_TEST(count == 3u);
}

BOOST_AUTO_TEST_SUITE_END()
