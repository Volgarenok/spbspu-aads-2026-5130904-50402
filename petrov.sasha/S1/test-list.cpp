#include <boost/test/unit_test.hpp>

#include <sstream>
#include <string>
#include <utility>
#include "list.hpp"

BOOST_AUTO_TEST_SUITE(PetrovListTests)

BOOST_AUTO_TEST_CASE(test_default_constructor)
{
  petrov::List< int > list;
  BOOST_CHECK(list.empty());
  BOOST_CHECK_EQUAL(list.size(), 0);
  BOOST_CHECK(list.begin() == list.end());
}

BOOST_AUTO_TEST_CASE(test_push_back)
{
  petrov::List< int > list;
  list.pushBack(10);
  BOOST_CHECK(!list.empty());
  BOOST_CHECK_EQUAL(list.size(), 1);
  BOOST_CHECK_EQUAL(list.front(), 10);
  BOOST_CHECK_EQUAL(list.back(), 10);

  list.pushBack(20);
  BOOST_CHECK_EQUAL(list.size(), 2);
  BOOST_CHECK_EQUAL(list.front(), 10);
  BOOST_CHECK_EQUAL(list.back(), 20);
}

BOOST_AUTO_TEST_CASE(test_push_front)
{
  petrov::List< int > list;
  list.pushFront(10);
  BOOST_CHECK_EQUAL(list.front(), 10);

  list.pushFront(20);
  BOOST_CHECK_EQUAL(list.front(), 20);
  BOOST_CHECK_EQUAL(list.back(), 10);
  BOOST_CHECK_EQUAL(list.size(), 2);
}

BOOST_AUTO_TEST_CASE(test_pop_front)
{
  petrov::List< int > list;
  list.pushBack(1);
  list.pushBack(2);

  list.popFront();
  BOOST_CHECK_EQUAL(list.size(), 1);
  BOOST_CHECK_EQUAL(list.front(), 2);

  list.popFront();
  BOOST_CHECK(list.empty());
  BOOST_CHECK(list.begin() == list.end());
}

BOOST_AUTO_TEST_CASE(test_pop_back)
{
  petrov::List< int > list;
  list.pushBack(1);
  list.pushBack(2);

  list.popBack();
  BOOST_CHECK_EQUAL(list.size(), 1);
  BOOST_CHECK_EQUAL(list.back(), 1);

  list.popBack();
  BOOST_CHECK(list.empty());
}

BOOST_AUTO_TEST_CASE(test_clear)
{
  petrov::List< int > list;
  list.pushBack(1);
  list.pushBack(2);
  list.pushBack(3);

  list.clear();
  BOOST_CHECK(list.empty());
  BOOST_CHECK_EQUAL(list.size(), 0);
  BOOST_CHECK(list.begin() == list.end());
}

BOOST_AUTO_TEST_CASE(test_iterator_increment)
{
  petrov::List< int > list;
  list.pushBack(10);
  list.pushBack(20);

  petrov::LIter< int > it = list.begin();
  BOOST_CHECK_EQUAL(*it, 10);

  ++it;
  BOOST_CHECK_EQUAL(*it, 20);

  ++it;
  BOOST_CHECK(it == list.end());
}

BOOST_AUTO_TEST_CASE(test_iterator_decrement)
{
  petrov::List< int > list;
  list.pushBack(10);
  list.pushBack(20);

  petrov::LIter< int > it = list.begin();
  ++it;
  BOOST_CHECK_EQUAL(*it, 20);
  --it;
  BOOST_CHECK_EQUAL(*it, 10);
  BOOST_CHECK(it == list.begin());
}

BOOST_AUTO_TEST_CASE(test_copy_constructor)
{
  petrov::List< int > list1;
  list1.pushBack(1);
  list1.pushBack(2);

  petrov::List< int > list2(list1);
  BOOST_CHECK_EQUAL(list2.size(), 2);
  BOOST_CHECK_EQUAL(list2.front(), 1);
  BOOST_CHECK_EQUAL(list2.back(), 2);

  list1.popFront();
  BOOST_CHECK_EQUAL(list1.size(), 1);
  BOOST_CHECK_EQUAL(list2.size(), 2);
}

BOOST_AUTO_TEST_CASE(test_move_constructor)
{
  petrov::List< int > list1;
  list1.pushBack(100);

  petrov::List< int > list2(std::move(list1));
  BOOST_CHECK_EQUAL(list2.size(), 1);
  BOOST_CHECK_EQUAL(list2.front(), 100);
  BOOST_CHECK_EQUAL(list1.size(), 0);
  BOOST_CHECK(list1.empty());
}

BOOST_AUTO_TEST_CASE(test_copy_assignment)
{
  petrov::List< int > list1;
  list1.pushBack(5);

  petrov::List< int > list2;
  list2.pushBack(10);

  list2 = list1;
  BOOST_CHECK_EQUAL(list2.size(), 1);
  BOOST_CHECK_EQUAL(list2.front(), 5);
}

BOOST_AUTO_TEST_CASE(test_move_assignment)
{
  petrov::List< int > list1;
  list1.pushBack(8);

  petrov::List< int > list2;
  list2 = std::move(list1);

  BOOST_CHECK_EQUAL(list2.size(), 1);
  BOOST_CHECK_EQUAL(list2.front(), 8);
  BOOST_CHECK(list1.empty());
}

BOOST_AUTO_TEST_CASE(test_const_iterator)
{
  petrov::List< int > list;
  list.pushBack(1);
  list.pushBack(2);

  const petrov::List< int > & const_list = list;
  petrov::LCIter< int > it = const_list.cbegin();

  BOOST_CHECK_EQUAL(*it, 1);
  ++it;
  BOOST_CHECK_EQUAL(*it, 2);
  ++it;
  BOOST_CHECK(it == const_list.cend());
}

BOOST_AUTO_TEST_CASE(test_swap)
{
  petrov::List< int > list1;
  list1.pushBack(1);
  petrov::List< int > list2;
  list2.pushBack(2);
  list2.pushBack(3);

  list1.swap(list2);

  BOOST_CHECK_EQUAL(list1.size(), 2);
  BOOST_CHECK_EQUAL(list1.front(), 2);
  BOOST_CHECK_EQUAL(list2.size(), 1);
  BOOST_CHECK_EQUAL(list2.front(), 1);
}

BOOST_AUTO_TEST_SUITE_END()

