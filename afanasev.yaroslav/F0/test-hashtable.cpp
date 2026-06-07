#include <boost/test/unit_test.hpp>
#include "CuckooHashTable/CuckooHashTable.hpp"
#include "CuckooHashTable/CuckooHashIter.hpp"
#include <string>
#include <utility>

using namespace afanasev;

BOOST_AUTO_TEST_SUITE(test_cuckoo_hash_table)

BOOST_AUTO_TEST_CASE(test_add_has_get)
{
  CuckooHashTable<int, std::string> table(4);
  BOOST_CHECK(table.empty());
  BOOST_CHECK_EQUAL(table.size(), 0);

  table.add(1, "one");
  table.add(2, "two");
  table.add(3, "three");

  BOOST_CHECK(!table.empty());
  BOOST_CHECK_EQUAL(table.size(), 3);
  BOOST_CHECK(table.has(1));
  BOOST_CHECK(table.has(2));
  BOOST_CHECK(table.has(3));
  BOOST_CHECK(!table.has(4));

  BOOST_CHECK_EQUAL(table.get(1), "one");
  BOOST_CHECK_EQUAL(table.get(2), "two");
  BOOST_CHECK_EQUAL(table.get(3), "three");

  table.add(2, "TWO");
  BOOST_CHECK_EQUAL(table.get(2), "TWO");
  BOOST_CHECK_EQUAL(table.size(), 3);
}

BOOST_AUTO_TEST_CASE(test_throws_on_not_found)
{
  CuckooHashTable<int, double> table(2);
  table.add(10, 1.23);
  BOOST_CHECK_EQUAL(table.get(10), 1.23);

  BOOST_CHECK_THROW(table.get(20), std::out_of_range);
  BOOST_CHECK_THROW(table.drop(30), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(test_drop)
{
  CuckooHashTable<std::string, int> table(4);
  table.add("a", 100);
  table.add("b", 200);
  table.add("c", 300);

  int val = table.drop("b");
  BOOST_CHECK_EQUAL(val, 200);
  BOOST_CHECK(!table.has("b"));
  BOOST_CHECK_EQUAL(table.size(), 2);
  BOOST_CHECK_EQUAL(table.get("a"), 100);
  BOOST_CHECK_EQUAL(table.get("c"), 300);

  BOOST_CHECK_THROW(table.drop("b"), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(test_rehash)
{
  CuckooHashTable<int, int> table(4);
  const int N = 100;
  for (int i = 0; i < N; ++i)
  {
    table.add(i, i * i);
  }
  BOOST_CHECK_EQUAL(table.size(), N);
  for (int i = 0; i < N; ++i)
  {
    BOOST_CHECK(table.has(i));
    BOOST_CHECK_EQUAL(table.get(i), i * i);
  }
}

BOOST_AUTO_TEST_CASE(test_iterators)
{
  CuckooHashTable<char, int> table(8);
  table.add('a', 1);
  table.add('b', 2);
  table.add('c', 3);
  table.add('d', 4);

  size_t count = 0;
  for (auto it = table.begin(); it != table.end(); ++it)
  {
    ++count;
    char key = (*it).first;
    int val = (*it).second;
    BOOST_CHECK((key == 'a' && val == 1) ||
      (key == 'b' && val == 2) ||
      (key == 'c' && val == 3) ||
      (key == 'd' && val == 4));
  }
  BOOST_CHECK_EQUAL(count, 4);

  const auto& const_table = table;
  size_t const_count = 0;
  for (auto it = const_table.cbegin(); it != const_table.cend(); ++it)
  {
    ++const_count;
  }
  BOOST_CHECK_EQUAL(const_count, 4);
}

BOOST_AUTO_TEST_CASE(test_copy)
{
  CuckooHashTable<int, int> original(4);
  original.add(1, 10);
  original.add(2, 20);
  original.add(3, 30);

  CuckooHashTable<int, int> copy(original);
  BOOST_CHECK_EQUAL(copy.size(), 3);
  BOOST_CHECK_EQUAL(copy.get(2), 20);

  original.add(4, 40);
  BOOST_CHECK_EQUAL(original.size(), 4);
  BOOST_CHECK_EQUAL(copy.size(), 3);
  BOOST_CHECK(!copy.has(4));
}

BOOST_AUTO_TEST_CASE(test_assign)
{
  CuckooHashTable<int, int> original(4);
  original.add(1, 10);
  original.add(2, 20);

  CuckooHashTable<int, int> assigned;
  assigned = original;
  BOOST_CHECK_EQUAL(assigned.size(), 2);
  BOOST_CHECK_EQUAL(assigned.get(1), 10);
  BOOST_CHECK_EQUAL(assigned.get(2), 20);
}

BOOST_AUTO_TEST_CASE(test_move)
{
  CuckooHashTable<std::string, std::string> source(2);
  source.add("key1", "val1");
  source.add("key2", "val2");

  CuckooHashTable<std::string, std::string> dest(std::move(source));
  BOOST_CHECK_EQUAL(dest.size(), 2);
  BOOST_CHECK(dest.has("key1"));
  BOOST_CHECK_EQUAL(dest.get("key2"), "val2");
  BOOST_CHECK(source.empty());

  CuckooHashTable<std::string, std::string> moved_assigned;
  moved_assigned = std::move(dest);
  BOOST_CHECK_EQUAL(moved_assigned.size(), 2);
  BOOST_CHECK(moved_assigned.has("key1"));
  BOOST_CHECK(dest.empty());
}

BOOST_AUTO_TEST_CASE(test_clear)
{
  CuckooHashTable<int, double> table(4);
  for (int i = 0; i < 10; ++i)
  {
    table.add(i, i * 0.5);
  }
  BOOST_CHECK_EQUAL(table.size(), 10);
  table.clear();
  BOOST_CHECK(table.empty());
  BOOST_CHECK_EQUAL(table.size(), 0);
  BOOST_CHECK(!table.has(5));
  BOOST_CHECK_THROW(table.get(5), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(test_custom_key)
{
  CuckooHashTable<std::pair<int,int>, std::string,
    PairHasher<int>, PairHasher<int>> table(8);
  auto p1 = std::make_pair(1, 2);
  auto p2 = std::make_pair(3, 4);
  table.add(p1, "first");
  table.add(p2, "second");

  BOOST_CHECK(table.has(p1));
  BOOST_CHECK_EQUAL(table.get(p2), "second");
}

BOOST_AUTO_TEST_CASE(test_move_add)
{
  CuckooHashTable<int, std::string> table(2);
  std::string s1 = "hello";
  std::string s2 = "world";
  table.add(1, std::move(s1));
  table.add(2, std::move(s2));

  BOOST_CHECK_EQUAL(table.get(1), "hello");
  BOOST_CHECK_EQUAL(table.get(2), "world");
  BOOST_CHECK_EQUAL(table.size(), 2);
}

BOOST_AUTO_TEST_SUITE_END()
