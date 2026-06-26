#include <string>
#include <boost/test/unit_test.hpp>
#include "robin.hpp"

using namespace dirko;

BOOST_AUTO_TEST_SUITE(RobinTableTests)

BOOST_AUTO_TEST_CASE(test_default_constructor)
{
  RobinTable< std::string, int > rt(16, .7);
  BOOST_CHECK(rt.empty());
  BOOST_CHECK_EQUAL(rt.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_add_and_size)
{
  RobinTable< std::string, int > rt(16, .7);
  rt.add("one", 1);
  BOOST_CHECK_EQUAL(rt.size(), 1);
  BOOST_CHECK(!rt.empty());

  rt.add("two", 2);
  BOOST_CHECK_EQUAL(rt.size(), 2);
}

BOOST_AUTO_TEST_CASE(test_has)
{
  RobinTable< std::string, int > rt(16, .7);
  rt.add("one", 1);
  rt.add("two", 2);

  BOOST_CHECK(rt.has("one"));
  BOOST_CHECK(rt.has("two"));
  BOOST_CHECK(!rt.has("three"));
}

BOOST_AUTO_TEST_CASE(test_get)
{
  RobinTable< std::string, int > rt(16, .7);
  rt.add("one", 1);
  rt.add("two", 2);

  BOOST_CHECK_EQUAL(rt.get("one"), 1);
  BOOST_CHECK_EQUAL(rt.get("two"), 2);
  rt.get("one") = 10;
  BOOST_CHECK_EQUAL(rt.get("one"), 10);
}

BOOST_AUTO_TEST_CASE(test_drop)
{
  RobinTable< std::string, int > rt(16, .7);
  rt.add("one", 1);
  rt.add("two", 2);

  int val = rt.get("one");
  rt.drop("one");
  BOOST_CHECK_EQUAL(val, 1);
  BOOST_CHECK_EQUAL(rt.size(), 1);
  BOOST_CHECK(!rt.has("one"));
  BOOST_CHECK(rt.has("two"));
}

BOOST_AUTO_TEST_CASE(test_add_update_existing)
{
  RobinTable< std::string, int > rt(16, .7);
  rt.add("key", 1);
  rt.add("key", 2);

  BOOST_CHECK_EQUAL(rt.size(), 1);
  BOOST_CHECK_EQUAL(rt.get("key"), 2);
}

BOOST_AUTO_TEST_CASE(test_clear)
{
  RobinTable< std::string, int > rt(16, .7);
  rt.add("one", 1);
  rt.add("two", 2);
  rt.add("three", 3);

  rt.clear();
  BOOST_CHECK(rt.empty());
  BOOST_CHECK_EQUAL(rt.size(), 0);
  BOOST_CHECK(!rt.has("one"));
}

BOOST_AUTO_TEST_CASE(test_copy_constructor)
{
  RobinTable< std::string, int > rt1(16, .7);
  rt1.add("one", 1);
  rt1.add("two", 2);

  RobinTable< std::string, int > rt2 = rt1;

  BOOST_CHECK_EQUAL(rt2.size(), 2);
  BOOST_CHECK(rt2.has("one"));
  BOOST_CHECK(rt2.has("two"));
  BOOST_CHECK_EQUAL(rt2.get("one"), 1);
}

BOOST_AUTO_TEST_CASE(test_init_list)
{
  RobinTable< std::string, int > rt = {{"one", 1}, {"two", 2}, {"three", 3}};
  BOOST_CHECK_EQUAL(rt.size(), 3);
  BOOST_CHECK_EQUAL(rt.get("one"), 1);
  BOOST_CHECK_EQUAL(rt.get("two"), 2);
  BOOST_CHECK_EQUAL(rt.get("three"), 3);
}

BOOST_AUTO_TEST_CASE(test_move_constructor)
{
  RobinTable< std::string, int > rt1(16, .7);
  rt1.add("one", 1);
  rt1.add("two", 2);

  RobinTable< std::string, int > rt2 = std::move(rt1);

  BOOST_CHECK_EQUAL(rt2.size(), 2);
  BOOST_CHECK(rt2.has("one"));
  BOOST_CHECK(rt2.has("two"));
}

BOOST_AUTO_TEST_CASE(test_rehash)
{
  RobinTable< std::string, int > rt(10, .7);
  rt.add("one", 1);
  rt.add("two", 2);
  rt.add("three", 3);
  rt.add("four", 4);
  rt.add("five", 5);

  rt.rehash(20);
  BOOST_CHECK_EQUAL(rt.size(), 5);
  BOOST_CHECK(rt.has("one"));
  BOOST_CHECK(rt.has("two"));
  BOOST_CHECK(rt.has("three"));
  BOOST_CHECK(rt.has("four"));
  BOOST_CHECK(rt.has("five"));
}

BOOST_AUTO_TEST_CASE(test_changeKey)
{
  RobinTable< std::string, int > rt(10, .7);
  rt.add("one", 2);
  rt.changeKey("one", "two");
  BOOST_CHECK_EQUAL(rt.size(), 1);
  BOOST_CHECK(rt.has("two"));
  BOOST_CHECK(!rt.has("one"));
}

BOOST_AUTO_TEST_SUITE_END()
