#include <boost/test/unit_test.hpp>
#include "liter.hpp"
#include "lciter.hpp"
#include "list.hpp"

using namespace muhamadiarov;

BOOST_AUTO_TEST_SUITE(ListMethodsTestsAndIter)

BOOST_AUTO_TEST_CASE(test_default_constructor)
{
  List< int > list;
  BOOST_CHECK_EQUAL(list.size(), 0);
  BOOST_CHECK(list.begin() == list.end());
}

BOOST_AUTO_TEST_CASE(test_copy_constructor)
{
  List< int > list1;
  list1.pushFront(1);

  List< int > list2(list1);
  BOOST_CHECK_EQUAL(list2.size(), 1);
  BOOST_CHECK_EQUAL(*(list2.begin()), 1);
}

BOOST_AUTO_TEST_CASE(test_move_constructor)
{
  List< int > list1;
  list1.pushFront(1);
  list1.pushFront(2);

  List< int > list2(std::move(list1));
  BOOST_CHECK_EQUAL(list2.size(), 2);
  BOOST_CHECK_EQUAL(list1.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_copy_assignment)
{
  List< int > list1;
  list1.pushFront(1);

  List< int > list2;
  list2 = list1;

  BOOST_CHECK_EQUAL(list2.size(), 1);
  BOOST_CHECK_EQUAL(*list2.begin(), 1);
}

BOOST_AUTO_TEST_CASE(test_move_assignment)
{
  List< int > list1;
  list1.pushFront(1);

  List< int > list2;
  list2 = std::move(list1);

  BOOST_CHECK_EQUAL(list2.size(), 1);
  BOOST_CHECK_EQUAL(list1.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_destructor)
{
  {
    List< int > list;
    list.pushFront(1);
    list.pushFront(2);
  }
  BOOST_CHECK(true);
}

BOOST_AUTO_TEST_CASE(test_begin)
{
  List< int > list;
  list.pushFront(5);

  BOOST_CHECK_EQUAL(*list.begin(), 5);
}

BOOST_AUTO_TEST_CASE(test_end)
{
  List< int > list;
  list.pushFront(1);

  Iter< int > it = list.begin();
  it++;
  BOOST_CHECK(it == list.end());
}

BOOST_AUTO_TEST_CASE(test_begin_const)
{
  const List< int > list;
  BOOST_CHECK(list.cbegin() == list.cend());
}

BOOST_AUTO_TEST_CASE(test_end_const)
{
  const List< int > list;
  BOOST_CHECK(list.cbegin() == list.cend());
}

BOOST_AUTO_TEST_CASE(test_push_front)
{
  List< int > list;
  list.pushFront(2);
  list.pushFront(1);

  BOOST_CHECK_EQUAL(*list.begin(), 1);
  BOOST_CHECK_EQUAL(list.size(), 2);
}

BOOST_AUTO_TEST_CASE(test_push_back)
{
  List< int > list;
  list.pushBack(1);
  list.pushBack(2);

  Iter< int > it = list.begin();
  BOOST_CHECK_EQUAL(*it, 1);
  it++;
  BOOST_CHECK_EQUAL(*it, 2);
}

BOOST_AUTO_TEST_CASE(test_pop_front)
{
  List< int > list;
  list.pushFront(1);
  list.pushFront(2);

  list.popFront();
  BOOST_CHECK_EQUAL(list.size(), 1);
  BOOST_CHECK_EQUAL(*list.begin(), 2);
}

BOOST_AUTO_TEST_CASE(test_pop_back)
{
  List< int > list;
  list.pushFront(1);
  list.pushFront(2);

  list.popBack();
  BOOST_CHECK_EQUAL(list.size(), 1);
  BOOST_CHECK_EQUAL(*list.begin(), 1);
}

BOOST_AUTO_TEST_CASE(test_clear)
{
  List< int > list;
  list.pushFront(1);
  list.pushFront(2);

  list.clear();
  BOOST_CHECK_EQUAL(list.size(), 0);
  BOOST_CHECK(list.begin() == list.end());
}

BOOST_AUTO_TEST_CASE(test_size)
{
  List< int > list;
  BOOST_CHECK_EQUAL(list.size(), 0);

  list.pushFront(1);
  BOOST_CHECK_EQUAL(list.size(), 1);

  list.pushFront(2);
  BOOST_CHECK_EQUAL(list.size(), 2);
}

BOOST_AUTO_TEST_SUITE_END()
