#include <boost/test/unit_test.hpp>
#include "list.hpp"

using namespace bukreev;

BOOST_AUTO_TEST_SUITE(ListTests)

BOOST_AUTO_TEST_CASE(test_default_constructor)
{
  List< int > list;
  BOOST_CHECK_EQUAL(list.size(), 0);
  BOOST_CHECK(list.begin() == list.end());
  BOOST_CHECK(list.cbegin() == list.cend());
}

BOOST_AUTO_TEST_CASE(test_copy_constructor)
{
  List< int > list1;
  list1.pushBack(1);

  List< int > list2(list1);
  BOOST_CHECK_EQUAL(list2.size(), 1);
  BOOST_CHECK_EQUAL(*list2.begin(), 1);
}

BOOST_AUTO_TEST_CASE(test_copy_operator)
{
  List< int > list1;
  list1.pushBack(1);

  List< int > list2 = list1;

  BOOST_CHECK_EQUAL(list2.size(), 1);
  BOOST_CHECK_EQUAL(*list2.begin(), 1);
}

BOOST_AUTO_TEST_CASE(test_clear)
{
  List< int > list;
  list.pushBack(1);
  list.clear();
  BOOST_CHECK_EQUAL(list.size(), 0);
  BOOST_CHECK(list.begin() == list.end());
  BOOST_CHECK(list.cbegin() == list.cend());
}

BOOST_AUTO_TEST_CASE(test_push_back)
{
  List< int > list;
  list.pushBack(1);
  list.pushBack(2);

  LCIter< int > it = list.cbegin();

  BOOST_CHECK_EQUAL(*it, 1);
  it = it.next();
  BOOST_CHECK_EQUAL(*it, 2);
}

BOOST_AUTO_TEST_SUITE_END()
