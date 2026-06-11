#include <boost/test/unit_test.hpp>
#include "stack.hpp"

BOOST_AUTO_TEST_CASE(new_stack_is_empty)
{
  shirokov::Stack< int > s;
  BOOST_TEST(s.empty());
}

BOOST_AUTO_TEST_CASE(copy_push_make_stack_non_empty)
{
  shirokov::Stack< int > s;
  int a = 1;
  s.push(a);
  BOOST_TEST(!s.empty());
}

BOOST_AUTO_TEST_CASE(move_push_make_stack_non_empty)
{
  shirokov::Stack< int > s;
  s.push(1);
  BOOST_TEST(!s.empty());
}

BOOST_AUTO_TEST_CASE(top_method_return_last_elem)
{
  shirokov::Stack< int > s;
  s.push(1);
  s.push(2);
  s.push(3);
  const shirokov::Stack< int > rs = s;
  BOOST_TEST(rs.top() == 3);
}

BOOST_AUTO_TEST_CASE(pop_throws_exception_if_stack_is_empty)
{
  shirokov::Stack< int > s;
  BOOST_REQUIRE(s.empty());
  BOOST_CHECK_THROW(s.pop(), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(when_using_pop_front_returns_following_element_in_stack)
{
  shirokov::Stack< int > s;
  s.push(1);
  s.push(2);
  s.push(3);
  s.pop();
  BOOST_TEST(s.top() == 2);
}
