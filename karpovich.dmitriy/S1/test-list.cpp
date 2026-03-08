#include <boost/test/unit_test.hpp>
#include "iterators.hpp"
#include "list.hpp"

using namespace karpovich;

BOOST_AUTO_TEST_SUITE(ListMethodsTests)

BOOST_AUTO_TEST_CASE(test_default_constructor)
{
  List< int > list;
  BOOST_CHECK_EQUAL(list.size(), 0);
  BOOST_CHECK(list.begin() == list.end());
}

BOOST_AUTO_TEST_CASE(test_copy_constructor)
{
  List< int > list1;
  list1.push_back(1);
  list1.push_back(2);

  List< int > list2(list1);
  BOOST_CHECK_EQUAL(list2.size(), 2);
  BOOST_CHECK_EQUAL(*list2.begin(), 1);
}

BOOST_AUTO_TEST_CASE(test_move_constructor)
{
  List< int > list1;
  list1.push_back(1);
  list1.push_back(2);

  List< int > list2(std::move(list1));
  BOOST_CHECK_EQUAL(list2.size(), 2);
  BOOST_CHECK_EQUAL(list1.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_copy_assignment)
{
  List< int > list1;
  list1.push_back(1);

  List< int > list2;
  list2 = list1;

  BOOST_CHECK_EQUAL(list2.size(), 1);
  BOOST_CHECK_EQUAL(*list2.begin(), 1);
}

BOOST_AUTO_TEST_CASE(test_move_assignment)
{
  List< int > list1;
  list1.push_back(1);

  List< int > list2;
  list2 = std::move(list1);

  BOOST_CHECK_EQUAL(list2.size(), 1);
  BOOST_CHECK_EQUAL(list1.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_destructor)
{
  {
    List< int > list;
    list.push_back(1);
    list.push_back(2);
  }
  BOOST_CHECK(true);
}

BOOST_AUTO_TEST_CASE(test_begin)
{
  List< int > list;
  list.push_back(5);

  BOOST_CHECK_EQUAL(*list.begin(), 5);
}

BOOST_AUTO_TEST_CASE(test_end)
{
  List< int > list;
  list.push_back(1);

  LIter< int > it = list.begin();
  ++it;
  BOOST_CHECK(it == list.end());
}

BOOST_AUTO_TEST_CASE(test_begin_const)
{
  const List< int > list;
  BOOST_CHECK(list.begin() == list.end());
}

BOOST_AUTO_TEST_CASE(test_end_const)
{
  const List< int > list;
  BOOST_CHECK(list.begin() == list.end());
}

BOOST_AUTO_TEST_CASE(test_push_front)
{
  List< int > list;
  list.push_front(2);
  list.push_front(1);

  BOOST_CHECK_EQUAL(*list.begin(), 1);
  BOOST_CHECK_EQUAL(list.size(), 2);
}

BOOST_AUTO_TEST_CASE(test_push_back)
{
  List< int > list;
  list.push_back(1);
  list.push_back(2);

  LIter< int > it = list.begin();
  BOOST_CHECK_EQUAL(*it, 1);
  it++;
  BOOST_CHECK_EQUAL(*it, 2);
}

BOOST_AUTO_TEST_CASE(test_pop_front)
{
  List< int > list;
  list.push_back(1);
  list.push_back(2);

  list.pop_front();
  BOOST_CHECK_EQUAL(list.size(), 1);
  BOOST_CHECK_EQUAL(*list.begin(), 2);
}

BOOST_AUTO_TEST_CASE(test_pop_back)
{
  List< int > list;
  list.push_back(1);
  list.push_back(2);

  list.pop_back();
  BOOST_CHECK_EQUAL(list.size(), 1);
  BOOST_CHECK_EQUAL(*list.begin(), 1);
}

BOOST_AUTO_TEST_CASE(test_clear)
{
  List< int > list;
  list.push_back(1);
  list.push_back(2);

  list.clear();
  BOOST_CHECK_EQUAL(list.size(), 0);
  BOOST_CHECK(list.begin() == list.end());
}

BOOST_AUTO_TEST_CASE(test_size)
{
  List< int > list;
  BOOST_CHECK_EQUAL(list.size(), 0);

  list.push_back(1);
  BOOST_CHECK_EQUAL(list.size(), 1);

  list.push_back(2);
  BOOST_CHECK_EQUAL(list.size(), 2);
}

BOOST_AUTO_TEST_CASE(test_empty)
{
  List< int > list;
  BOOST_CHECK_EQUAL(list.empty(), true);
}

BOOST_AUTO_TEST_CASE(test_insert)
{
  List< int > list;
  list.push_back(1);
  list.push_back(2);
  list.push_back(3);
  LIter< int > it = list.begin();
  it++;
  it = list.insert(it, 4);
  BOOST_CHECK_EQUAL(*it, 4);
}

BOOST_AUTO_TEST_SUITE_END()
