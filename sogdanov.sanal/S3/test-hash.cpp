#include <stdexcept>
#include <string>

#include <boost/test/unit_test.hpp>

#include "Hashtable.hpp"

BOOST_AUTO_TEST_SUITE(HashTableTests)

BOOST_AUTO_TEST_CASE(test_default_constructor)
{
  sogdanov::HashTable< std::string, int > ht(16);
  BOOST_CHECK_EQUAL(ht.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_add_and_size)
{
  sogdanov::HashTable< std::string, int > ht(16);
  ht.add("one", 1);
  BOOST_CHECK_EQUAL(ht.size(), 1);

  ht.add("two", 2);
  BOOST_CHECK_EQUAL(ht.size(), 2);
}

BOOST_AUTO_TEST_CASE(test_has)
{
  sogdanov::HashTable< std::string, int > ht(16);
  ht.add("one", 1);
  ht.add("two", 2);

  BOOST_CHECK(ht.has("one"));
  BOOST_CHECK(ht.has("two"));
  BOOST_CHECK(!ht.has("three"));
}

BOOST_AUTO_TEST_CASE(test_get_and_modify)
{
  sogdanov::HashTable< std::string, int > ht(16);
  ht.add("one", 1);
  ht.add("two", 2);

  BOOST_CHECK_EQUAL(ht.get("one"), 1);
  BOOST_CHECK_EQUAL(ht.get("two"), 2);

  ht.get("one") = 10;
  BOOST_CHECK_EQUAL(ht.get("one"), 10);
}

BOOST_AUTO_TEST_CASE(test_drop)
{
  sogdanov::HashTable< std::string, int > ht(16);
  ht.add("one", 1);
  ht.add("two", 2);

  int val = ht.drop("one");
  BOOST_CHECK_EQUAL(val, 1);
  BOOST_CHECK_EQUAL(ht.size(), 1);

  BOOST_CHECK(!ht.has("one"));
  BOOST_CHECK(ht.has("two"));
}

BOOST_AUTO_TEST_CASE(test_add_update_existing)
{
  sogdanov::HashTable< std::string, int > ht(16);
  ht.add("key", 1);
  ht.add("key", 2);

  BOOST_CHECK_EQUAL(ht.size(), 1);
  BOOST_CHECK_EQUAL(ht.get("key"), 2);
}

BOOST_AUTO_TEST_CASE(test_copy_constructor)
{
  sogdanov::HashTable< std::string, int > ht1(16);
  ht1.add("one", 1);
  ht1.add("two", 2);

  sogdanov::HashTable< std::string, int > ht2 = ht1;

  BOOST_CHECK_EQUAL(ht2.size(), 2);
  BOOST_CHECK(ht2.has("one"));
  BOOST_CHECK(ht2.has("two"));
  BOOST_CHECK_EQUAL(ht2.get("one"), 1);
}

BOOST_AUTO_TEST_CASE(test_exceptions_out_of_range)
{
  sogdanov::HashTable< std::string, int > ht(16);
  ht.add("one", 1);

  BOOST_CHECK_THROW(ht.get("two"), std::out_of_range);
  BOOST_CHECK_THROW(ht.drop("two"), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(test_exceptions_overflow)
{
  sogdanov::HashTable< std::string, int > ht(3);
  ht.add("one", 1);
  ht.add("two", 2);

  BOOST_CHECK_THROW(ht.add("three", 3), std::overflow_error);
}

BOOST_AUTO_TEST_CASE(test_rehash)
{
  sogdanov::HashTable< std::string, int > ht(4);
  ht.add("one", 1);
  ht.add("two", 2);
  ht.add("three", 3);

  ht.rehash(16);
  BOOST_CHECK_EQUAL(ht.size(), 3);

  ht.add("four", 4);
  ht.add("five", 5);

  BOOST_CHECK_EQUAL(ht.size(), 5);
  BOOST_CHECK(ht.has("one"));
  BOOST_CHECK(ht.has("five"));
}

BOOST_AUTO_TEST_CASE(test_tombstone_reuse)
{
  sogdanov::HashTable< std::string, int > ht(5);
  ht.add("one", 1);
  ht.add("two", 2);
  ht.add("three", 3);

  ht.drop("two");
  BOOST_CHECK_EQUAL(ht.size(), 2);

  ht.add("four", 4);
  BOOST_CHECK_EQUAL(ht.size(), 3);
  BOOST_CHECK(ht.has("four"));
  BOOST_CHECK(!ht.has("two"));
}

BOOST_AUTO_TEST_SUITE_END()
