#include "stack.hpp"
#include <boost/test/unit_test.hpp>

using namespace khalikov;

BOOST_AUTO_TEST_SUITE(StackTests)

BOOST_AUTO_TEST_CASE(isEmpty_test)
{
  Stack< int > stack;
  BOOST_CHECK(stack.isEmpty());
  stack.push(12);
  stack.push(3);
  BOOST_CHECK(!stack.isEmpty());
}

BOOST_AUTO_TEST_CASE(size_test)
{
  Stack< int > stack;
  BOOST_CHECK_EQUAL(stack.size(), 0);
  stack.push(12);
  stack.push(3);
  BOOST_CHECK_EQUAL(stack.size(), 2);
}

BOOST_AUTO_TEST_CASE(top_test)
{
  Stack< int > stack;
  stack.push(12);
  stack.push(3);
  BOOST_CHECK_EQUAL(stack.top(), 3);
  stack.pop();
  BOOST_CHECK_EQUAL(stack.top(), 12);
}

BOOST_AUTO_TEST_CASE(push_test)
{
  Stack< int > stack;
  stack.push(12);
  stack.push(3);
  BOOST_CHECK_EQUAL(stack.top(), 3);
  BOOST_CHECK_EQUAL(stack.size(), 2);
}

BOOST_AUTO_TEST_CASE(pop_test)
{
  Stack< int > stack;
  stack.push(12);
  stack.push(3);
  stack.pop();
  BOOST_CHECK_EQUAL(stack.top(), 12);
  BOOST_CHECK_EQUAL(stack.size(), 1);
}

BOOST_AUTO_TEST_CASE(drop_test)
{
  Stack< int > stack;
  stack.push(12);
  stack.push(3);
  int val = stack.drop();
  BOOST_CHECK_EQUAL(val, 3);
  BOOST_CHECK_EQUAL(stack.size(), 1);
}

BOOST_AUTO_TEST_SUITE_END()
