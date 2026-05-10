#include <boost/test/unit_test.hpp>
#include "HashTable/HashTable.hpp"
#include "HashTable/HashFunction.hpp"
#include <string>
#include <utility>

namespace af = afanasev;

BOOST_AUTO_TEST_SUITE(HashTableStringInt)

BOOST_AUTO_TEST_CASE(test_copy_constructor)
{
  af::HashTable< std::string, int, af::Hasher< std::string >, std::equal_to< std::string > > ht(16);
  ht.add("one", 1);
  ht.add("two", 2);

  af::HashTable< std::string, int, af::Hasher< std::string >, std::equal_to< std::string > > copy(ht);
  BOOST_CHECK_EQUAL(copy.size(), 2);
  BOOST_CHECK(copy.has("one"));
  BOOST_CHECK(copy.has("two"));

  BOOST_CHECK_EQUAL(ht.size(), 2);
  BOOST_CHECK(ht.has("one"));
}

BOOST_AUTO_TEST_CASE(test_move_constructor)
{
  af::HashTable< std::string, int, af::Hasher< std::string >, std::equal_to< std::string > > ht(16);
  ht.add("one", 1);
  ht.add("two", 2);

  af::HashTable<std::string, int, af::Hasher<std::string>, std::equal_to<std::string> > moved(std::move(ht));
  BOOST_CHECK_EQUAL(moved.size(), 2);
  BOOST_CHECK(moved.has("one"));
  BOOST_CHECK(moved.has("two"));

  BOOST_CHECK(ht.empty());
}

BOOST_AUTO_TEST_CASE(test_copy_assignment)
{
  af::HashTable<std::string, int, af::Hasher<std::string>, std::equal_to<std::string> > ht1(16);
  ht1.add("one", 1);
  ht1.add("two", 2);

  af::HashTable<std::string, int, af::Hasher<std::string>, std::equal_to<std::string> > ht2(8);
  ht2.add("three", 3);

  ht2 = ht1;
  BOOST_CHECK_EQUAL(ht2.size(), 2);
  BOOST_CHECK(ht2.has("one"));
  BOOST_CHECK(ht2.has("two"));
  BOOST_CHECK(!ht2.has("three"));

  ht2 = ht2;
  BOOST_CHECK_EQUAL(ht2.size(), 2);
}

BOOST_AUTO_TEST_CASE(test_move_assignment)
{
  af::HashTable<std::string, int, af::Hasher<std::string>, std::equal_to<std::string> > ht1(16);
  ht1.add("one", 1);
  ht1.add("two", 2);

  af::HashTable<std::string, int, af::Hasher<std::string>, std::equal_to<std::string> > ht2(8);
  ht2.add("three", 3);

  ht2 = std::move(ht1);
  BOOST_CHECK_EQUAL(ht2.size(), 2);
  BOOST_CHECK(ht2.has("one"));
  BOOST_CHECK(ht2.has("two"));
  BOOST_CHECK(!ht2.has("three"));
  BOOST_CHECK(ht1.empty());
}

BOOST_AUTO_TEST_CASE(test_swap)
{
  af::HashTable<std::string, int, af::Hasher<std::string>, std::equal_to<std::string> > ht1(16);
  ht1.add("a", 1);
  ht1.add("b", 2);

  af::HashTable<std::string, int, af::Hasher<std::string>, std::equal_to<std::string> > ht2(8);
  ht2.add("c", 3);

  ht1.swap(ht2);
  BOOST_CHECK_EQUAL(ht1.size(), 1);
  BOOST_CHECK(ht1.has("c"));
  BOOST_CHECK(!ht1.has("a"));
  BOOST_CHECK_EQUAL(ht2.size(), 2);
  BOOST_CHECK(ht2.has("a"));
  BOOST_CHECK(ht2.has("b"));
}

