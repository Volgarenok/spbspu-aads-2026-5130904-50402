#define BOOST_TEST_MODULE StackQueueTest
#include <boost/test/included/unit_test.hpp>
#include <sstream>
#include "StackQueue.hpp"

BOOST_AUTO_TEST_CASE(push_test)
{
  lavrentev::Stack<size_t> s{};
  s.push(1);
  BOOST_TEST(s.top() == 1);
}

BOOST_AUTO_TEST_CASE(drop_test)
{
  lavrentev::Stack<size_t> s{};
  BOOST_CHECK_THROW(s.drop(), std::out_of_range);

  s.push(1);
  s.push(2);
  BOOST_TEST(s.drop() == 2);
  BOOST_TEST(s.top() == 1);
}

BOOST_AUTO_TEST_CASE(empty_test)
{
  lavrentev::Stack<size_t> s{};
  BOOST_TEST(s.empty());
  s.push(1);
  BOOST_TEST(!s.empty());
}

BOOST_AUTO_TEST_CASE(top_test)
{
  lavrentev::Stack<size_t> s{};
  BOOST_CHECK_THROW(s.top(), std::out_of_range);
  s.push(1);
  BOOST_TEST(s.top() == 1);
}
