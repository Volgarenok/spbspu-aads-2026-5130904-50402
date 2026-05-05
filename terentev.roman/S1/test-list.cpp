#define BOOST_TEST_MODULE list_tests
#include <boost/test/included/unit_test.hpp>
#include "list.hpp"
#include <string>

BOOST_AUTO_TEST_CASE(default_constructor_test)
{
  terentev::List< int > list;
  BOOST_TEST(list.isEmpty());
  BOOST_TEST(list.getSize() == 0);
  BOOST_CHECK(list.begin() == list.end());
}

BOOST_AUTO_TEST_CASE(push_front_test)
{
  terentev::List< int > list;
  list.pushFront(1);
  BOOST_TEST(!list.isEmpty());
  BOOST_TEST(list.getSize() == 1);
  BOOST_TEST(list.front() == 1);

  list.pushFront(2);
  BOOST_TEST(list.getSize() == 2);
  BOOST_TEST(list.front() == 2);
}

BOOST_AUTO_TEST_CASE(pop_front_test)
{
  terentev::List< int > list;
  list.pushFront(1);
  list.pushFront(2);
  list.pushFront(3);

  list.popFront();
  BOOST_TEST(list.getSize() == 2);
  BOOST_TEST(list.front() == 2);

  list.popFront();
  BOOST_TEST(list.getSize() == 1);
  BOOST_TEST(list.front() == 1);

  list.popFront();
  BOOST_TEST(list.isEmpty());
  BOOST_TEST(list.getSize() == 0);
}

BOOST_AUTO_TEST_CASE(pop_front_empty_test)
{
  terentev::List< int > list;
  list.popFront();
  BOOST_TEST(list.isEmpty());
  BOOST_TEST(list.getSize() == 0);
}

BOOST_AUTO_TEST_CASE(insert_after_test)
{
  terentev::List< int > list;
  list.pushFront(1);
  terentev::List< int >::LIter iter = list.begin();

  list.insertAfter(iter, 2);
  BOOST_TEST(list.getSize() == 2);

  terentev::List< int >::LIter check = list.begin();
  BOOST_TEST(*check == 1);
  ++check;
  BOOST_TEST(*check == 2);
  ++check;
  BOOST_CHECK(check == list.end());
}

BOOST_AUTO_TEST_CASE(insert_after_middle_test)
{
  terentev::List< int > list;
  list.pushFront(3);
  list.pushFront(2);
  list.pushFront(1);

  terentev::List< int >::LIter iter = list.begin();
  ++iter;
  list.insertAfter(iter, 10);

  BOOST_TEST(list.getSize() == 4);

  terentev::List< int >::LIter check = list.begin();
  BOOST_TEST(*check == 1);
  ++check;
  BOOST_TEST(*check == 2);
  ++check;
  BOOST_TEST(*check == 10);
  ++check;
  BOOST_TEST(*check == 3);
}

BOOST_AUTO_TEST_CASE(insert_after_null_iterator_test)
{
  terentev::List< int > list;
  terentev::List< int >::LIter iter;
  list.insertAfter(iter, 5);
  BOOST_TEST(list.isEmpty());
  BOOST_TEST(list.getSize() == 0);
}

BOOST_AUTO_TEST_CASE(clear_test)
{
  terentev::List< int > list;
  list.pushFront(1);
  list.pushFront(2);
  list.pushFront(3);

  list.clear();
  BOOST_TEST(list.isEmpty());
  BOOST_TEST(list.getSize() == 0);
  BOOST_CHECK(list.begin() == list.end());
}

BOOST_AUTO_TEST_CASE(copy_constructor_test)
{
  terentev::List< int > list;
  list.pushFront(3);
  list.pushFront(2);
  list.pushFront(1);

  terentev::List< int > copy(list);

  BOOST_TEST(copy.getSize() == 3);
  BOOST_TEST(copy.front() == 1);

  terentev::List< int >::LIter originalIter = list.begin();
  terentev::List< int >::LIter copyIter = copy.begin();

  while (originalIter != list.end() && copyIter != copy.end()) {
    BOOST_TEST(*originalIter == *copyIter);
    ++originalIter;
    ++copyIter;
  }

  BOOST_CHECK(originalIter == list.end());
  BOOST_CHECK(copyIter == copy.end());
}

BOOST_AUTO_TEST_CASE(copy_assignment_test)
{
  terentev::List< int > list;
  list.pushFront(3);
  list.pushFront(2);
  list.pushFront(1);

  terentev::List< int > copy;
  copy.pushFront(100);
  copy = list;

  BOOST_TEST(copy.getSize() == 3);
  BOOST_TEST(copy.front() == 1);

  terentev::List< int >::LIter originalIter = list.begin();
  terentev::List< int >::LIter copyIter = copy.begin();

  while (originalIter != list.end() && copyIter != copy.end()) {
    BOOST_TEST(*originalIter == *copyIter);
    ++originalIter;
    ++copyIter;
  }

  BOOST_CHECK(originalIter == list.end());
  BOOST_CHECK(copyIter == copy.end());
}

BOOST_AUTO_TEST_CASE(move_constructor_test)
{
  terentev::List< int > list;
  list.pushFront(2);
  list.pushFront(1);

  terentev::List< int > moved(static_cast< terentev::List< int > && >(list));

  BOOST_TEST(moved.getSize() == 2);
  BOOST_TEST(moved.front() == 1);
  BOOST_TEST(list.isEmpty());
  BOOST_TEST(list.getSize() == 0);
}

BOOST_AUTO_TEST_CASE(move_assignment_test)
{
  terentev::List< int > list;
  list.pushFront(2);
  list.pushFront(1);

  terentev::List< int > moved;
  moved.pushFront(100);
  moved = static_cast< terentev::List< int > && >(list);

  BOOST_TEST(moved.getSize() == 2);
  BOOST_TEST(moved.front() == 1);
  BOOST_TEST(list.isEmpty());
  BOOST_TEST(list.getSize() == 0);
}

BOOST_AUTO_TEST_CASE(swap_test)
{
  terentev::List< int > first;
  first.pushFront(2);
  first.pushFront(1);

  terentev::List< int > second;
  second.pushFront(4);
  second.pushFront(3);

  first.swap(second);

  BOOST_TEST(first.getSize() == 2);
  BOOST_TEST(second.getSize() == 2);
  BOOST_TEST(first.front() == 3);
  BOOST_TEST(second.front() == 1);
}

BOOST_AUTO_TEST_CASE(iteration_test)
{
  terentev::List< int > list;
  list.pushFront(3);
  list.pushFront(2);
  list.pushFront(1);

  int expected = 1;
  for (terentev::List< int >::LIter iter = list.begin(); iter != list.end(); ++iter) {
    BOOST_TEST(*iter == expected);
    ++expected;
  }
}

BOOST_AUTO_TEST_CASE(const_iteration_test)
{
  terentev::List< int > source;
  source.pushFront(3);
  source.pushFront(2);
  source.pushFront(1);

  const terentev::List< int > list(source);

  int expected = 1;
  for (terentev::List< int >::LCIter iter = list.begin(); iter != list.end(); ++iter) {
    BOOST_TEST(*iter == expected);
    ++expected;
  }
}

BOOST_AUTO_TEST_CASE(arrow_operator_test)
{
  terentev::List< std::string > list;
  list.pushFront("abc");

  terentev::List< std::string >::LIter iter = list.begin();
  BOOST_TEST(iter->size() == 3);

  terentev::List< std::string >::LCIter constIter = list.begin();
  BOOST_TEST(constIter->size() == 3);
}