BOOST_AUTO_TEST_CASE(test_constructor)
{
  af::HashTable<std::string, int, af::Hasher<std::string>, std::equal_to<std::string> > ht(16);
  BOOST_CHECK(ht.empty());
  BOOST_CHECK_EQUAL(ht.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_add_and_size)
{
  af::HashTable<std::string, int, af::Hasher<std::string>, std::equal_to<std::string> > ht(16);
  ht.add("one", 1);
  BOOST_CHECK_EQUAL(ht.size(), 1);
  BOOST_CHECK(!ht.empty());

  ht.add("two", 2);
  BOOST_CHECK_EQUAL(ht.size(), 2);
}

BOOST_AUTO_TEST_CASE(test_has)
{
  af::HashTable<std::string, int, af::Hasher<std::string>, std::equal_to<std::string> > ht(16);
  ht.add("one", 1);
  ht.add("two", 2);

  BOOST_CHECK(ht.has("one"));
  BOOST_CHECK(ht.has("two"));
  BOOST_CHECK(!ht.has("three"));
}

BOOST_AUTO_TEST_CASE(test_drop)
{
  af::HashTable<std::string, int, af::Hasher<std::string>, std::equal_to<std::string> > ht(16);
  ht.add("one", 1);
  ht.add("two", 2);

  int val = ht.drop("one");
  BOOST_CHECK_EQUAL(val, 1);
  BOOST_CHECK_EQUAL(ht.size(), 1);
  BOOST_CHECK(!ht.has("one"));
  BOOST_CHECK(ht.has("two"));

  BOOST_CHECK_THROW(ht.drop("missing"), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(test_add_update_existing)
{
  af::HashTable<std::string, int, af::Hasher<std::string>, std::equal_to<std::string> > ht(16);
  ht.add("key", 1);
  ht.add("key", 2);

  int val = ht.drop("key");
  BOOST_CHECK_EQUAL(val, 2);
  BOOST_CHECK_EQUAL(ht.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_clear)
{
  af::HashTable<std::string, int, af::Hasher<std::string>, std::equal_to<std::string> > ht(16);
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

BOOST_AUTO_TEST_CASE(test_rehash)
{
  af::HashTable<std::string, int, af::Hasher<std::string>, std::equal_to<std::string> > ht(4);
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

  ht.rehash(2);
  BOOST_CHECK_EQUAL(ht.size(), 5);
  BOOST_CHECK(ht.has("one"));
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(HashTableIntInt)

BOOST_AUTO_TEST_CASE(test_copy_move_swap)
{
  af::HashTable<int, int, std::hash<int>, std::equal_to<int> > ht1(16);
  ht1.add(1, 100);
  ht1.add(2, 200);

  af::HashTable<int, int, std::hash<int>, std::equal_to<int> > ht2(ht1);
  BOOST_CHECK_EQUAL(ht2.size(), 2);
  BOOST_CHECK(ht2.has(1));
  BOOST_CHECK(ht2.has(2));


  af::HashTable<int, int, std::hash<int>, std::equal_to<int> > ht3(std::move(ht1));
  BOOST_CHECK_EQUAL(ht3.size(), 2);
  BOOST_CHECK(ht3.has(1));
  BOOST_CHECK(ht3.has(2));
  BOOST_CHECK(ht1.empty());

  af::HashTable<int, int, std::hash<int>, std::equal_to<int> > ht4(4);
  ht4 = ht2;
  BOOST_CHECK_EQUAL(ht4.size(), 2);
  BOOST_CHECK(ht4.has(1));
  BOOST_CHECK(ht4.has(2));

  af::HashTable<int, int, std::hash<int>, std::equal_to<int> > ht5(4);
  ht5 = std::move(ht2);
  BOOST_CHECK_EQUAL(ht5.size(), 2);
  BOOST_CHECK(ht5.has(1));
  BOOST_CHECK(ht5.has(2));
  BOOST_CHECK(ht2.empty());

  ht4.swap(ht5);
  BOOST_CHECK_EQUAL(ht4.size(), 2);
  BOOST_CHECK_EQUAL(ht5.size(), 2);
}

BOOST_AUTO_TEST_CASE(test_int_keys)
{
  af::HashTable<int, int, std::hash<int>, std::equal_to<int> > ht(8);
  BOOST_CHECK(ht.empty());

  ht.add(10, 100);
  ht.add(20, 200);
  BOOST_CHECK_EQUAL(ht.size(), 2);
  BOOST_CHECK(ht.has(10));
  BOOST_CHECK(ht.has(20));

  ht.add(20, 250);
  int val = ht.drop(20);
  BOOST_CHECK_EQUAL(val, 250);
  BOOST_CHECK_EQUAL(ht.size(), 1);
  BOOST_CHECK(!ht.has(20));
  BOOST_CHECK(ht.has(10));
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(HashTablePairString)

BOOST_AUTO_TEST_CASE(test_copy_move_swap)
{
  typedef std::pair<std::string, std::string> Key;
  af::HashTable<Key, int, af::PairHasher<std::string>, std::equal_to<Key> > ht1(8);
  Key k1("A", "B");
  Key k2("C", "D");
  ht1.add(k1, 10);
  ht1.add(k2, 20);

  af::HashTable<Key, int, af::PairHasher<std::string>, std::equal_to<Key> > ht2(ht1);
  BOOST_CHECK_EQUAL(ht2.size(), 2);
  BOOST_CHECK(ht2.has(k1));
  BOOST_CHECK(ht2.has(k2));

  af::HashTable<Key, int, af::PairHasher<std::string>, std::equal_to<Key> > ht3(std::move(ht1));
  BOOST_CHECK_EQUAL(ht3.size(), 2);
  BOOST_CHECK(ht3.has(k1));
  BOOST_CHECK(ht3.has(k2));
  BOOST_CHECK(ht1.empty());

  af::HashTable<Key, int, af::PairHasher<std::string>, std::equal_to<Key> > ht4(4);
  ht4 = ht2;
  BOOST_CHECK_EQUAL(ht4.size(), 2);
  BOOST_CHECK(ht4.has(k1));
  BOOST_CHECK(ht4.has(k2));
}

BOOST_AUTO_TEST_CASE(test_pair_keys)
{
  typedef std::pair<std::string, std::string> Key;
  af::HashTable<Key, int, af::PairHasher<std::string>, std::equal_to<Key> > ht(8);

  Key e1 = std::make_pair("A", "B");
  Key e2 = std::make_pair("B", "C");
  Key e3 = std::make_pair("A", "B");

  ht.add(e1, 10);
  ht.add(e2, 20);
  ht.add(e3, 30);

  BOOST_CHECK_EQUAL(ht.size(), 2);
  BOOST_CHECK(ht.has(e1));
  BOOST_CHECK(ht.has(e2));

  int val = ht.drop(e1);
  BOOST_CHECK_EQUAL(val, 30);
  BOOST_CHECK_EQUAL(ht.size(), 1);
  BOOST_CHECK(!ht.has(e1));
  BOOST_CHECK(ht.has(e2));
}

BOOST_AUTO_TEST_SUITE_END()
