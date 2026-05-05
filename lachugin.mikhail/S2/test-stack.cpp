#include <boost/test/unit_test.hpp>
#include "stack.hpp"

using namespace lachugin;

BOOST_AUTO_TEST_SUITE(StackTests)

BOOST_AUTO_TEST_CASE(test_default_constructor)
{
  Stack< long long > stack;
  BOOST_CHECK(stack.empty());
  BOOST_CHECK_EQUAL(stack.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_push_and_size)
{
  Stack< long long > stack;
  stack.push(1);
  BOOST_CHECK_EQUAL(stack.size(), 1);
  BOOST_CHECK(!stack.empty());

  stack.push(2);
  BOOST_CHECK_EQUAL(stack.size(), 2);
}

BOOST_AUTO_TEST_CASE(test_top)
{
  Stack< long long > stack;
  stack.push(1);
  BOOST_CHECK_EQUAL(stack.top(), 1);

  stack.push(2);
  BOOST_CHECK_EQUAL(stack.top(), 2);

  stack.top() = 3;
  BOOST_CHECK_EQUAL(stack.top(), 3);
}

BOOST_AUTO_TEST_CASE(test_pop)
{
  Stack< long long > stack;
  stack.push(1);
  stack.push(2);

  stack.pop();
  BOOST_CHECK_EQUAL(stack.size(), 1);
}

BOOST_AUTO_TEST_SUITE_END()
