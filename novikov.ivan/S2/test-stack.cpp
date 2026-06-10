#include <boost/test/unit_test.hpp>
#include "stack.hpp"

BOOST_AUTO_TEST_CASE(new_stack_is_empty)
{
  novikov::Stack< int > s;
  BOOST_TEST(s.empty());
}

BOOST_AUTO_TEST_CASE(copy_push_fills_stack)
{
  novikov::Stack< int > s;
  int a = 1;
  s.push(a);
  BOOST_TEST(!s.empty());
}

BOOST_AUTO_TEST_CASE(move_push_fills_stack)
{
  novikov::Stack< int > s;
  s.push(1);
  BOOST_TEST(!s.empty());
}

BOOST_AUTO_TEST_CASE(top_returns_last)
{
  novikov::Stack< int > s;
  s.push(1);
  s.push(2);
  s.push(3);
  const novikov::Stack< int > rs = s;
  BOOST_TEST(rs.top() == 3);
}

BOOST_AUTO_TEST_CASE(pop_exception_when_empty_stack)
{
  novikov::Stack< int > s;
  BOOST_REQUIRE(s.empty());
  BOOST_CHECK_THROW(s.pop(), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(top_returns_next_after_pop)
{
  novikov::Stack< int > s;
  s.push(1);
  s.push(2);
  s.push(3);
  s.pop();
  BOOST_TEST(s.top() == 2);
}
