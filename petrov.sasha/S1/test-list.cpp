#include <boost/test/unit_test.hpp>

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
  list.push_back(10);
  BOOST_CHECK(!list.empty());
  BOOST_CHECK_EQUAL(list.size(), 1);
  BOOST_CHECK_EQUAL(list.front(), 10);
  BOOST_CHECK_EQUAL(list.back(), 10);

  list.push_back(20);
  BOOST_CHECK_EQUAL(list.size(), 2);
  BOOST_CHECK_EQUAL(list.front(), 10);
  BOOST_CHECK_EQUAL(list.back(), 20);
}

BOOST_AUTO_TEST_CASE(test_push_front)
{
  petrov::List< int > list;
  list.push_front(10);
  BOOST_CHECK_EQUAL(list.front(), 10);

  list.push_front(20);
  BOOST_CHECK_EQUAL(list.front(), 20);
  BOOST_CHECK_EQUAL(list.back(), 10);
  BOOST_CHECK_EQUAL(list.size(), 2);
}

BOOST_AUTO_TEST_CASE(test_pop_front)
{
  petrov::List< int > list;
  list.push_back(1);
  list.push_back(2);

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
  list.push_back(1);
  list.push_back(2);

  list.popBack();
  BOOST_CHECK_EQUAL(list.size(), 1);
  BOOST_CHECK_EQUAL(list.back(), 1);

  list.popBack();
  BOOST_CHECK(list.empty());
}

BOOST_AUTO_TEST_CASE(test_clear)
{
  petrov::List< int > list;
  list.push_back(1);
  list.push_back(2);
  list.push_back(3);

  list.clear();
  BOOST_CHECK(list.empty());
  BOOST_CHECK_EQUAL(list.size(), 0);
  BOOST_CHECK(list.begin() == list.end());
}

BOOST_AUTO_TEST_CASE(test_iterator_increment)
{
  petrov::List< int > list;
  list.push_back(10);
  list.push_back(20);

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
  list.push_back(10);
  list.push_back(20);

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
  list1.push_back(1);
  list1.push_back(2);

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
  list1.push_back(100);

  petrov::List< int > list2(std::move(list1));
  BOOST_CHECK_EQUAL(list2.size(), 1);
  BOOST_CHECK_EQUAL(list2.front(), 100);
  BOOST_CHECK_EQUAL(list1.size(), 0);
  BOOST_CHECK(list1.empty());
}

BOOST_AUTO_TEST_CASE(test_copy_assignment)
{
  petrov::List< int > list1;
  list1.push_back(5);

  petrov::List< int > list2;
  list2.push_back(10);

  list2 = list1;
  BOOST_CHECK_EQUAL(list2.size(), 1);
  BOOST_CHECK_EQUAL(list2.front(), 5);
}

BOOST_AUTO_TEST_CASE(test_move_assignment)
{
  petrov::List< int > list1;
  list1.push_back(8);

  petrov::List< int > list2;
  list2 = std::move(list1);

  BOOST_CHECK_EQUAL(list2.size(), 1);
  BOOST_CHECK_EQUAL(list2.front(), 8);
  BOOST_CHECK(list1.empty());
}

BOOST_AUTO_TEST_CASE(test_const_iterator)
{
  petrov::List< int > list;
  list.push_back(1);
  list.push_back(2);

  const petrov::List< int >& const_list = list;
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
  list1.push_back(1);
  petrov::List< int > list2;
  list2.push_back(2);
  list2.push_back(3);

  list1.swap(list2);

  BOOST_CHECK_EQUAL(list1.size(), 2);
  BOOST_CHECK_EQUAL(list1.front(), 2);
  BOOST_CHECK_EQUAL(list2.size(), 1);
  BOOST_CHECK_EQUAL(list2.front(), 1);
}

