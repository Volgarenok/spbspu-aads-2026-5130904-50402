#define BOOST_TEST_MODULE S1
#include <stdexcept>
#include <boost/test/included/unit_test.hpp>
#include "list.hpp"

BOOST_AUTO_TEST_CASE(default_constructor)
{
  alisov::BiList< int > A{};
  BOOST_CHECK(A.empty());
}

BOOST_AUTO_TEST_CASE(empty_list_iterators)
{
  alisov::BiList< int > A{};
  BOOST_REQUIRE(A.empty());
  BOOST_CHECK(A.begin() == A.end());
}

BOOST_AUTO_TEST_CASE(pushBack_single_element)
{
  alisov::BiList< int > A{};
  A.pushBack(5);
  BOOST_CHECK(!A.empty());
  BOOST_CHECK(A.front() == A.back());
  BOOST_CHECK(A.back() == 5);
}

BOOST_AUTO_TEST_CASE(pushBack_two_elements)
{
  alisov::BiList< int > A{};
  A.pushBack(1);
  A.pushBack(2);

  auto it = A.begin();
  BOOST_CHECK(*it == 1);
  ++it;
  BOOST_CHECK(*it == 2);
  --it;
  BOOST_CHECK(*it == 1);
  BOOST_CHECK(A.front() == 1);
  BOOST_CHECK(A.back() == 2);
}

BOOST_AUTO_TEST_CASE(pushFront_single_element)
{
  alisov::BiList< int > A{};
  A.pushFront(5);
  BOOST_CHECK(!A.empty());
  BOOST_CHECK(A.front() == 5);
  BOOST_CHECK(A.back() == 5);
}

BOOST_AUTO_TEST_CASE(pushFront_two_elements)
{
  alisov::BiList< int > A{};
  A.pushFront(1);
  A.pushFront(2);

  auto it = A.cbegin();
  BOOST_CHECK(*it == 2);
  ++it;
  BOOST_CHECK(*it == 1);
  --it;
  BOOST_CHECK(*it == 2);
  BOOST_CHECK(A.front() == 2);
  BOOST_CHECK(A.back() == 1);
}

BOOST_AUTO_TEST_CASE(pop_operations)
{
  alisov::BiList< int > A{};
  A.pushBack(1);
  A.pushBack(2);
  A.pushFront(3);
  A.pop_back();
  A.pop_front();

  BOOST_CHECK(A.front() == A.back());
  BOOST_CHECK(A.front() == 1);
}

BOOST_AUTO_TEST_CASE(pop_on_empty_list)
{
  alisov::BiList< int > A{};
  BOOST_REQUIRE(A.empty());
  BOOST_CHECK_THROW(A.pop_back(), std::out_of_range);
  BOOST_CHECK_THROW(A.pop_front(), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(copy_constructor)
{
  alisov::BiList< int > A{};
  A.pushBack(1);
  A.pushBack(2);

  alisov::BiList< int > B(A);
  BOOST_CHECK(B.front() == 1);
  BOOST_CHECK(B.back() == 2);

  alisov::BiList< int > C{};
  C = B;
  BOOST_CHECK(C.front() == 1);
  BOOST_CHECK(C.back() == 2);
}

BOOST_AUTO_TEST_CASE(iterator_comparison)
{
  alisov::BiList< int > A{};
  A.pushBack(1);

  auto it1 = A.begin();
  ++it1;
  auto it2 = A.begin();
  BOOST_CHECK(it1 != it2);
  ++it2;
  BOOST_CHECK(it1 == it2);

  auto it3 = A.cbegin();
  ++it3;
  auto it4 = A.cbegin();
  BOOST_CHECK(it3 != it4);
  ++it4;
  BOOST_CHECK(it3 == it4);
}