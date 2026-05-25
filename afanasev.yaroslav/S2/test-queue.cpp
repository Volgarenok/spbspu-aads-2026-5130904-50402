#include <boost/test/unit_test.hpp>
#include "queue.hpp"

using namespace afanasev;

BOOST_AUTO_TEST_SUITE(queueTest)

BOOST_AUTO_TEST_CASE(empty_queue)
{
  Queue< int > q;
  BOOST_CHECK_EQUAL(q.empty(), true);
  BOOST_CHECK_EQUAL(q.size(), 0);
}

BOOST_AUTO_TEST_CASE(push_and_front)
{
  Queue< int > q;
  q.push(52);
  BOOST_CHECK_EQUAL(q.empty(), false);
  BOOST_CHECK_EQUAL(q.size(), 1);
  BOOST_CHECK_EQUAL(q.get(), 52);
}

BOOST_AUTO_TEST_CASE(pop)
{
  Queue< int > q;
  q.push(1);
  q.push(2);
  q.push(3);
  q.pop();
  BOOST_CHECK_EQUAL(q.size(), 2);
  BOOST_CHECK_EQUAL(q.get(), 2);
  q.pop();
  BOOST_CHECK_EQUAL(q.get(), 3);
}

BOOST_AUTO_TEST_CASE(fifo_order)
{
  Queue< int > q;
  for (int i = 0; i < 5; ++i)
  {
    q.push(i);
  }

  for (int i = 0; i < 5; ++i)
  {
    BOOST_CHECK_EQUAL(q.get(), i);
    q.pop();
  }

  BOOST_CHECK_EQUAL(q.empty(), true);
}

BOOST_AUTO_TEST_SUITE_END()
