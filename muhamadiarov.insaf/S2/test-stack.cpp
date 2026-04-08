#include <boost/test/unit_test.hpp>
#include "stack.hpp"

using namespace muhamadiarov;

BOOST_AUTO_TEST_SUITE(stackTest)
BOOST_AUTO_TEST_CASE(size)
{
  Stack< int > stack;
  BOOST_CHECK(stack.size() == 0);
  stack.push(7);
  BOOST_CHECK(stack.size() == 1);
}
BOOST_AUTO_TEST_CASE(push)
{
  Stack< int > stack;
  stack.push(5);
  stack.push(6);
  BOOST_CHECK(stack.size() == 2);
  BOOST_CHECK(stack.top() == 6)
}
BOOST_AUTO_TEST_CASE(pop)
{
  Stack< int > stack;
  stack.push(8);
  stack.push(7)
  stack.pop();
  BOOST_CHECK(stack.top() == 8);
}
BOOST_AUTO_TEST_CASE(top)
{
  Stack< int > stack;
  stack.push(8);
  stack.push(9);
  BOOST_CHECK(stack.top() == 9);
}
BOOST_AUTO_TEST_CASE(empty)
{
  Stack< int > stack;
  BOOST_CHECK(stack.empty() == true);
  stack.push(10);
  BOOST_CHECK(stack.empty() == false);
}
BOOST_AUTO_TEST_SUITE_END
