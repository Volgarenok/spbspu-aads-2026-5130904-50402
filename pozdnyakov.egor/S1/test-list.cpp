#include <cstddef>
#include <boost/test/unit_test.hpp>
#include "list.hpp"

using namespace pozdnyakov;

BOOST_AUTO_TEST_SUITE(ListTests)

BOOST_AUTO_TEST_CASE(testListPushFrontAndIterators)
{
  List< int > list;
  BOOST_CHECK(list.empty());

  list.pushFront(30);
  list.pushFront(20);
  list.pushFront(10);

  BOOST_CHECK(!list.empty());
  BOOST_CHECK_EQUAL(list.front(), 10);

  const int expected[] = {10, 20, 30};
  size_t i = 0;
  for (auto it = list.begin(); it != list.end(); ++it) {
    BOOST_CHECK_EQUAL(*it, expected[i]);
    ++i;
  }
  BOOST_CHECK_EQUAL(i, 3u);
}

BOOST_AUTO_TEST_CASE(testListCopyExceptionSafety)
{
  List< int > list1;
  list1.pushFront(2);
  list1.pushFront(1);

  List< int > list2 = list1;
  BOOST_CHECK(!list2.empty());
  BOOST_CHECK_EQUAL(list2.front(), 1);

  list1.pushFront(0);

  auto it = list2.begin();
  BOOST_CHECK_EQUAL(*it, 1);
  ++it;
  BOOST_CHECK_EQUAL(*it, 2);
}

BOOST_AUTO_TEST_CASE(testListSplice)
{
  List< int > list1;
  list1.pushFront(2);
  list1.pushFront(1);

  List< int > list2;
  list2.pushFront(4);
  list2.pushFront(3);

  list1.splice(list1.end(), list2);

  BOOST_CHECK(list2.empty());
  const int expected[] = {1, 2, 3, 4};
  size_t i = 0;
  for (auto it = list1.begin(); it != list1.end(); ++it) {
    BOOST_CHECK_EQUAL(*it, expected[i++]);
  }
}

BOOST_AUTO_TEST_CASE(testListSpliceSingle)
{
  List< int > list1;
  list1.pushFront(2);
  list1.pushFront(1);

  List< int > list2;
  list2.pushFront(4);
  list2.pushFront(3);

  list1.splice(list1.begin(), list2, list2.begin());

  const int expected1[] = {3, 1, 2};
  size_t i = 0;
  for (auto it = list1.begin(); it != list1.end(); ++it) {
    BOOST_CHECK_EQUAL(*it, expected1[i++]);
  }

  const int expected2[] = {4};
  size_t j = 0;
  for (auto it = list2.begin(); it != list2.end(); ++it) {
    BOOST_CHECK_EQUAL(*it, expected2[j++]);
  }
}

BOOST_AUTO_TEST_CASE(testListMerge)
{
  List< int > list1;
  list1.pushFront(5);
  list1.pushFront(3);
  list1.pushFront(1);

  List< int > list2;
  list2.pushFront(6);
  list2.pushFront(4);
  list2.pushFront(2);

  list1.merge(list2);

  BOOST_CHECK(list2.empty());
  const int expected[] = {1, 2, 3, 4, 5, 6};
  size_t i = 0;
  for (auto it = list1.begin(); it != list1.end(); ++it) {
    BOOST_CHECK_EQUAL(*it, expected[i++]);
  }
}

BOOST_AUTO_TEST_CASE(testListSort)
{
  List< int > list;
  list.pushFront(4);
  list.pushFront(2);
  list.pushFront(5);
  list.pushFront(1);
  list.pushFront(3);

  list.sort();

  const int expected[] = {1, 2, 3, 4, 5};
  size_t i = 0;
  for (auto it = list.begin(); it != list.end(); ++it) {
    BOOST_CHECK_EQUAL(*it, expected[i++]);
  }
}

BOOST_AUTO_TEST_CASE(testListPartition)
{
  List< int > list;
  list.pushFront(1);
  list.pushFront(2);
  list.pushFront(3);
  list.pushFront(4);
  list.pushFront(5);

  list.partition([](const int &val)
  {
    return val % 2 == 0;
  });

  const int expected[] = {4, 2, 5, 3, 1};
  size_t i = 0;
  for (auto it = list.begin(); it != list.end(); ++it) {
    BOOST_CHECK_EQUAL(*it, expected[i++]);
  }
  BOOST_CHECK_EQUAL(i, 5u);
}

BOOST_AUTO_TEST_CASE(testListPushBack)
{
  List< int > list;
  list.pushBack(10);
  list.pushBack(20);
  list.pushBack(30);

  BOOST_CHECK_EQUAL(list.front(), 10);
  BOOST_CHECK_EQUAL(list.back(), 30);

  const int expected[] = {10, 20, 30};
  size_t i = 0;
  for (auto it = list.begin(); it != list.end(); ++it) {
    BOOST_CHECK_EQUAL(*it, expected[i++]);
  }
  BOOST_CHECK_EQUAL(i, 3u);
}

