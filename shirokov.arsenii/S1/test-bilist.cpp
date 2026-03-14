#include <stdexcept>
#define BOOST_TEST_MODULE S1
#include <boost/test/included/unit_test.hpp>
#include "bilist.hpp"

BOOST_AUTO_TEST_CASE(just_created_it_means_empty)
{
  shirokov::BiList< int > A{};
  BOOST_TEST(A.empty());
}

BOOST_AUTO_TEST_CASE(empty_begin_equal_to_end)
{
  shirokov::BiList< int > A{};
  BOOST_REQUIRE(A.empty());
  BOOST_CHECK(A.begin() == A.end());
}

BOOST_AUTO_TEST_CASE(push_back_one)
{
  shirokov::BiList< int > A{};
  A.pushBack(5);
  BOOST_TEST(!A.empty());
  BOOST_TEST(A.front() == 5);
  BOOST_TEST(A.back() == 5);
}

BOOST_AUTO_TEST_CASE(push_back_two)
{
  shirokov::BiList< int > A{};
  A.pushBack(1);
  A.pushBack(2);

  auto it = A.begin();
  BOOST_TEST(*it == 1);
  ++it;
  BOOST_TEST(*it == 2);
  --it;
  BOOST_TEST(*it == 1);
  BOOST_TEST(A.front() == 1);
  BOOST_TEST(A.back() == 2);
}

BOOST_AUTO_TEST_CASE(push_front_one)
{
  shirokov::BiList< int > A{};
  int n = 10;
  A.pushFront(n);
  BOOST_TEST(!A.empty());
  BOOST_TEST(A.front() == n);
  BOOST_TEST(A.back() == n);
}

BOOST_AUTO_TEST_CASE(push_front_two)
{
  shirokov::BiList< int > A{};
  A.pushFront(1);
  A.pushFront(2);

  auto it = A.cbegin();
  BOOST_TEST(*it == 2);
  ++it;
  BOOST_TEST(*it == 1);
  --it;
  BOOST_TEST(*it == 2);
  BOOST_TEST(A.front() == 2);
  BOOST_TEST(A.back() == 1);
}

BOOST_AUTO_TEST_CASE(pop_two)
{
  shirokov::BiList< int > A{};
  A.pushBack(1);
  A.pushBack(2);
  A.pushFront(3);
  A.popBack();
  A.popFront();
  BOOST_TEST(A.front() == A.back());
  BOOST_TEST(A.front() == 1);
}

BOOST_AUTO_TEST_CASE(pop_empty)
{
  shirokov::BiList< int > A{};
  BOOST_REQUIRE(A.empty());
  BOOST_CHECK_THROW(A.popBack(), std::out_of_range);
  BOOST_CHECK_THROW(A.popFront(), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(copy)
{
  shirokov::BiList< int > A{};
  A.pushBack(1);
  A.pushBack(2);
  shirokov::BiList< int > B(A);
  BOOST_TEST(B.back() == 2);
  BOOST_TEST(B.front() == 1);
  shirokov::BiList< int > C{};
  C = B;
  BOOST_TEST(C.back() == 2);
  BOOST_TEST(C.front() == 1);
}

BOOST_AUTO_TEST_CASE(move)
{
  shirokov::BiList< int > A{};
  A.pushBack(1);
  A.pushBack(2);
  shirokov::BiList< int > B(std::move(A));
  BOOST_TEST(B.back() == 2);
  BOOST_TEST(B.front() == 1);
  shirokov::BiList< int > C{};
  C = std::move(B);
  BOOST_TEST(C.back() == 2);
  BOOST_TEST(C.front() == 1);
}

BOOST_AUTO_TEST_CASE(copying_itself)
{
  shirokov::BiList< int > A{};
  A.pushBack(5);
  A = A;
  BOOST_TEST(A.back() == 5);
}

BOOST_AUTO_TEST_CASE(comparing_iterators)
{
  shirokov::BiList< int > A{};
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
