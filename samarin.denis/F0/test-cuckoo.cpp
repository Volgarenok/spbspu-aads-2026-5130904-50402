#include <boost/test/unit_test.hpp>
#include <string>
#include "cuckoo_table.hpp"
#include "hashers.hpp"

using Table = samarin::CuckooTable< std::string, int, samarin::StringHash >;

BOOST_AUTO_TEST_SUITE(cuckoo)

BOOST_AUTO_TEST_CASE(insert_and_lookup)
{
  Table table;
  table.add("one", 1);
  table.add("two", 2);
  BOOST_TEST(table.size() == 2u);
  BOOST_TEST(table.at("one") == 1);
  BOOST_TEST(table.at("two") == 2);
  BOOST_TEST(table.has("one"));
  BOOST_TEST(!table.has("three"));
}

BOOST_AUTO_TEST_CASE(overwrite_keeps_size)
{
  Table table;
  table.add("key", 1);
  table.add("key", 5);
  BOOST_TEST(table.size() == 1u);
  BOOST_TEST(table.at("key") == 5);
}

BOOST_AUTO_TEST_CASE(subscript_creates)
{
  Table table;
  table["a"] += 3;
  table["a"] += 4;
  BOOST_TEST(table.at("a") == 7);
}

BOOST_AUTO_TEST_CASE(drop_removes)
{
  Table table;
  table.add("x", 1);
  table.add("y", 2);
  BOOST_TEST(table.drop("x") == 1);
  BOOST_TEST(!table.has("x"));
  BOOST_TEST(table.has("y"));
  BOOST_TEST(table.size() == 1u);
}

BOOST_AUTO_TEST_CASE(missing_throws)
{
  Table table;
  BOOST_CHECK_THROW(table.at("missing"), std::out_of_range);
  BOOST_CHECK_THROW(table.drop("missing"), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(grows_under_load)
{
  Table table;
  for (int i = 0; i < 500; ++i) {
    table.add("k" + std::to_string(i), i);
  }
  BOOST_TEST(table.size() == 500u);
  for (int i = 0; i < 500; ++i) {
    BOOST_TEST(table.at("k" + std::to_string(i)) == i);
  }
}

BOOST_AUTO_TEST_CASE(copy_is_independent)
{
  Table table;
  table.add("a", 1);
  Table copy = table;
  copy.add("a", 9);
  BOOST_TEST(table.at("a") == 1);
  BOOST_TEST(copy.at("a") == 9);
}

BOOST_AUTO_TEST_SUITE_END()
