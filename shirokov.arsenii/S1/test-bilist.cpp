#include <boost/test/unit_test.hpp>
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
  A.push_back(5);
  BOOST_TEST(!A.empty());
  BOOST_TEST(A.front() == 5);
  BOOST_TEST(A.back() == 5);
}

BOOST_AUTO_TEST_CASE(push_back_two)
{
  shirokov::BiList< int > A{};
  A.push_back(1);
  A.push_back(2);

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
  A.push_front(n);
  BOOST_TEST(!A.empty());
  BOOST_TEST(A.front() == n);
  BOOST_TEST(A.back() == n);
}

BOOST_AUTO_TEST_CASE(push_front_two)
{
  shirokov::BiList< int > A{};
  A.push_front(1);
  A.push_front(2);

  auto it = A.begin();
  BOOST_TEST(*it == 2);
  ++it;
  BOOST_TEST(*it == 1);
  --it;
  BOOST_TEST(*it == 2);
  BOOST_TEST(A.front() == 2);
  BOOST_TEST(A.back() == 1);
}
