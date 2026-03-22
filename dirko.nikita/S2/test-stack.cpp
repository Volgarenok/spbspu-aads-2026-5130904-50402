#include <boost/test/unit_test.hpp>
#include "stack.hpp"

using namespace dirko;

BOOST_AUTO_TEST_SUITE(stackTest)
BOOST_AUTO_TEST_CASE(empty)
{
  Stack< int > st;
  BOOST_CHECK_EQUAL(st.empty(), true);
}

BOOST_AUTO_TEST_CASE(size)
{
  Stack< int > st;
  BOOST_CHECK_EQUAL(st.size(), 0);
  st.push(1);
  BOOST_CHECK_EQUAL(st.size(), 1);
}

BOOST_AUTO_TEST_SUITE_END()
