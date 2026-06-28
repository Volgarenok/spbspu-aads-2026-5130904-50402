#include <boost/test/unit_test.hpp>
#include <stdexcept>
#include "stack.hpp"

BOOST_AUTO_TEST_SUITE(stack_tests)

BOOST_AUTO_TEST_CASE(push_and_drop_lifo)
{
  samarin::Stack< int > st;
  st.push(1);
  st.push(2);
  st.push(3);
  BOOST_TEST(st.drop() == 3);
  BOOST_TEST(st.drop() == 2);
  BOOST_TEST(st.drop() == 1);
  BOOST_TEST(st.empty());
}

BOOST_AUTO_TEST_CASE(drop_empty_throws)
{
  samarin::Stack< int > st;
  BOOST_CHECK_THROW(st.drop(), std::logic_error);
}

BOOST_AUTO_TEST_CASE(top_peeks_and_mutates)
{
  samarin::Stack< int > st;
  st.push(5);
  BOOST_TEST(st.top() == 5);
  st.top() = 7;
  BOOST_TEST(st.top() == 7);
}

BOOST_AUTO_TEST_SUITE_END()
