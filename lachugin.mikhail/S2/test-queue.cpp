#include <boost/test/included/unit_test.hpp>
#include "queue.hpp"

BOOST_AUTO_TEST_SUITE(LachuginQueueTests)

BOOST_AUTO_TEST_CASE(test_queue_basic)
{
  lachugin::Queue< int > q;

  BOOST_CHECK(q.empty());
  BOOST_CHECK_EQUAL(q.size(), 0);

  q.push(1);
  q.push(2);
  q.push(3);

  BOOST_CHECK(!q.empty());
  BOOST_CHECK_EQUAL(q.size(), 3);

  BOOST_CHECK_EQUAL(q.front(), 1);
  q.pop();

  BOOST_CHECK_EQUAL(q.front(), 2);
  q.pop();

  BOOST_CHECK_EQUAL(q.front(), 3);
  q.pop();

  BOOST_CHECK(q.empty());
}

BOOST_AUTO_TEST_SUITE_END()
