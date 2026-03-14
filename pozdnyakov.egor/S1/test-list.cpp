#include <boost/test/unit_test.hpp>
#include "list.hpp"

using namespace pozdnyakov;

BOOST_AUTO_TEST_SUITE(ListTests)

BOOST_AUTO_TEST_CASE(testListInsertAndIterators)
{
  List< int > list;
  BOOST_CHECK(list.empty());

  list.pushFront(10);
  BOOST_CHECK_EQUAL(list.front(), 10);

  list.insertAfter(list.begin(), 20);

  int expected[] = {10, 20};
  int i = 0;
  for (auto it = list.begin(); it != list.end(); ++it) {
    BOOST_CHECK_EQUAL(*it, expected[i]);
    ++i;
  }
}

BOOST_AUTO_TEST_CASE(testListCopyExceptionSafety)
{
  List< int > list1;
  list1.pushFront(2);
  list1.pushFront(1);

  List< int > list2 = list1;
  BOOST_CHECK(!list2.empty());
  BOOST_CHECK_EQUAL(list2.front(), 1);

  list2.insertAfter(list2.begin(), 3);

  auto it = list1.begin();
  ++it;
  BOOST_CHECK_EQUAL(*it, 2);
}

BOOST_AUTO_TEST_SUITE_END()
