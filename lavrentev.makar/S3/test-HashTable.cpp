#include <stdexcept>
#define BOOST_TEST_MODULE StackQueueTest
#include <boost/test/included/unit_test.hpp>
#include "HashTable.hpp"

BOOST_AUTO_TEST_CASE(add_test)
{
  lavrentev::HashTable<std::string, size_t, Siphash< std::string >, std::equal_to<std::string>> hs{};
  hs.add("a", 1);
  BOOST_TEST(hs["a"] == 1);
  BOOST_CHECK_THROW(hs.add("a", 1), std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(drop_test)
{
  lavrentev::HashTable<std::string, size_t, Siphash< std::string >, std::equal_to<std::string>> hs{};
  BOOST_CHECK_THROW(hs.drop("a"), std::invalid_argument);
  hs.add("a", 1);
  hs.drop("a");
  BOOST_TEST(hs.size() == 0);
}

BOOST_AUTO_TEST_CASE(size_test)
{
  lavrentev::HashTable<std::string, size_t, Siphash< std::string >, std::equal_to<std::string>> hs{};
  BOOST_TEST(hs.size() == 0);
  hs.add("a", 1);
  BOOST_TEST(hs.size() == 1);
}

BOOST_AUTO_TEST_CASE(has_test)
{
  lavrentev::HashTable<std::string, size_t, Siphash< std::string >, std::equal_to<std::string>> hs{};
  BOOST_TEST(hs.has("a") == false);
  hs.add("a", 1);
  BOOST_TEST(hs.has("a") == true);
}
