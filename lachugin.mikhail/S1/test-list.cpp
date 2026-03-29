#include <stdexcept>
#define BOOST_TEST_MODULE S1
#include <boost/test/included/unit_test.hpp>
#include "common/list.hpp"

using namespace lachugin;

BOOST_AUTO_TEST_SUITE(ListMethodsTests)

BOOST_AUTO_TEST_CASE(constructor_test)
{
  List<int> list;
  BOOST_CHECK(list.begin() == list.end());
}

BOOST_AUTO_TEST_CASE(test_add_single)
{
  List<int> list;

  Node<int>* node = list.add(10);

  BOOST_CHECK(node != nullptr);
  BOOST_CHECK_EQUAL(*list.begin(), 10);
}

BOOST_AUTO_TEST_CASE(test_add_next)
{
  List<int> list;

  Node<int>* n1 = list.add(1);
  Node<int>* n2 = list.addNext(2, n1);
  list.addNext(3, n2);

  auto it = list.begin();

  BOOST_CHECK_EQUAL(*it, 1); ++it;
  BOOST_CHECK_EQUAL(*it, 2); ++it;
  BOOST_CHECK_EQUAL(*it, 3);
}

BOOST_AUTO_TEST_CASE(test_empty_iterators)
{
  List<int> list;
  BOOST_CHECK(list.begin() == list.end());
}

BOOST_AUTO_TEST_CASE(test_iteration)
{
  List<int> list;

  Node<int>* curr = list.add(1);
  curr = list.addNext(2, curr);
  curr = list.addNext(3, curr);

  int expected = 1;
  for (auto it = list.begin(); it != list.end(); ++it)
  {
    BOOST_CHECK_EQUAL(*it, expected);
    ++expected;
  }
}

BOOST_AUTO_TEST_CASE(test_clear)
{
  List<int> list;

  Node<int>* curr = list.add(1);
  curr = list.addNext(2, curr);

  list.clear();

  BOOST_CHECK(list.begin() == list.end());
}

BOOST_AUTO_TEST_CASE(test_copy_constructor)
{
  List<int> list1;

  Node<int>* curr = list1.add(1);
  curr = list1.addNext(2, curr);

  List<int> list2(list1);

  auto it = list2.begin();

  BOOST_CHECK_EQUAL(*it, 1); ++it;
  BOOST_CHECK_EQUAL(*it, 2);
}

BOOST_AUTO_TEST_CASE(test_self_assignment)
{
  List<int> list;

  Node<int>* curr = list.add(1);
  curr = list.addNext(2, curr);

  auto it = list.begin();

  BOOST_CHECK_EQUAL(*it, 1); ++it;
  BOOST_CHECK_EQUAL(*it, 2);
}

BOOST_AUTO_TEST_SUITE_END()
