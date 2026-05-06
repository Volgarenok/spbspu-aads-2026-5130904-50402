#include <stdexcept>
#define BOOST_TEST_MODULE StackQueueTest
#include <boost/test/included/unit_test.hpp>
#include "HashTable.hpp"

BOOST_AUTO_TEST_CASE(HashTable_add_test)
{
  lavrentev::HashTable<std::string, size_t, Siphash< std::string >, std::equal_to<std::string>> hs{};
  hs.add("a", 1);
  BOOST_TEST(hs["a"] == 1);
}
