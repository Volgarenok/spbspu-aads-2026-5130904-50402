#include "hashtable.hpp"
#include "hasher.hpp"
#include <boost/test/unit_test.hpp>
#include <functional>

using namespace khalikov;
using h_t = HashTable< int, int, SipHash< int >, std::equal_to< int > >;

BOOST_AUTO_TEST_SUITE(HashTableTests)

BOOST_AUTO_TEST_CASE(default_constructor_test)
{
  h_t ht;
  BOOST_CHECK(ht.isEmpty());
  BOOST_CHECK_EQUAL(ht.getCap(), 8);
  BOOST_CHECK_EQUAL(ht.getSize(), 0);
}

BOOST_AUTO_TEST_CASE(copy_constructor_test)
{
  h_t ht;
  h_t yaht(ht);
  BOOST_CHECK(yaht.isEmpty());
  BOOST_CHECK_EQUAL(yaht.getSize(), ht.getSize());
  BOOST_CHECK_EQUAL(yaht.getCap(), ht.getCap());
}

BOOST_AUTO_TEST_CASE(move_constructor_test)
{
  h_t ht;
  ht.insert(1, 1);
  BOOST_CHECK_EQUAL(ht.getSize(), 1);
  h_t yaht;
  BOOST_CHECK_EQUAL(yaht.getSize(), 0);
  yaht = std::move(ht);
  BOOST_CHECK(ht.isEmpty());
  BOOST_CHECK_EQUAL(yaht.getSize(), 1);
}

BOOST_AUTO_TEST_CASE(copy_op_test)
{
  h_t ht1;
  ht1.insert(1, 10);
  ht1.insert(2, 20);
  h_t ht2;
  ht2.insert(3, 30);
  ht2 = ht1;
  BOOST_CHECK_EQUAL(ht2.getSize(), 2);
  BOOST_CHECK_EQUAL(ht2.at(1), 10);
  BOOST_CHECK_EQUAL(ht2.at(2), 20);
}

BOOST_AUTO_TEST_CASE(move_op_test)
{
  h_t ht1;
  ht1.insert(1, 10);
  size_t old_cap = ht1.getCap();
  h_t ht2;
  ht2 = std::move(ht1);
  BOOST_CHECK_EQUAL(ht2.getSize(), 1);
  BOOST_CHECK_EQUAL(ht2.at(1), 10);
  BOOST_CHECK_EQUAL(ht2.getCap(), old_cap);
  BOOST_CHECK_EQUAL(ht1.getSize(), 0);
}

BOOST_AUTO_TEST_CASE(insert_and_find_test)
{
  h_t ht;
  BOOST_CHECK(ht.insert(1, 10));
  BOOST_CHECK(ht.insert(2, 20));
  BOOST_CHECK(!ht.insert(1, 100));
  BOOST_CHECK_EQUAL(ht.getSize(), 2);
  BOOST_CHECK_EQUAL(ht.at(1), 100);
  BOOST_CHECK_EQUAL(ht.at(2), 20);
  auto it = ht.find(2);
  BOOST_CHECK(it != ht.end());
  BOOST_CHECK_EQUAL(it->value, 20);
}

BOOST_AUTO_TEST_CASE(remove_test)
{
  h_t ht;
  ht.insert(1, 10);
  ht.insert(2, 20);
  BOOST_CHECK_EQUAL(ht.getSize(), 2);
  BOOST_CHECK(ht.remove(1));
  BOOST_CHECK_EQUAL(ht.getSize(), 1);
}

BOOST_AUTO_TEST_CASE(op_square_brackets_test)
{
  h_t ht;
  ht[1] = 10;
  BOOST_CHECK_EQUAL(ht.getSize(), 1);
  BOOST_CHECK_EQUAL(ht.at(1), 10);
  ht[1] = 20;
  BOOST_CHECK_EQUAL(ht.at(1), 20);
  BOOST_CHECK_EQUAL(ht.getSize(), 1);
}

BOOST_AUTO_TEST_CASE(rehash_test)
{
  h_t ht;
  size_t initial_cap = ht.getCap();
  for (int i = 0; i < 10; ++i) {
    ht.insert(i, i * i);
  }
  BOOST_CHECK(ht.getCap() > initial_cap);
  BOOST_CHECK_EQUAL(ht.getSize(), 10);
  for (int i = 0; i < 10; ++i) {
    BOOST_CHECK_EQUAL(ht.at(i), i * i);
  }
}

BOOST_AUTO_TEST_CASE(iterator_test)
{
  h_t ht;
  ht.insert(1, 10);
  ht.insert(2, 20);
  ht.insert(3, 30);
  size_t count = 0;
  for (auto it = ht.cbegin(); it != ht.cend(); ++it) {
    count++;
  }
  BOOST_CHECK_EQUAL(count, 3);
}

BOOST_AUTO_TEST_SUITE_END()
