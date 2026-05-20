#include <stdexcept>
#define BOOST_TEST_MODULE StackQueueTest
#include <boost/test/included/unit_test.hpp>
#include "StackQueue.hpp"

BOOST_AUTO_TEST_CASE(Stack_push_test)
{
  lavrentev::Stack<size_t> s{};
  s.push(1);
  BOOST_TEST(s.top() == 1);
}

BOOST_AUTO_TEST_CASE(Stack_drop_test)
{
  lavrentev::Stack<size_t> s{};
  BOOST_CHECK_THROW(s.drop(), std::out_of_range);

  s.push(1);
  s.push(2);
  BOOST_TEST(s.drop() == 2);
  BOOST_TEST(s.top() == 1);
}

BOOST_AUTO_TEST_CASE(Stack_empty_test)
{
  lavrentev::Stack<size_t> s{};
  BOOST_TEST(s.empty());
  s.push(1);
  BOOST_TEST(!s.empty());
}

BOOST_AUTO_TEST_CASE(Stack_top_test)
{
  lavrentev::Stack<size_t> s{};
  BOOST_CHECK_THROW(s.top(), std::out_of_range);
  s.push(1);
  BOOST_TEST(s.top() == 1);
}

BOOST_AUTO_TEST_CASE(Queue_push_test)
{
  lavrentev::Queue<size_t> s{};
  s.push(1);
  BOOST_TEST(s.front() == 1);
}

BOOST_AUTO_TEST_CASE(Queue_drop_test)
{
  lavrentev::Queue<size_t> s{};
  BOOST_CHECK_THROW(s.drop(), std::out_of_range);

  s.push(1);
  s.push(2);
  BOOST_TEST(s.drop() == 1);
  BOOST_TEST(s.front() == 2);
}

BOOST_AUTO_TEST_CASE(Queue_back_test)
{
  lavrentev::Queue<size_t> s{};
  BOOST_CHECK_THROW(s.back(), std::runtime_error);

  s.push(1);
  s.push(2);
  BOOST_TEST(s.back() == 2);
}

BOOST_AUTO_TEST_CASE(Queue_empty_test)
{
  lavrentev::Queue<size_t> s{};
  BOOST_TEST(s.empty());
  s.push(1);
  BOOST_TEST(!s.empty());
}

BOOST_AUTO_TEST_CASE(Queue_front_test)
{
  lavrentev::Queue<size_t> s{};
  BOOST_CHECK_THROW(s.front(), std::out_of_range);
  s.push(1);
  s.push(2);
  BOOST_TEST(s.front() == 1);
}

BOOST_AUTO_TEST_CASE(Gcd_test)
{
  BOOST_TEST(lavrentev::gcd(1, 0) == 1);
  BOOST_TEST(lavrentev::gcd(0, 1) == 1);
  BOOST_TEST(lavrentev::gcd(25, 15) == 5);
}
