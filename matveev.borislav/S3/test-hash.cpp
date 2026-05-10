#define BOOST_TEST_MODULE hash_table_tests
#include <boost/test/included/unit_test.hpp>

#include "hash-table.hpp"

#include <cstddef>
#include <stdexcept>
#include <string>

namespace
{
struct IntHash
{
  size_t operator()(int value) const
  {
    return static_cast< size_t >(value);
  }
};

struct IntEqual
{
  bool operator()(int lhs, int rhs) const
  {
    return lhs == rhs;
  }
};
}

BOOST_AUTO_TEST_CASE(add_and_find_value)
{
  matveev::HashTable< int, std::string, IntHash, IntEqual > table(5, 2);

  table.add(1, "one");
  table.add(2, "two");

  BOOST_TEST(table.has(1));
  BOOST_TEST(table.has(2));
  BOOST_TEST(!table.has(3));

  BOOST_TEST(table.at(1) == "one");
  BOOST_TEST(table.at(2) == "two");

  BOOST_TEST(table.size() == 2);
}

BOOST_AUTO_TEST_CASE(duplicate_key_throws)
{
  matveev::HashTable< int, std::string, IntHash, IntEqual > table(5, 2);

  table.add(1, "one");

  BOOST_CHECK_THROW(table.add(1, "another"), std::logic_error);
}

BOOST_AUTO_TEST_CASE(drop_removes_value)
{
  matveev::HashTable< int, std::string, IntHash, IntEqual > table(5, 2);

  table.add(1, "one");
  table.add(2, "two");

  BOOST_TEST(table.drop(1) == "one");

  BOOST_TEST(!table.has(1));
  BOOST_TEST(table.has(2));

  BOOST_TEST(table.size() == 1);
}

BOOST_AUTO_TEST_CASE(clear_removes_all_values)
{
  matveev::HashTable< int, std::string, IntHash, IntEqual > table(5, 2);

  table.add(1, "one");
  table.add(2, "two");

  table.clear();

  BOOST_TEST(table.size() == 0);

  BOOST_TEST(!table.has(1));
  BOOST_TEST(!table.has(2));
}
