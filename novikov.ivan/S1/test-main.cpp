#include <stdexcept>
#define BOOST_TEST_MODULE S1
#include <boost/test/included/unit_test.hpp>
#include "list.hpp"

BOOST_AUTO_TEST_CASE(empty_on_creation)
{
  novikov::List< int > A{};
  BOOST_TEST(A.empty());
}

BOOST_AUTO_TEST_CASE(empty_begin_equal_to_end)
{
  novikov::List< int > A{};
  BOOST_REQUIRE(A.empty());
  BOOST_CHECK(A.begin() == A.end());
}

BOOST_AUTO_TEST_CASE(push_back_one)
{
  novikov::List< int > A{};
  A.push_back(5);
  BOOST_TEST(!A.empty());
  BOOST_TEST(A.front() == 5);
  BOOST_TEST(A.back() == 5);
}

BOOST_AUTO_TEST_CASE(push_back_two)
{
  novikov::List< int > A{};
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
  novikov::List< int > A{};
  int n = 10;
  A.push_front(n);
  BOOST_TEST(!A.empty());
  BOOST_TEST(A.front() == n);
  BOOST_TEST(A.back() == n);
}

BOOST_AUTO_TEST_CASE(push_front_two)
{
  novikov::List< int > A{};
  A.push_front(1);
  A.push_front(2);

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
  novikov::List< int > A{};
  A.push_back(1);
  A.push_back(2);
  A.push_front(3);
  A.pop_back();
  A.pop_front();
  BOOST_TEST(A.front() == A.back());
  BOOST_TEST(A.front() == 1);
}

BOOST_AUTO_TEST_CASE(pop_empty)
{
  novikov::List< int > A{};
  BOOST_REQUIRE(A.empty());
  BOOST_CHECK_THROW(A.pop_back(), std::out_of_range);
  BOOST_CHECK_THROW(A.pop_front(), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(copy)
{
  novikov::List< int > A{};
  A.push_back(1);
  A.push_back(2);
  novikov::List< int > B(A);
  BOOST_TEST(B.back() == 2);
  BOOST_TEST(B.front() == 1);
  novikov::List< int > C{};
  C = B;
  BOOST_TEST(C.back() == 2);
  BOOST_TEST(C.front() == 1);
}

BOOST_AUTO_TEST_CASE(move)
{
  novikov::List< int > A{};
  A.push_back(1);
  A.push_back(2);
  novikov::List< int > B(std::move(A));
  BOOST_TEST(B.back() == 2);
  BOOST_TEST(B.front() == 1);
  novikov::List< int > C{};
  C = std::move(B);
  BOOST_TEST(C.back() == 2);
  BOOST_TEST(C.front() == 1);
}

BOOST_AUTO_TEST_CASE(self_copy)
{
  novikov::List< int > A{};
  A.push_back(5);
  A = A;
  BOOST_TEST(A.back() == 5);
}

BOOST_AUTO_TEST_CASE(compare_iterators)
{
  novikov::List< int > A{};
  A.push_back(1);
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
