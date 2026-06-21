#include <boost/test/unit_test.hpp>

#include "CuckooHashTable.hpp"
#include <stdexcept>

using namespace lachugin;

using Table = CuckooHashTable< std::string, int, DefaultHash1< std::string >, DefaultHash2< std::string >,
  std::equal_to<std::string> >;

BOOST_AUTO_TEST_CASE(constructor_empty_test)
{
  Table table;

  BOOST_TEST(table.empty());
  BOOST_TEST(table.size() == 0);
}

BOOST_AUTO_TEST_CASE(add_get_test)
{
  Table table;

  table.add("one", 1);
  table.add("two", 2);

  BOOST_TEST(table.get("one") == 1);
  BOOST_TEST(table.get("two") == 2);
  BOOST_TEST(table.size() == 2);
}

BOOST_AUTO_TEST_CASE(duplicate_key_test)
{
  Table table;

  table.add("one", 1);
  BOOST_CHECK_THROW(table.add("one", 100), std::logic_error);
}

BOOST_AUTO_TEST_CASE(has_test)
{
  Table table;
  BOOST_TEST(!table.has("one"));
  table.add("one", 1);
  BOOST_TEST(table.has("one"));
}

BOOST_AUTO_TEST_CASE(get_throw_test)
{
  Table table;

  BOOST_CHECK_THROW(table.get("unknown"), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(drop_test)
{
  Table table;
  table.add("one", 1);
  int value = table.drop("one");
  BOOST_TEST(value == 1);
  BOOST_TEST(table.size() == 0);
  BOOST_TEST(!table.has("one"));
}

BOOST_AUTO_TEST_CASE(drop_throw_test)
{
  Table table;

  BOOST_CHECK_THROW(table.drop("unknown"), std::out_of_range);
}


BOOST_AUTO_TEST_CASE(clear_test)
{
  Table table;

  table.add("one", 1);
  table.add("two", 2);

  table.clear();

  BOOST_TEST(table.empty());
  BOOST_TEST(table.size() == 0);
}

BOOST_AUTO_TEST_CASE(copy_constructor_test)
{
  Table table;
  table.add("one", 1);
  table.add("two", 2);
  Table copy(table);
  BOOST_TEST(copy.size() == 2);
  BOOST_TEST(copy.get("one") == 1);
  BOOST_TEST(copy.get("two") == 2);
}

BOOST_AUTO_TEST_CASE(copy_assignment_test)
{
  Table table;
  table.add("one", 1);
  table.add("two", 2);
  Table copy;
  copy = table;
  BOOST_TEST(copy.size() == 2);
  BOOST_TEST(copy.get("one") == 1);
  BOOST_TEST(copy.get("two") == 2);
}

BOOST_AUTO_TEST_CASE(move_constructor_test)
{
  Table table;
  table.add("one", 1);
  table.add("two", 2);
  Table moved(std::move(table));
  BOOST_TEST(moved.size() == 2);
  BOOST_TEST(moved.get("one") == 1);
  BOOST_TEST(moved.get("two") == 2);
}

BOOST_AUTO_TEST_CASE(move_assignment_test)
{
  Table source;
  source.add("one", 1);
  source.add("two", 2);
  Table target;
  target = std::move(source);
  BOOST_TEST(target.size() == 2);
  BOOST_TEST(target.get("one") == 1);
  BOOST_TEST(target.get("two") == 2);
}

BOOST_AUTO_TEST_CASE(rehash_test)
{
  Table table;
  for (int i = 0; i < 100; ++i)
  {
    table.add(std::to_string(i), i);
  }

  for (int i = 0; i < 100; ++i)
  {
    BOOST_TEST(table.get(std::to_string(i)) == i);
  }
  BOOST_TEST(table.size() == 100);
}

BOOST_AUTO_TEST_CASE(iterator_count_test)
{
  Table table;
  table.add("a", 1);
  table.add("b", 2);
  table.add("c", 3);

  size_t count = 0;

  for (auto it = table.begin(); it != table.end(); ++it)
  {
    ++count;
  }

  BOOST_TEST(count == 3);
}
