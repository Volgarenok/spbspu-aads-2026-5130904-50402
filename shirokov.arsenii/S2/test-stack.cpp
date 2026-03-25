#include <boost/test/unit_test.hpp>
#include "stack.hpp"

BOOST_AUTO_TEST_CASE(new_stack_is_empty)
{
  shirokov::Stack< int > q;
  BOOST_TEST(q.empty());
}