BOOST_AUTO_TEST_CASE(test_splice)
{
  petrov::List< int > list1;
  list1.push_back(1);
  list1.push_back(2);

  petrov::List< int > list2;
  list2.push_back(10);
  list2.push_back(20);
  list2.push_back(30);

  petrov::List< int > tmp_rvalue;
  tmp_rvalue.push_back(100);
  tmp_rvalue.push_back(200);
  tmp_rvalue.push_back(300);
  list1.splice(list1.end(), std::move(tmp_rvalue));
  BOOST_CHECK_EQUAL(list1.size(), 5);
  BOOST_CHECK(tmp_rvalue.empty());
  petrov::LIter< int > check = list1.begin();
  BOOST_CHECK_EQUAL(*check++, 1);
  BOOST_CHECK_EQUAL(*check++, 2);
  BOOST_CHECK_EQUAL(*check++, 100);
  BOOST_CHECK_EQUAL(*check++, 200);
  BOOST_CHECK_EQUAL(*check, 300);
  list1.popBack();
  list1.popBack();
  list1.popBack();

  petrov::List< int > tmp1;
  tmp1.push_back(100);
  tmp1.push_back(200);
  list1.splice(list1.end(), tmp1);
  BOOST_CHECK_EQUAL(list1.size(), 4);
  BOOST_CHECK_EQUAL(tmp1.size(), 0);

  petrov::LIter< int > it2 = list2.begin();
  ++it2;
  list1.splice(list1.begin(), list2, it2);
  BOOST_CHECK_EQUAL(list1.size(), 5);
  BOOST_CHECK_EQUAL(list2.size(), 2);

  petrov::LIter< int > first = list2.begin();
  petrov::LIter< int > last = list2.begin();
  ++last;
  list1.splice(list1.end(), list2, first, last);
  BOOST_CHECK_EQUAL(list1.size(), 6);
  BOOST_CHECK_EQUAL(list2.size(), 1);
}

BOOST_AUTO_TEST_CASE(test_sort)
{
  petrov::List< int > list;
  list.push_back(5);
  list.push_back(2);
  list.push_back(8);
  list.push_back(1);
  list.push_back(4);

  list.sort();

  petrov::LIter< int > it = list.begin();
  BOOST_CHECK_EQUAL(*it++, 1);
  BOOST_CHECK_EQUAL(*it++, 2);
  BOOST_CHECK_EQUAL(*it++, 4);
  BOOST_CHECK_EQUAL(*it++, 5);
  BOOST_CHECK_EQUAL(*it, 8);
}

BOOST_AUTO_TEST_CASE(test_merge)
{
  petrov::List< int > list1;
  list1.push_back(1);
  list1.push_back(3);
  list1.push_back(5);

  petrov::List< int > list2;
  list2.push_back(2);
  list2.push_back(4);
  list2.push_back(6);

  list1.merge(list2);

  BOOST_CHECK_EQUAL(list1.size(), 6);
  BOOST_CHECK_EQUAL(list2.size(), 0);

  petrov::LIter< int > it = list1.begin();
  for (int i = 1; i <= 6; ++i) {
    BOOST_CHECK_EQUAL(*it++, i);
  }
}

BOOST_AUTO_TEST_CASE(test_partition)
{
  petrov::List< int > list;
  list.push_back(1);
  list.push_back(2);
  list.push_back(3);
  list.push_back(4);
  list.push_back(5);

  petrov::LIter< int > split = list.partition([](int v) {
    return v % 2 == 0;
  });

  BOOST_CHECK_EQUAL(*split, 1);

  petrov::LIter< int > it = list.begin();
  BOOST_CHECK_EQUAL(*it++, 2);
  BOOST_CHECK_EQUAL(*it++, 4);
  BOOST_CHECK_EQUAL(*it++, 1);
  BOOST_CHECK_EQUAL(*it++, 3);
  BOOST_CHECK_EQUAL(*it, 5);
}

BOOST_AUTO_TEST_SUITE_END()

