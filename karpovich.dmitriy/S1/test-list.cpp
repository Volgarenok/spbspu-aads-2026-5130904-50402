#include <boost/test/unit_test.hpp>
#include "list.hpp"
using namespace karpovich;

BOOST_AUTO_TEST_SUITE(ListTests)

BOOST_AUTO_TEST_CASE(default_constructor)
{
  List< int > list;
  BOOST_CHECK_EQUAL(list.size(), 0);
}

BOOST_AUTO_TEST_CASE(copy_constructor)
{
  List< int > list1;
  List< int > list2(list1);
  BOOST_CHECK_EQUAL(list2.size(), 0);
}

BOOST_AUTO_TEST_CASE(move_constructor)
{
  List< int > list1;
  list1.push_back(1);
  list1.push_back(2);

  List< int > list2(std::move(list1));
  BOOST_CHECK_EQUAL(list2.size(), 2);
  BOOST_CHECK_EQUAL(list1.size(), 0);
}

BOOST_AUTO_TEST_CASE(copy_assignment)
{
  List< int > list1;
  List< int > list2;
  list2.push_back(5);

  list2 = list1;
  BOOST_CHECK_EQUAL(list2.size(), 0);
}

BOOST_AUTO_TEST_CASE(move_assignment)
{
  List< int > list1;
  list1.push_back(1);
  list1.push_back(2);

  List< int > list2;
  list2.push_back(10);
  list2 = std::move(list1);

  BOOST_CHECK_EQUAL(list2.size(), 2);
  BOOST_CHECK_EQUAL(list1.size(), 0);
}

BOOST_AUTO_TEST_CASE(push_front)
{
  List< int > list;
  list.push_front(1);
  BOOST_CHECK_EQUAL(list.size(), 1);

  list.push_front(2);
  BOOST_CHECK_EQUAL(list.size(), 2);
}

BOOST_AUTO_TEST_CASE(pop_front)
{
  List< int > list;
  list.push_back(1);
  list.push_back(2);
  list.push_back(3);

  list.pop_front();
  BOOST_CHECK_EQUAL(list.size(), 2);
}

BOOST_AUTO_TEST_CASE(pop_back)
{
  List< int > list;
  list.push_back(1);
  list.push_back(2);
  list.push_back(3);

  list.pop_back();
  BOOST_CHECK_EQUAL(list.size(), 2);
}

BOOST_AUTO_TEST_CASE(clear)
{
  List< int > list;
  list.push_back(1);
  list.push_back(2);
  list.push_back(3);

  list.clear();
  BOOST_CHECK_EQUAL(list.size(), 0);
  list.push_back(10);
  BOOST_CHECK_EQUAL(list.size(), 1);
}

BOOST_AUTO_TEST_SUITE_END()
