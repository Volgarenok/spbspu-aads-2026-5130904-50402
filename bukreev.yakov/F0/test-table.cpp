#include <boost/test/unit_test.hpp>
#include "table.hpp"
#include <string>

using namespace bukreev;

BOOST_AUTO_TEST_SUITE(TableTests)

BOOST_AUTO_TEST_CASE(test_constructor)
{
  HashTable< std::string, int, std::hash< std::string > > t(19);

  BOOST_CHECK_EQUAL(t.capacity(), 19);
  BOOST_CHECK_EQUAL(t.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_put)
{
  HashTable< std::string, int, std::hash< std::string > > t(19);

  t.put("abc", 3);
  BOOST_CHECK_EQUAL(t.size(), 1);
  t.put("abcd", 4);
  BOOST_CHECK_EQUAL(t.size(), 2);
  t.put("abc", 5);
  BOOST_CHECK_EQUAL(t.size(), 2);

  BOOST_CHECK(t.get("abc") == 5);
  BOOST_CHECK(t.get("abcd") == 4);
}

BOOST_AUTO_TEST_CASE(test_erase)
{
  HashTable< std::string, int, std::hash< std::string > > t(19);

  t.put("abce", 3);
  t.put("abcd", 4);
  t.put("abc", 5);
  t.erase("abc");
  BOOST_CHECK_EQUAL(t.size(), 2);
  t.put("abc", 5);
  BOOST_CHECK_EQUAL(t.size(), 3);
}

BOOST_AUTO_TEST_CASE(test_exist)
{
  HashTable< std::string, int, std::hash< std::string > > t(19);

  t.put("abce", 3);
  BOOST_CHECK(t.exist("abce"));
}

BOOST_AUTO_TEST_CASE(test_rehash)
{
  HashTable< std::string, int, std::hash< std::string > > t(3);

  t.put("abc", 3);
  t.put("abcd", 4);
  t.put("abce", 5);
  t.put("abcf", 6);

  BOOST_CHECK(t.get("abcf") == 6);
  BOOST_CHECK_EQUAL(t.capacity(), 5);
}

BOOST_AUTO_TEST_SUITE_END()
