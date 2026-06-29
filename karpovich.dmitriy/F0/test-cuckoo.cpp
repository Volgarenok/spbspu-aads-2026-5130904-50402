#include <boost/test/unit_test.hpp>
#include "cuckoo.hpp"

using namespace karpovich;

BOOST_AUTO_TEST_SUITE(CuckooTableTests)

BOOST_AUTO_TEST_CASE(test_default_constructor)
{
  CuckooTable< std::string, int > ht(16);

  BOOST_CHECK(ht.empty());
  BOOST_CHECK_EQUAL(ht.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_add_and_size)
{
  CuckooTable< std::string, int > ht(16);

  ht.add("one", 1);

  BOOST_CHECK_EQUAL(ht.size(), 1);
  BOOST_CHECK(!ht.empty());

  ht.add("two", 2);

  BOOST_CHECK_EQUAL(ht.size(), 2);
}

BOOST_AUTO_TEST_CASE(test_has)
{
  CuckooTable< std::string, int > ht(16);

  ht.add("one", 1);
  ht.add("two", 2);

  BOOST_CHECK(ht.has("one"));
  BOOST_CHECK(ht.has("two"));
  BOOST_CHECK(!ht.has("three"));
}

BOOST_AUTO_TEST_CASE(test_get)
{
  CuckooTable< std::string, int > ht(16);

  ht.add("one", 1);
  ht.add("two", 2);

  BOOST_CHECK_EQUAL(ht.get("one"), 1);
  BOOST_CHECK_EQUAL(ht.get("two"), 2);

  ht.get("one") = 10;

  BOOST_CHECK_EQUAL(ht.get("one"), 10);
}

BOOST_AUTO_TEST_CASE(test_drop)
{
  CuckooTable< std::string, int > ht(16);

  ht.add("one", 1);
  ht.add("two", 2);

  ht.drop("one");

  BOOST_CHECK_EQUAL(ht.size(), 1);

  BOOST_CHECK(!ht.has("one"));
  BOOST_CHECK(ht.has("two"));
}

BOOST_AUTO_TEST_CASE(test_add_update_existing)
{
  CuckooTable< std::string, int > ht(16);

  ht.add("key", 1);
  ht.add("key", 2);

  BOOST_CHECK_EQUAL(ht.size(), 1);
  BOOST_CHECK_EQUAL(ht.get("key"), 2);
}

BOOST_AUTO_TEST_CASE(test_clear)
{
  CuckooTable< std::string, int > ht(16);

  ht.add("one", 1);
  ht.add("two", 2);
  ht.add("three", 3);

  ht.clear();

  BOOST_CHECK(ht.empty());
  BOOST_CHECK_EQUAL(ht.size(), 0);

  BOOST_CHECK(!ht.has("one"));
  BOOST_CHECK(!ht.has("two"));
  BOOST_CHECK(!ht.has("three"));
}

BOOST_AUTO_TEST_CASE(test_copy_constructor)
{
  CuckooTable< std::string, int > ht1(16);

  ht1.add("one", 1);
  ht1.add("two", 2);

  CuckooTable< std::string, int > ht2 = ht1;

  BOOST_CHECK_EQUAL(ht2.size(), 2);

  BOOST_CHECK(ht2.has("one"));
  BOOST_CHECK(ht2.has("two"));

  BOOST_CHECK_EQUAL(ht2.get("one"), 1);
  BOOST_CHECK_EQUAL(ht2.get("two"), 2);
}

BOOST_AUTO_TEST_CASE(test_move_constructor)
{
  CuckooTable< std::string, int > ht1(16);

  ht1.add("one", 1);
  ht1.add("two", 2);

  CuckooTable< std::string, int > ht2 = std::move(ht1);

  BOOST_CHECK_EQUAL(ht2.size(), 2);

  BOOST_CHECK(ht2.has("one"));
  BOOST_CHECK(ht2.has("two"));

  BOOST_CHECK(ht1.empty());
}

BOOST_AUTO_TEST_CASE(test_copy_assignment)
{
  CuckooTable< std::string, int > ht1(16);

  ht1.add("one", 1);
  ht1.add("two", 2);

  CuckooTable< std::string, int > ht2(8);

  ht2 = ht1;

  BOOST_CHECK_EQUAL(ht2.size(), 2);

  BOOST_CHECK(ht2.has("one"));
  BOOST_CHECK(ht2.has("two"));

  BOOST_CHECK_EQUAL(ht2.get("one"), 1);
  BOOST_CHECK_EQUAL(ht2.get("two"), 2);
}

BOOST_AUTO_TEST_CASE(test_move_assignment)
{
  CuckooTable< std::string, int > ht1(16);

  ht1.add("one", 1);
  ht1.add("two", 2);

  CuckooTable< std::string, int > ht2(8);

  ht2 = std::move(ht1);

  BOOST_CHECK_EQUAL(ht2.size(), 2);

  BOOST_CHECK(ht2.has("one"));
  BOOST_CHECK(ht2.has("two"));

  BOOST_CHECK(ht1.empty());
}

BOOST_AUTO_TEST_CASE(test_rehash)
{
  CuckooTable< std::string, int > ht(4);

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

BOOST_AUTO_TEST_CASE(test_load_factor)
{
  CuckooTable< std::string, int > ht(10);

  ht.add("one", 1);
  ht.add("two", 2);

  BOOST_CHECK_CLOSE(ht.loadFactor(), 0.1, 0.001);
}

BOOST_AUTO_TEST_CASE(test_iterator)
{
  CuckooTable< std::string, int > ht(16);

  ht.add("one", 1);
  ht.add("two", 2);
  ht.add("three", 3);

  size_t count = 0;

  for (auto it = ht.begin(); it != ht.end(); ++it) {
    ++count;
  }

  BOOST_CHECK_EQUAL(count, 3);
}

BOOST_AUTO_TEST_CASE(test_const_iterator)
{
  CuckooTable< std::string, int > ht(16);

  ht.add("one", 1);
  ht.add("two", 2);

  const CuckooTable< std::string, int > &cht = ht;

  size_t count = 0;

  for (auto it = cht.begin(); it != cht.end(); ++it) {
    ++count;
  }

  BOOST_CHECK_EQUAL(count, 2);
}

BOOST_AUTO_TEST_CASE(test_iterator_operator_arrow)
{
  CuckooTable< std::string, int > ht(16);

  ht.add("one", 1);

  auto it = ht.begin();

  BOOST_CHECK_EQUAL(it->first, "one");
  BOOST_CHECK_EQUAL(it->second, 1);
}

BOOST_AUTO_TEST_CASE(test_get_nonexistent)
{
  CuckooTable< std::string, int > ht(16);

  BOOST_CHECK_THROW(ht.get("missing"), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(test_drop_nonexistent)
{
  CuckooTable< std::string, int > ht(16);

  BOOST_CHECK_THROW(ht.drop("missing"), std::out_of_range);
}

BOOST_AUTO_TEST_SUITE_END()
