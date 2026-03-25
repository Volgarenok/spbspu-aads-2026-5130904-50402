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
