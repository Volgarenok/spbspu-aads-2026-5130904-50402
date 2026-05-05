#include <boost/test/unit_test.hpp>
#include "stack.hpp"

using namespace pozdnyakov;

BOOST_AUTO_TEST_SUITE(StackTests)

BOOST_AUTO_TEST_CASE(testLifoBehavior)
{
  Stack< int > stack;
  BOOST_CHECK(stack.empty());

  stack.push(10);
  stack.push(20);
  stack.push(30);

  BOOST_CHECK(!stack.empty());
  BOOST_CHECK_EQUAL(stack.top(), 30);

  stack.pop();
  BOOST_CHECK_EQUAL(stack.top(), 20);

  stack.pop();
  BOOST_CHECK_EQUAL(stack.top(), 10);

  stack.pop();
  BOOST_CHECK(stack.empty());
}

BOOST_AUTO_TEST_CASE(testClearMethod)
{
  Stack< int > stack;
  stack.push(1);
  stack.push(2);

  stack.clear();
  BOOST_CHECK(stack.empty());
}

BOOST_AUTO_TEST_SUITE_END()
