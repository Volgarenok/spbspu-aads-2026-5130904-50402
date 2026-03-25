#include <boost/test/unit_test.hpp>
#include "queue.hpp"

BOOST_AUTO_TEST_CASE(new_queue_is_empty)
{
  shirokov::Queue< int > q;
  BOOST_TEST(q.empty());
}
