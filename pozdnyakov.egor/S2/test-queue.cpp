#include <boost/test/unit_test.hpp>
#include "queue.hpp"

using namespace pozdnyakov;

BOOST_AUTO_TEST_SUITE(QueueTests)

BOOST_AUTO_TEST_CASE(testFifoBehavior)
{
  Queue< int > queue;
  BOOST_CHECK(queue.empty());

  queue.push(10);
  queue.push(20);
  queue.push(30);

  BOOST_CHECK(!queue.empty());
  BOOST_CHECK_EQUAL(queue.front(), 10);

  queue.pop();
  BOOST_CHECK_EQUAL(queue.front(), 20);

  queue.pop();
  BOOST_CHECK_EQUAL(queue.front(), 30);

  queue.pop();
  BOOST_CHECK(queue.empty());
}

BOOST_AUTO_TEST_CASE(testCopyConstructorTailRecovery)
{
  Queue< int > original;
  original.push(1);
  original.push(2);

  Queue< int > copied = original;
  BOOST_CHECK_EQUAL(copied.front(), 1);

  copied.push(3);

  copied.pop();
  copied.pop();
  BOOST_CHECK_EQUAL(copied.front(), 3);
}

BOOST_AUTO_TEST_CASE(testEmptyQueueCopy)
{
  Queue< int > emptyQueue;
  Queue< int > copied = emptyQueue;

  BOOST_CHECK(copied.empty());

  copied.push(42);
  BOOST_CHECK_EQUAL(copied.front(), 42);
}

BOOST_AUTO_TEST_SUITE_END()
