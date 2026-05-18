#include <boost/test/unit_test.hpp>
#include "stack.hpp"

using namespace bukreev;

BOOST_AUTO_TEST_SUITE(StackTests)

BOOST_AUTO_TEST_CASE(test_push)
{
  Stack< int > stack;
  stack.push(1);
  stack.push(2);

  BOOST_CHECK_EQUAL(stack.size(), 2);
}

BOOST_AUTO_TEST_CASE(test_pop)
{
  Stack< int > stack;
  stack.push(1);
  stack.push(2);
  int popped = stack.pop();

  BOOST_CHECK_EQUAL(stack.size(), 1);
  BOOST_CHECK_EQUAL(popped, 2);
  popped = stack.pop();
  BOOST_CHECK_EQUAL(stack.size(), 0);
  BOOST_CHECK_EQUAL(popped, 1);
  BOOST_CHECK(stack.empty());
}

BOOST_AUTO_TEST_SUITE_END()
