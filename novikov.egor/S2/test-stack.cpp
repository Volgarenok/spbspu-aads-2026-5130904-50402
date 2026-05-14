#include <boost/test/unit_test.hpp>
#include "stack.hpp"

using namespace novikov;

BOOST_AUTO_TEST_SUITE(StackTest)

BOOST_AUTO_TEST_CASE(empty)
{
  Stack< int > stack;
  BOOST_CHECK(stack.empty() == true);
  stack.push(10);
  BOOST_CHECK(stack.empty() == false);
}

BOOST_AUTO_TEST_CASE(push_and_drop)
{
  Stack< int > stack;
  stack.push(5);
  stack.push(6);
  stack.push(7);

  BOOST_CHECK(stack.drop() == 7);
  BOOST_CHECK(stack.drop() == 6);
  BOOST_CHECK(stack.drop() == 5);
  BOOST_CHECK(stack.empty() == true);
}

BOOST_AUTO_TEST_CASE(front)
{
  Stack< int > stack;
  stack.push(8);
  stack.push(9);
  stack.push(10);

  BOOST_CHECK(stack.front() == 10);
  stack.drop();
  BOOST_CHECK(stack.front() == 9);
}

BOOST_AUTO_TEST_CASE(empty_exception)
{
  Stack< int > stack;
  BOOST_CHECK_THROW(stack.drop(), std::runtime_error);
  BOOST_CHECK_THROW(stack.front(), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(const_front)
{
  const Stack< int > stack;
  BOOST_CHECK_THROW(stack.front(), std::runtime_error);

  Stack< int > stack2;
  stack2.push(42);
  const Stack< int > &const_stack = stack2;
  BOOST_CHECK(const_stack.front() == 42);
}

BOOST_AUTO_TEST_SUITE_END()
