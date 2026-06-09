#include <stdexcept>
#define BOOST_TEST_MODULE CCHTableTest
#include <boost/test/included/unit_test.hpp>
#include <hasher.hpp>
#include "CCHTable.hpp"

BOOST_AUTO_TEST_CASE(add_test)
{
  lavrentev::CCHTable<std::string, size_t, Siphash< std::string >, std::equal_to<std::string>> hs{};
  hs.add("a", 1);
  BOOST_TEST(hs["a"] == 1);
  hs.add("a", 1);
  BOOST_TEST(hs["a"] == 1);
  BOOST_TEST(hs.size() == 1);
}

BOOST_AUTO_TEST_CASE(drop_test)
{
  lavrentev::CCHTable<std::string, size_t, Siphash< std::string >, std::equal_to<std::string>> hs{};
  BOOST_CHECK_THROW(hs.drop("a"), std::out_of_range);
  hs.add("a", 1);
  hs.drop("a");
  BOOST_TEST(hs.size() == 0);
}

BOOST_AUTO_TEST_CASE(size_test)
{
  lavrentev::CCHTable<std::string, size_t, Siphash< std::string >, std::equal_to<std::string>> hs{};
  BOOST_TEST(hs.size() == 0);
  hs.add("a", 1);
  BOOST_TEST(hs.size() == 1);
}

BOOST_AUTO_TEST_CASE(has_test)
{
  lavrentev::CCHTable<std::string, size_t, Siphash< std::string >, std::equal_to<std::string>> hs{};
  BOOST_TEST(hs.has("a") == false);
  hs.add("a", 1);
  BOOST_TEST(hs.has("a") == true);
}

BOOST_AUTO_TEST_CASE(rehash_test)
{
  lavrentev::CCHTable<std::string, size_t, Siphash< std::string >, std::equal_to<std::string>> hs{};
  hs.add("a", 1);
  hs.add("b", 2);
  hs.add("c", 3);
  BOOST_CHECK_THROW(hs.rehash(0), std::invalid_argument);
  hs.rehash(10);
  BOOST_TEST(hs.getCapacity() == 20);
}

BOOST_AUTO_TEST_CASE(getCapacity_test)
{
  lavrentev::CCHTable<std::string, size_t, Siphash< std::string >, std::equal_to<std::string>> hs{};
  BOOST_TEST(hs.getCapacity() == 10);
  hs.rehash(10);
  BOOST_TEST(hs.getCapacity() == 20);
}

BOOST_AUTO_TEST_CASE(print_test)
{
  lavrentev::CCHTable<std::string, size_t, Siphash< std::string >, std::equal_to<std::string>> hs{};
  hs.add("a", 1);
  hs.add("b", 2);
  hs.add("c", 3);
  std::stringstream output;
  hs.print(output);
  std::string result = output.str();
  BOOST_CHECK(result.find("a: 1") != std::string::npos);
  BOOST_CHECK(result.find("b: 2") != std::string::npos);
  BOOST_CHECK(result.find("c: 3") != std::string::npos);
}
