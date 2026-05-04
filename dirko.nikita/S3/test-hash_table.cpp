#include <boost/test/unit_test.hpp>
#include "hash-table.hpp"

using namespace dirko;

BOOST_AUTO_TEST_SUITE(HashTableTests)

BOOST_AUTO_TEST_CASE(test_default_constructor)
{
  HashTable< std::string, int > ht(16);
  BOOST_CHECK(ht.empty());
  BOOST_CHECK_EQUAL(ht.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_add_and_size)
{
  HashTable< std::string, int > ht(16);
  ht.add("one", 1);
  BOOST_CHECK_EQUAL(ht.size(), 1);
  BOOST_CHECK(!ht.empty());

  ht.add("two", 2);
  BOOST_CHECK_EQUAL(ht.size(), 2);
}

BOOST_AUTO_TEST_CASE(test_has)
{
  HashTable< std::string, int > ht(16);
  ht.add("one", 1);
  ht.add("two", 2);

  BOOST_CHECK(ht.has("one"));
  BOOST_CHECK(ht.has("two"));
  BOOST_CHECK(!ht.has("three"));
}

BOOST_AUTO_TEST_CASE(test_get)
{
  HashTable< std::string, int > ht(16);
  ht.add("one", 1);
  ht.add("two", 2);

  BOOST_CHECK_EQUAL(ht.get("one"), 1);
  BOOST_CHECK_EQUAL(ht.get("two"), 2);
}

BOOST_AUTO_TEST_CASE(test_rewrite)
{
  HashTable< std::string, int > ht(16);
  ht.add("one", 1);
  ht.add("two", 2);

  BOOST_CHECK_EQUAL(ht.get("one"), 1);
  BOOST_CHECK_EQUAL(ht.get("two"), 2);

  ht.rewrite("one", 10);
  BOOST_CHECK_EQUAL(ht.get("one"), 10);
}
BOOST_AUTO_TEST_CASE(test_drop)
{
  HashTable< std::string, int > ht(16);
  ht.add("one", 1);
  ht.add("two", 2);

  int val = ht.get("one");
  ht.drop("one");
  BOOST_CHECK_EQUAL(val, 1);
  BOOST_CHECK_EQUAL(ht.size(), 1);
  BOOST_CHECK(!ht.has("one"));
  BOOST_CHECK(ht.has("two"));
}

BOOST_AUTO_TEST_CASE(test_add_update_existing)
{
  HashTable< std::string, int > ht(16);
  ht.add("key", 1);
  ht.add("key", 2);

  BOOST_CHECK_EQUAL(ht.size(), 1);
  BOOST_CHECK_EQUAL(ht.get("key"), 2);
}

BOOST_AUTO_TEST_CASE(test_clear)
{
  HashTable< std::string, int > ht(16);
  ht.add("one", 1);
  ht.add("two", 2);
  ht.add("three", 3);

  ht.clear();
  BOOST_CHECK(ht.empty());
  BOOST_CHECK_EQUAL(ht.size(), 0);
  BOOST_CHECK(!ht.has("one"));
}

BOOST_AUTO_TEST_CASE(test_copy_constructor)
{
  HashTable< std::string, int > ht1(16);
  ht1.add("one", 1);
  ht1.add("two", 2);

  HashTable< std::string, int > ht2 = ht1;

  BOOST_CHECK_EQUAL(ht2.size(), 2);
  BOOST_CHECK(ht2.has("one"));
  BOOST_CHECK(ht2.has("two"));
  BOOST_CHECK_EQUAL(ht2.get("one"), 1);
}

BOOST_AUTO_TEST_CASE(test_move_constructor)
{
  HashTable< std::string, int > ht1(16);
  ht1.add("one", 1);
  ht1.add("two", 2);

  HashTable< std::string, int > ht2 = std::move(ht1);

  BOOST_CHECK_EQUAL(ht2.size(), 2);
  BOOST_CHECK(ht2.has("one"));
  BOOST_CHECK(ht2.has("two"));
  BOOST_CHECK(ht1.empty());
}

BOOST_AUTO_TEST_CASE(test_rehash)
{
  HashTable< std::string, int > ht(4);
  ht.add("one", 1);
  ht.add("two", 2);
  ht.add("three", 3);
  ht.add("four", 4);
  ht.add("five", 5);

  ht.rehash(16);
  BOOST_CHECK_EQUAL(ht.size(), 5);
  BOOST_CHECK(ht.has("one"));
  BOOST_CHECK(ht.has("two"));
  BOOST_CHECK(ht.has("three"));
  BOOST_CHECK(ht.has("four"));
  BOOST_CHECK(ht.has("five"));
}

BOOST_AUTO_TEST_SUITE_END()
