#include <boost/test/unit_test.hpp>
#include "list.hpp"

using namespace afanasev;

BOOST_AUTO_TEST_SUITE(ListMethodsTests)

BOOST_AUTO_TEST_CASE(test_default_constructor)
{
  List< int > list;
  BOOST_CHECK_EQUAL(list.size(), 0);
  LIter< int > end(nullptr);
  BOOST_CHECK(list.begin() == end);
}

BOOST_AUTO_TEST_CASE(test_pushFront)
{
  List< int > list;
  list.pushFront(2);
  list.pushFront(1);
  BOOST_CHECK_EQUAL(list.size(), 2);
  auto it = list.begin();
  BOOST_CHECK_EQUAL(*it, 1);
  ++it;
  BOOST_CHECK_EQUAL(*it, 2);
}

BOOST_AUTO_TEST_CASE(test_insert_middle)
{
  List< int > list;
  list.pushFront(3);
  list.pushFront(1);
  auto it = list.begin();
  list.insert(2, it);
  BOOST_CHECK_EQUAL(list.size(), 3);
  auto it2 = list.begin();
  BOOST_CHECK_EQUAL(*it2, 1);
  ++it2;
  BOOST_CHECK_EQUAL(*it2, 2);
  ++it2;
  BOOST_CHECK_EQUAL(*it2, 3);
}

BOOST_AUTO_TEST_CASE(test_deleteNext)
{
  List< int > list;
  list.pushFront(2);
  list.pushFront(1);
  auto first = list.begin();
  list.deleteNext(first);
  BOOST_CHECK_EQUAL(list.size(), 1);
  BOOST_CHECK_EQUAL(*list.begin(), 1);
}

BOOST_AUTO_TEST_CASE(test_deleteNext_middle)
{
  List< int > list;
  list.pushFront(3);
  list.pushFront(2);
  list.pushFront(1);
  auto it = list.begin();
  list.deleteNext(it);
  BOOST_CHECK_EQUAL(list.size(), 2);
  auto it2 = list.begin();
  BOOST_CHECK_EQUAL(*it2, 1);
  ++it2;
  BOOST_CHECK_EQUAL(*it2, 3);
}

BOOST_AUTO_TEST_CASE(test_clear)
{
  List< int > list;
  list.pushFront(1);
  list.pushFront(2);
  list.clear();
  BOOST_CHECK_EQUAL(list.size(), 0);
  LIter< int > end(nullptr);
  BOOST_CHECK(list.begin() == end);
}

BOOST_AUTO_TEST_CASE(test_size)
{
  List< int > list;
  BOOST_CHECK_EQUAL(list.size(), 0);
  list.pushFront(1);
  BOOST_CHECK_EQUAL(list.size(), 1);
  list.pushFront(2);
  BOOST_CHECK_EQUAL(list.size(), 2);
  list.deleteNext(list.begin());
  BOOST_CHECK_EQUAL(list.size(), 1);
}

BOOST_AUTO_TEST_CASE(test_copy_constructor)
{
  List< int > list1;
  list1.pushFront(2);
  list1.pushFront(1);
  List< int > list2(list1);
  BOOST_CHECK_EQUAL(list2.size(), 2);
  auto it = list2.begin();
  BOOST_CHECK_EQUAL(*it, 1);
  ++it;
  BOOST_CHECK_EQUAL(*it, 2);
  list1.deleteNext(list1.begin());
  BOOST_CHECK_EQUAL(list1.size(), 1);
  BOOST_CHECK_EQUAL(list2.size(), 2);
}

BOOST_AUTO_TEST_CASE(test_move_constructor)
{
  List< int > list1;
  list1.pushFront(2);
  list1.pushFront(1);
  List< int > list2(std::move(list1));
  BOOST_CHECK_EQUAL(list2.size(), 2);
  auto it = list2.begin();
  BOOST_CHECK_EQUAL(*it, 1);
  ++it;
  BOOST_CHECK_EQUAL(*it, 2);
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
  BOOST_CHECK_EQUAL(*list2.begin(), 1);
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
  LIter< int > it = list.begin();
  ++it;
  LIter< int > end(nullptr);
  BOOST_CHECK(it == end);
}

BOOST_AUTO_TEST_CASE(test_begin_const)
{
  List< int > list;
  list.pushFront(100);
  const List< int > & constList = list;
  LCIter< int > end(nullptr);
  BOOST_CHECK(constList.begin() != end);
  BOOST_CHECK_EQUAL(*constList.begin(), 100);
}

BOOST_AUTO_TEST_SUITE_END()
