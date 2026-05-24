#include <boost/test/unit_test.hpp>
#include "stack.hpp"

using namespace permyakov;

BOOST_AUTO_TEST_SUITE(StackTests)

BOOST_AUTO_TEST_CASE(test_push)
{
  Stack< int > stk;
  stk.push(1);
  BOOST_CHECK_EQUAL(stk.top(), 1);
  stk.push(2);
  BOOST_CHECK_EQUAL(stk.top(), 2);
}

BOOST_AUTO_TEST_CASE(test_pop)
{
  Stack< int > stk;
  stk.push(1);
  stk.pop();
  BOOST_CHECK_EQUAL(stk.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_top)
{
  Stack< int > stk;
  stk.push(1);
  stk.push(2);
  stk.push(3);
  BOOST_CHECK_EQUAL(stk.top(), 3);
  stk.pop();
  BOOST_CHECK_EQUAL(stk.top(), 2);
  stk.pop();
  BOOST_CHECK_EQUAL(stk.top(), 1);
}

BOOST_AUTO_TEST_CASE(test_size)
{
  Stack< int > stk;
  BOOST_CHECK_EQUAL(stk.size(), 0);
  stk.push(1);
  BOOST_CHECK_EQUAL(stk.size(), 1);
  stk.push(2);
  BOOST_CHECK_EQUAL(stk.size(), 2);
  stk.pop();
  BOOST_CHECK_EQUAL(stk.size(), 1);
  stk.pop();
  BOOST_CHECK_EQUAL(stk.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_is_empty)
{
  Stack< int > stk;
  BOOST_CHECK(stk.isEmpty());
  stk.push(1);
  stk.pop();
  BOOST_CHECK(stk.isEmpty());
}

BOOST_AUTO_TEST_SUITE_END()
