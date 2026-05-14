#include <boost/test/unit_test.hpp>
#include "stack.hpp"

using namespace karpovich;

BOOST_AUTO_TEST_SUITE(StackMethodsTests)

BOOST_AUTO_TEST_CASE(test_default_constructor)
{
  Stack< int > stack;
  BOOST_CHECK(stack.empty());
  BOOST_CHECK_EQUAL(stack.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_push_and_size)
{
  Stack< int > stack;
  stack.push(1);
  BOOST_CHECK_EQUAL(stack.size(), 1);
  BOOST_CHECK(!stack.empty());

  stack.push(2);
  BOOST_CHECK_EQUAL(stack.size(), 2);
}

BOOST_AUTO_TEST_CASE(test_top)
{
  Stack< int > stack;
  stack.push(10);
  BOOST_CHECK_EQUAL(stack.top(), 10);

  stack.push(20);
  BOOST_CHECK_EQUAL(stack.top(), 20);

  stack.top() = 30;
  BOOST_CHECK_EQUAL(stack.top(), 30);
}

BOOST_AUTO_TEST_CASE(test_pop)
{
  Stack< int > stack;
  stack.push(1);
  stack.push(2);

  stack.pop();
  BOOST_CHECK_EQUAL(stack.size(), 1);
}

BOOST_AUTO_TEST_SUITE_END()
