#include <boost/test/unit_test.hpp>
#include "queue.hpp"

using namespace dirko;

BOOST_AUTO_TEST_SUITE(queueTest)
BOOST_AUTO_TEST_CASE(empty)
{
  Queue< int > q;
  BOOST_CHECK_EQUAL(q.empty(), true);
}

BOOST_AUTO_TEST_CASE(size)
{
  Queue< int > q;
  BOOST_CHECK_EQUAL(q.size(), 0);
  q.push(1);
  BOOST_CHECK_EQUAL(q.size(), 1);
}

BOOST_AUTO_TEST_CASE(test_push_get)
{
  Queue< int > q;
  q.push(1);
  BOOST_CHECK_EQUAL(q.get(), 1);
}
BOOST_AUTO_TEST_CASE(test_pop)
{
  Queue< int > q;
  q.push(1);
  q.push(2);
  q.pop();
  BOOST_CHECK_EQUAL(q.get(), 2);
}
BOOST_AUTO_TEST_SUITE_END()
