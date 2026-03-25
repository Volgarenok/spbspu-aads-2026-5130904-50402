#include <boost/test/unit_test.hpp>
#include "queue.hpp"

BOOST_AUTO_TEST_CASE(new_queue_is_empty)
{
  shirokov::Queue< int > q;
  BOOST_TEST(q.empty());
}

BOOST_AUTO_TEST_CASE(copy_push_make_queue_non_empty)
{
  shirokov::Queue< int > q;
  int a = 1;
  q.push(a);
  BOOST_TEST(!q.empty());
}
