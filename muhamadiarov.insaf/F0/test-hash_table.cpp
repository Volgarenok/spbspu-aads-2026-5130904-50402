#include <boost/test/unit_test.hpp>
#include "hashtable.hpp"

using namespace muhamadiarov;

struct TestHashTable
{
  RobinTable< std::string, int > emptyTable;
  RobinTable< std::string, int > table;

  TestHashTable()
  {
    table.add("one", 1);
    table.add("two", 2);
    table.add("three", 3);
    table.add("four", 4);
    table.add("five", 5);
  }
};

BOOST_FIXTURE_TEST_SUITE(HashTableTtest, TestHashTable)

BOOST_AUTO_TEST_CASE(DefaultConstructor)
{
  BOOST_CHECK_EQUAL(emptyTable.size(), 0);
  BOOST_CHECK(emptyTable.empty());
}

BOOST_AUTO_TEST_CASE(ConstructorWithSlots)
{
  RobinTable< std::string, int > table(20, 0.8f);
  BOOST_CHECK_EQUAL(table.size(), 0);
  BOOST_CHECK(table.empty());
}

BOOST_AUTO_TEST_CASE(CopyConstructor)
{
  RobinTable< std::string, int > copy(table);
  BOOST_CHECK_EQUAL(copy.size(), table.size());
  BOOST_CHECK_EQUAL(copy.get("one"), 1);
  BOOST_CHECK_EQUAL(copy.get("five"), 5);
}

BOOST_AUTO_TEST_CASE(MoveConstructor)
{
  RobinTable< std::string, int > moved(std::move(table));
  BOOST_CHECK_EQUAL(moved.size(), 5);
  BOOST_CHECK_EQUAL(moved.get("one"), 1);
  BOOST_CHECK_EQUAL(moved.get("five"), 5);

  BOOST_CHECK_EQUAL(table.size(), 0);
  BOOST_CHECK(table.empty());
}

BOOST_AUTO_TEST_CASE(CopyAssignment)
{
  RobinTable< std::string, int > copy;
  copy = table;

  BOOST_CHECK_EQUAL(copy.size(), table.size());
  BOOST_CHECK_EQUAL(copy.get("one"), 1);
  BOOST_CHECK_EQUAL(copy.get("five"), 5);
}

BOOST_AUTO_TEST_CASE(MoveAssignment)
{
  RobinTable< std::string, int > moved;
  moved = std::move(table);

  BOOST_CHECK_EQUAL(moved.size(), 5);
  BOOST_CHECK_EQUAL(moved.get("one"), 1);
  BOOST_CHECK_EQUAL(moved.get("five"), 5);

  BOOST_CHECK_EQUAL(table.size(), 0);
  BOOST_CHECK(table.empty());
}

BOOST_AUTO_TEST_CASE(AddMethod)
{
  table.add("six", 6);
  BOOST_CHECK_EQUAL(table.size(), 6);
  BOOST_CHECK_EQUAL(table.get("six"), 6);
}

BOOST_AUTO_TEST_CASE(DropMethod)
{
  table.drop("one");
  BOOST_CHECK_EQUAL(table.size(), 4);
  BOOST_CHECK(!table.has("one"));
}

BOOST_AUTO_TEST_CASE(GetMethod)
{
  BOOST_CHECK_EQUAL(table.get("one"), 1);
  BOOST_CHECK_EQUAL(table.get("two"), 2);
  BOOST_CHECK_EQUAL(table.get("three"), 3);
}

BOOST_AUTO_TEST_CASE(HasMethod)
{
  BOOST_CHECK(table.has("one"));
  BOOST_CHECK(table.has("two"));
  BOOST_CHECK(table.has("three"));
  BOOST_CHECK(table.has("four"));
  BOOST_CHECK(table.has("five"));
  BOOST_CHECK(!table.has("six"));
}

BOOST_AUTO_TEST_CASE(RehashMethod)
{
  size_t old_size = table.size();
  table.rehash(50);

  BOOST_CHECK_EQUAL(table.size(), old_size);
  BOOST_CHECK(table.has("one"));
  BOOST_CHECK(table.has("five"));
  BOOST_CHECK_EQUAL(table.get("one"), 1);
  BOOST_CHECK_EQUAL(table.get("two"), 2);
}

BOOST_AUTO_TEST_CASE(ClearMethod)
{
  table.clear();
  BOOST_CHECK_EQUAL(table.size(), 0);
  BOOST_CHECK(table.empty());
  BOOST_CHECK(!table.has("one"));
  BOOST_CHECK(!table.has("five"));
}

BOOST_AUTO_TEST_CASE(Iterator)
{
  auto it = table.begin();
  BOOST_CHECK(it != table.end());

  int count = 0;
  for (; it != table.end(); ++it)
  {
    ++count;
    auto pair = *it;
    BOOST_CHECK(table.has(pair.first));
  }
  BOOST_CHECK_EQUAL(count, table.size());
}

BOOST_AUTO_TEST_CASE(ConstIterator)
{
  auto it = table.cbegin();
  BOOST_CHECK(it != table.cend());

  int count = 0;
  for (auto it = table.cbegin(); it != table.cend(); ++it)
  {
    ++count;
    auto pair = *it;
    BOOST_CHECK(table.has(pair.first));
  }
  BOOST_CHECK_EQUAL(count, table.size());
}
BOOST_AUTO_TEST_SUITE_END()
