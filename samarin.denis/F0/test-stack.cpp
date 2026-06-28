#include <boost/test/unit_test.hpp>
#include "stack.hpp"

BOOST_AUTO_TEST_SUITE(stack)

BOOST_AUTO_TEST_CASE(lifo_order)
{
  samarin::Stack< int > stack;
  stack.push(1);
  stack.push(2);
  stack.push(3);
  BOOST_TEST(stack.drop() == 3);
  BOOST_TEST(stack.drop() == 2);
  BOOST_TEST(stack.drop() == 1);
  BOOST_TEST(stack.empty());
}

BOOST_AUTO_TEST_CASE(top_peeks)
{
  samarin::Stack< int > stack;
  stack.push(10);
  BOOST_TEST(stack.top() == 10);
  BOOST_TEST(!stack.empty());
}

BOOST_AUTO_TEST_CASE(drop_empty_throws)
{
  samarin::Stack< int > stack;
  BOOST_CHECK_THROW(stack.drop(), std::logic_error);
}

BOOST_AUTO_TEST_SUITE_END()