BOOST_AUTO_TEST_CASE(testListBackAfterPushFront)
{
  List< int > list;
  list.pushFront(2);
  list.pushFront(1);
  BOOST_CHECK_EQUAL(list.back(), 2);
}

BOOST_AUTO_TEST_CASE(testListPopBack)
{
  List< int > list;
  list.pushBack(1);
  list.pushBack(2);
  list.pushBack(3);
  list.popBack();

  BOOST_CHECK_EQUAL(list.back(), 2);
  const int expected[] = {1, 2};
  size_t i = 0;
  for (auto it = list.begin(); it != list.end(); ++it) {
    BOOST_CHECK_EQUAL(*it, expected[i++]);
  }
  BOOST_CHECK_EQUAL(i, 2u);
}

BOOST_AUTO_TEST_CASE(testListPopBackSingleElement)
{
  List< int > list;
  list.pushBack(42);
  list.popBack();
  BOOST_CHECK(list.empty());
}

BOOST_AUTO_TEST_CASE(testListInsertAfterMiddle)
{
  List< int > list;
  list.pushBack(1);
  list.pushBack(3);

  auto pos = list.cbegin();
  list.insertAfter(pos, 2);

  const int expected[] = {1, 2, 3};
  size_t i = 0;
  for (auto it = list.begin(); it != list.end(); ++it) {
    BOOST_CHECK_EQUAL(*it, expected[i++]);
  }
  BOOST_CHECK_EQUAL(i, 3u);
  BOOST_CHECK_EQUAL(list.back(), 3);
}

BOOST_AUTO_TEST_CASE(testListInsertAfterUpdatesBack)
{
  List< int > list;
  list.pushBack(1);
  list.pushBack(2);

  auto pos = list.cbegin();
  ++pos;
  list.insertAfter(pos, 3);

  BOOST_CHECK_EQUAL(list.back(), 3);
  const int expected[] = {1, 2, 3};
  size_t i = 0;
  for (auto it = list.begin(); it != list.end(); ++it) {
    BOOST_CHECK_EQUAL(*it, expected[i++]);
  }
  BOOST_CHECK_EQUAL(i, 3u);
}

BOOST_AUTO_TEST_CASE(testListMoveConstructor)
{
  List< int > original;
  original.pushBack(1);
  original.pushBack(2);
  original.pushBack(3);

  List< int > moved(std::move(original));

  BOOST_CHECK(original.empty());
  BOOST_CHECK_EQUAL(moved.front(), 1);
  BOOST_CHECK_EQUAL(moved.back(), 3);

  const int expected[] = {1, 2, 3};
  size_t i = 0;
  for (auto it = moved.begin(); it != moved.end(); ++it) {
    BOOST_CHECK_EQUAL(*it, expected[i++]);
  }
  BOOST_CHECK_EQUAL(i, 3u);
}

BOOST_AUTO_TEST_CASE(testListMoveAssignment)
{
  List< int > a;
  a.pushBack(10);
  a.pushBack(20);

  List< int > b;
  b.pushBack(99);
  b = std::move(a);

  BOOST_CHECK(a.empty());
  BOOST_CHECK_EQUAL(b.front(), 10);
  BOOST_CHECK_EQUAL(b.back(), 20);
}

BOOST_AUTO_TEST_CASE(testListSelfAssignment)
{
  List< int > list;
  list.pushBack(1);
  list.pushBack(2);

  list = list;

  BOOST_CHECK_EQUAL(list.front(), 1);
  BOOST_CHECK_EQUAL(list.back(), 2);
}

BOOST_AUTO_TEST_CASE(testListSortDescending)
{
  List< int > list;
  list.pushBack(3);
  list.pushBack(1);
  list.pushBack(4);
  list.pushBack(2);

  list.sort([](const int &a, const int &b)
  {
    return a > b;
  });

  const int expected[] = {4, 3, 2, 1};
  size_t i = 0;
  for (auto it = list.begin(); it != list.end(); ++it) {
    BOOST_CHECK_EQUAL(*it, expected[i++]);
  }
  BOOST_CHECK_EQUAL(i, 4u);
}

BOOST_AUTO_TEST_CASE(testListSpliceRange)
{
  List< int > list1;
  list1.pushBack(1);
  list1.pushBack(4);

  List< int > list2;
  list2.pushBack(2);
  list2.pushBack(3);

  auto pos = list1.cbegin();
  ++pos;
  list1.splice(pos, list2, list2.cbegin(), list2.cend());

  BOOST_CHECK(list2.empty());
  const int expected[] = {1, 2, 3, 4};
  size_t i = 0;
  for (auto it = list1.begin(); it != list1.end(); ++it) {
    BOOST_CHECK_EQUAL(*it, expected[i++]);
  }
  BOOST_CHECK_EQUAL(i, 4u);
  BOOST_CHECK_EQUAL(list1.back(), 4);
}

BOOST_AUTO_TEST_SUITE_END()
