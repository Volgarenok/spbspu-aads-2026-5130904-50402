#include <boost/test/unit_test.hpp>
#include "stack.hpp"

BOOST_AUTO_TEST_CASE(new_stack_empty)
{
  alisov::Stack< int > s;
  BOOST_TEST(s.empty());
}

BOOST_AUTO_TEST_CASE(push_make_stack_non_empty)
{
  alisov::Stack< int > s;
  s.push(1);
  BOOST_TEST(!s.empty());
}

BOOST_AUTO_TEST_CASE(get_method_return_last_elem)
{
  alisov::Stack< int > s;
  s.push(1);
  s.push(2);
  s.push(3);
  const alisov::Stack< int > rs = s;
  BOOST_TEST(rs.get() == 3);
}

BOOST_AUTO_TEST_CASE(pop_remove_elem)
{
  alisov::Stack< int > s;
  s.push(1);
  s.push(2);
  s.pop();
  BOOST_TEST(s.get() == 1);
}

BOOST_AUTO_TEST_CASE(pop_throws_exception_if_stack_is_empty)
{
  alisov::Stack< int > s;
  BOOST_REQUIRE(s.empty());
  BOOST_CHECK_THROW(s.pop(), std::underflow_error);
}

BOOST_AUTO_TEST_CASE(get_throws_exception_if_stack_is_empty)
{
  alisov::Stack< int > s;
  BOOST_REQUIRE(s.empty());
  BOOST_CHECK_THROW(s.get(), std::underflow_error);
}
