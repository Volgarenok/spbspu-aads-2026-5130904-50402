#include <boost/test/unit_test.hpp>
#include "stack.hpp"

using namespace afanasev;

BOOST_AUTO_TEST_SUITE(stackTest)

BOOST_AUTO_TEST_CASE(empty_stack)
{
  Stack< int > st;
  BOOST_CHECK_EQUAL(st.empty(), true);
  BOOST_CHECK_EQUAL(st.size(), 0);
}

BOOST_AUTO_TEST_CASE(push_and_top)
{
  Stack< int > st;
  st.push(42);
  BOOST_CHECK_EQUAL(st.empty(), false);
  BOOST_CHECK_EQUAL(st.size(), 1);
  BOOST_CHECK_EQUAL(st.get(), 42);
}

BOOST_AUTO_TEST_CASE(pop)
{
  Stack< int > st;
  st.push(1);
  st.push(2);
  st.pop();
  BOOST_CHECK_EQUAL(st.size(), 1);
  BOOST_CHECK_EQUAL(st.get(), 1);
}

BOOST_AUTO_TEST_CASE(multiple_push_pop)
{
  Stack< int > st;
  for (int i = 0; i < 10; ++i)
  {
    st.push(i);
  }

  BOOST_CHECK_EQUAL(st.size(), 10);
  for (int i = 9; i >= 0; --i)
  {
    BOOST_CHECK_EQUAL(st.get(), i);
    st.pop();
  }
  BOOST_CHECK_EQUAL(st.empty(), true);
}

BOOST_AUTO_TEST_SUITE_END()
