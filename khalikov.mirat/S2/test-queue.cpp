#include "queue.hpp"
#include <boost/test/unit_test.hpp>

using namespace khalikov;

BOOST_AUTO_TEST_SUITE(QueueTests)

BOOST_AUTO_TEST_CASE(isEmpty_test)
{
  Queue< int > queue;
  BOOST_CHECK(queue.isEmpty());
  queue.push(12);
  queue.push(3);
  BOOST_CHECK(!queue.isEmpty());
}

BOOST_AUTO_TEST_CASE(size_test)
{
  Queue< int > queue;
  BOOST_CHECK_EQUAL(queue.size(), 0);
  queue.push(12);
  queue.push(3);
  BOOST_CHECK_EQUAL(queue.size(), 2);
}

BOOST_AUTO_TEST_CASE(back_test)
{
  Queue< int > queue;
  queue.push(12);
  queue.push(3);
  BOOST_CHECK_EQUAL(queue.back(), 3);
  queue.push(15);
  BOOST_CHECK_EQUAL(queue.back(), 15);
}

BOOST_AUTO_TEST_CASE(swap_test)
{
  Queue< int > queue;
  Queue< int > yaqueue;
  queue.push(12);
  queue.push(3);
  yaqueue.push(24);
  BOOST_CHECK_EQUAL(queue.front(), 12);
  BOOST_CHECK_EQUAL(yaqueue.front(), 24);
	BOOST_CHECK_EQUAL(queue.size(), 2);
  BOOST_CHECK_EQUAL(yaqueue.size(), 1);
  queue.swap(yaqueue);
	BOOST_CHECK_EQUAL(yaqueue.size(), 2);
  BOOST_CHECK_EQUAL(queue.size(), 1);
  BOOST_CHECK_EQUAL(yaqueue.front(), 12);
  BOOST_CHECK_EQUAL(queue.front(), 24);
}

BOOST_AUTO_TEST_CASE(front_test)
{
  Queue< int > queue;
  queue.push(12);
  queue.push(3);
  BOOST_CHECK_EQUAL(queue.front(), 12);
  queue.pop();
  BOOST_CHECK_EQUAL(queue.front(), 3);
}

BOOST_AUTO_TEST_CASE(push_test)
{
  Queue< int > queue;
  queue.push(12);
  queue.push(3);
  BOOST_CHECK_EQUAL(queue.back(), 3);
  BOOST_CHECK_EQUAL(queue.size(), 2);
}

BOOST_AUTO_TEST_CASE(pop_test)
{
  Queue< int > queue;
  queue.push(12);
  queue.push(3);
  queue.pop();
  BOOST_CHECK_EQUAL(queue.front(), 3);
  BOOST_CHECK_EQUAL(queue.size(), 1);
}

BOOST_AUTO_TEST_CASE(drop_test)
{
  Queue< int > queue;
  queue.push(12);
  queue.push(3);
  int val = queue.drop();
  BOOST_CHECK_EQUAL(val, 12);
  BOOST_CHECK_EQUAL(queue.size(), 1);
}

BOOST_AUTO_TEST_SUITE_END()
