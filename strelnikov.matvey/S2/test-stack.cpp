#include "boost/test/unit_test.hpp"
#include "../common/stack.hpp"

using namespace strelnikov;

BOOST_AUTO_TEST_SUITE(STACK_TESTS)

BOOST_AUTO_TEST_CASE(STACK_EMPTY_TEST)
{
  Stack< int > st;

  BOOST_CHECK_EQUAL(st.empty(), true);
  st.push(5);
  BOOST_CHECK_EQUAL(st.empty(), false);
}

BOOST_AUTO_TEST_CASE(STACK_GET_TEST)
{
  Stack< int > st;
  st.push(5);
  BOOST_CHECK_EQUAL(st.get(), 5);
}

BOOST_AUTO_TEST_CASE(STACK_PUSH_TEST)
{
  Stack< int > st;
  int to_push = 5;
  st.push(to_push);
  BOOST_CHECK_EQUAL(st.get(), to_push);
}

BOOST_AUTO_TEST_CASE(STACK_NO_COPY_PUSH_TEST)
{
  Stack< int > st;
  int to_push = 5;
  st.push(std::move(to_push));
  BOOST_CHECK_EQUAL(st.get(), 5);
}

BOOST_AUTO_TEST_CASE(STACK_POP_TEST)
{
  Stack< int > st;
  st.push(5);
  BOOST_CHECK_EQUAL(st.get(), 5);
  st.pop();
  BOOST_CHECK_EQUAL(st.empty(), true);
}

BOOST_AUTO_TEST_SUITE_END()
