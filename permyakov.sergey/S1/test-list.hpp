#include <boost/test/unit_test.hpp>
#include "iterators.hpp"
#include "list.hpp"

using namespace permyakov;

BOOST_AUTO_TEST_SUITE(ListMethodsTests)

BOOST_AUTO_TEST_CASE(test_erase)
{
  List < int > list;
  list.push_front(2);
  list.push_front(1);
  list.push_front(0);
  LIter < int > bItList = list.begin();
  LIter < int > eItList = list.end();
  bItList++;
  list.erase(bItList);
  BOOST_CHECK_EQUAL(list.size(), 2);
  BOOST_CHECK_EQUAL(list.front(), 0);
  BOOST_CHECK(bItList == eItList);
}

BOOST_AUTO_TEST_CASE(test_clear)
{
  List < int > list;
  LIter < int > itList = list.begin();
  list.push_front(0);
  list.push_front(0);
  list.clear();
  BOOST_CHECK_EQUAL(list.size(), 0);
  BOOST_CHECK(itList == list.front());
}

BOOST_AUTO_TEST_CASE(test_begin)
{
  List < int > list;
  list.push_front(2);
  list.push_front(1);
  LIter < int > itList = list.begin();
  BOOST_CHECK_EQUAL(*itList, 1);
}

BOOST_AUTO_TEST_CASE(test_end)
{
  List < int > list;
  list.push_front(2);
  list.push_front(1);
  LIter < int > itList = list.end();
  BOOST_CHECK_EQUAL(*itList, 2);
}

BOOST_AUTO_TEST_CASE(test_size)
{
  List < int > list;
  BOOST_CHECK_EQUAL(list.size(), 0);
  list.push_front(0);
  BOOST_CHECK_EQUAL(list.size(), 1);
  list.push_front(0);
  BOOST_CHECK_EQUAL(list.size(), 2);
}

BOOST_AUTO_TEST_CASE(test_push_front)
{
  List < int > list;
  list.push_front(10);
  BOOST_CHECK_EQUAL(list.size(), 1);
  BOOST_CHECK_EQUAL(list.front(), 10);
}

BOOST_AUTO_TEST_CASE(test_pop_front)
{
  List < int > list;
  list.push_front(2);
  list.push_front(1);
  list.pop_front();
  BOOST_CHECK_EQUAL(list.size(), 1);
  BOOST_CHECK_EQUAL(list.front(), 2);
}

BOOST_AUTO_TEST_SUITE_END()