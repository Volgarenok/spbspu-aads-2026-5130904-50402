#define BOOST_TEST_MODULE ListTests
#include <boost/test/included/unit_test.hpp>
#include "List.hpp"

BOOST_AUTO_TEST_CASE(test_empty_on_new_list)
{
  novikov::List< int > list;
  BOOST_CHECK(list.empty());
}

BOOST_AUTO_TEST_CASE(test_empty_after_push_back)
{
  novikov::List< int > list;
  list.push_back(1);
  BOOST_CHECK(!list.empty());
}

BOOST_AUTO_TEST_CASE(test_push_back_elements)
{
  novikov::List< int > list;
  list.push_back(1);
  list.push_back(2);
  list.push_back(3);
  auto it = list.begin();
  BOOST_CHECK_EQUAL(*it, 1);
  ++it;
  BOOST_CHECK_EQUAL(*it, 2);
  ++it;
  BOOST_CHECK_EQUAL(*it, 3);
}

BOOST_AUTO_TEST_CASE(test_list_clear)
{
  novikov::List< int > list;
  list.push_back(1);
  list.clear();
  BOOST_CHECK(list.empty());
}

BOOST_AUTO_TEST_CASE(test_copy_construct)
{
  novikov::List< int > list1;
  list1.push_back(1);
  list1.push_back(2);
  novikov::List< int > list2(list1);
  auto it1 = list1.begin();
  auto it2 = list2.begin();
  BOOST_CHECK_EQUAL(*it1, *it2);
  ++it1;
  ++it2;
  BOOST_CHECK_EQUAL(*it1, 2);
  BOOST_CHECK_EQUAL(*it2, 2);
}

BOOST_AUTO_TEST_CASE(test_const_iterator)
{
  novikov::List< int > list;
  list.push_back(100);
  list.push_back(200);
  const novikov::List< int > &const_list = list;
  auto it = const_list.begin();
  BOOST_CHECK_EQUAL(*it, 100);
  ++it;
  BOOST_CHECK_EQUAL(*it, 200);
}

BOOST_AUTO_TEST_CASE(test_assigment_operator)
{
  novikov::List< int > list1;
  list1.push_back(1);
  novikov::List< int > list2;
  list2 = list1;
  auto it1 = list1.begin();
  auto it2 = list2.begin();
  BOOST_CHECK_EQUAL(*it1, *it2);
}

BOOST_AUTO_TEST_CASE(test_list_swap)
{
  novikov::List< int > list1;
  list1.push_back(1);
  list1.push_back(2);
  list1.push_back(3);
  novikov::List< int > list2;
  list2.push_back(4);
  list1.swap(list2);
  auto it1 = list1.begin();
  auto it2 = list2.begin();
  BOOST_CHECK_EQUAL(*it1, 4);
  BOOST_CHECK_EQUAL(*it2, 1);
  ++it2;
  BOOST_CHECK_EQUAL(*it2, 2);
  ++it2;
  BOOST_CHECK_EQUAL(*it2, 3);
}
