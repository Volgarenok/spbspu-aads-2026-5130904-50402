#include <boost/test/unit_test.hpp>
#include "list.hpp"
#include <vector>

BOOST_AUTO_TEST_SUITE(ListTests)

BOOST_AUTO_TEST_CASE(testListInsertAndIterators)
{
  pozdnyakov::List< int > list;
  BOOST_TEST(list.empty());

  list.pushFront(10);
  BOOST_TEST(list.front() == 10);

  list.insertAfter(list.begin(), 20);

  std::vector< int > expected = {10, 20};
  int i = 0;
  for (auto it = list.begin(); it != list.end(); ++it) {
    BOOST_TEST(*it == expected[i]);
    ++i;
  }
}

BOOST_AUTO_TEST_CASE(testListCopyExceptionSafety)
{
  pozdnyakov::List< int > list1;
  list1.pushFront(2);
  list1.pushFront(1);

  pozdnyakov::List< int > list2 = list1;
  BOOST_TEST(!list2.empty());
  BOOST_TEST(list2.front() == 1);

  list2.insertAfter(list2.begin(), 3);

  auto it = list1.begin();
  ++it;
  BOOST_TEST(*it == 2);
}

BOOST_AUTO_TEST_SUITE_END()
