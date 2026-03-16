#define BOOST_TEST_MODULE ListTests
#include <boost/test/included/unit_test.hpp>
#include "list.hpp"

using namespace karpenkov;

BOOST_AUTO_TEST_SUITE(ListTests);

BOOST_AUTO_TEST_CASE(defaultConstructor)
{
  List< int > list;
  BOOST_CHECK(list.begin() == list.end());
}

BOOST_AUTO_TEST_CASE(push_back)
{
  List< int > list;
  list.push_back(1);
  list.push_back(2);
  auto it = list.begin();
  BOOST_CHECK_EQUAL(*it, 1);
  ++it;
  BOOST_CHECK_EQUAL(*it, 2);
}

BOOST_AUTO_TEST_CASE(push_front)
{
  List< int > list;
  list.push_front(1);
  list.push_front(2);
  auto it = list.begin();
  BOOST_CHECK_EQUAL(*it, 2);
  ++it;
  BOOST_CHECK_EQUAL(*it, 1);
}

BOOST_AUTO_TEST_CASE(pop_back)
{
  List< int > list;
  list.push_back(1);
  list.push_back(2);
  list.push_back(3);
  list.pop_back();
  auto it = list.begin();
  BOOST_CHECK_EQUAL(*it, 1);
  ++it;
  BOOST_CHECK_EQUAL(*it, 2);
  ++it;
  BOOST_CHECK(it == list.end());
}

BOOST_AUTO_TEST_CASE(pop_front)
{
  List< int > list;
  list.push_back(1);
  list.push_back(2);
  list.push_back(3);
  list.pop_front();
  auto it = list.begin();
  BOOST_CHECK_EQUAL(*it, 2);
}

BOOST_AUTO_TEST_CASE(clear)
{
  List< int > list;
  list.push_back(1);
  list.push_back(2);
  list.clear();
  BOOST_CHECK(list.begin() == list.end());
}

BOOST_AUTO_TEST_CASE(copy_constructor)
{
  List< int > list;
  list.push_back(5);
  list.push_back(6);
  List< int > copy(list);
  auto it = copy.begin();
  BOOST_CHECK_EQUAL(*it, 5);
  ++it;
  BOOST_CHECK_EQUAL(*it, 6);
}

BOOST_AUTO_TEST_CASE(move_constructor)
{
  List< int > list;
  list.push_back(7);
  list.push_back(8);
  List< int > moved(std::move(list));
  auto it = moved.begin();
  BOOST_CHECK_EQUAL(*it, 7);
  ++it;
  BOOST_CHECK_EQUAL(*it, 8);
}

BOOST_AUTO_TEST_CASE(const_iterator)
{
  List< int > list;
  list.push_back(9);
  list.push_back(10);
  const List< int > &clist = list;
  auto it = clist.cbegin();
  BOOST_CHECK_EQUAL(*it, 9);
  ++it;
  BOOST_CHECK_EQUAL(*it, 10);
}

BOOST_AUTO_TEST_SUITE_END();
