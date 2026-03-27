#include <boost/test/unit_test.hpp>
#include "Stack.hpp"
#include "Queue.hpp"

namespace chernikov {
  BOOST_AUTO_TEST_SUITE(StackTests)

  BOOST_AUTO_TEST_CASE(StackEmptyInitially)
  {
    Stack< int > stack;
    BOOST_CHECK(stack.empty());
    BOOST_CHECK_EQUAL(stack.size(), 0);
  }
  BOOST_AUTO_TEST_CASE(StackPushAndTop)
  {
    Stack< int > stack;
    stack.push(42);
    BOOST_CHECK(!stack.empty());
    BOOST_CHECK_EQUAL(stack.size(), 1);
    BOOST_CHECK_EQUAL(stack.top(), 42);
  }
  BOOST_AUTO_TEST_CASE(StackPushMultipleElements)
  {
    Stack< int > stack;
    stack.push(10);
    stack.push(20);
    stack.push(30);
    BOOST_CHECK_EQUAL(stack.size(), 3);
    BOOST_CHECK_EQUAL(stack.top(), 30);
  }
  BOOST_AUTO_TEST_CASE(StackDropRemovesTop)
  {
    Stack< int > stack;
    stack.push(100);
    stack.push(200);
    stack.push(300);
    int value = stack.drop();
    BOOST_CHECK_EQUAL(value, 300);
    BOOST_CHECK_EQUAL(stack.size(), 2);
    BOOST_CHECK_EQUAL(stack.top(), 200);
  }
  BOOST_AUTO_TEST_CASE(StackLIFO)
  {
    Stack< int > stack;
    stack.push(1);
    stack.push(2);
    stack.push(3);
    BOOST_CHECK_EQUAL(stack.drop(), 3);
    BOOST_CHECK_EQUAL(stack.drop(), 2);
    BOOST_CHECK_EQUAL(stack.drop(), 1);
    BOOST_CHECK(stack.empty());
  }

  BOOST_AUTO_TEST_SUITE_END()

  BOOST_AUTO_TEST_SUITE(QueueTests)

  BOOST_AUTO_TEST_CASE(QueueEmptyInitially)
  {
    Queue< int > queue;
    BOOST_CHECK(queue.empty());
    BOOST_CHECK_EQUAL(queue.size(), 0);
  }
  BOOST_AUTO_TEST_CASE(QueuePushAndFront)
  {
    Queue< int > queue;
    queue.push(42);
    BOOST_CHECK(!queue.empty());
    BOOST_CHECK_EQUAL(queue.size(), 1);
    BOOST_CHECK_EQUAL(queue.front(), 42);
  }
  BOOST_AUTO_TEST_CASE(QueuePushMultipleElements)
  {
    Queue< int > queue;
    queue.push(10);
    queue.push(20);
    queue.push(30);
    BOOST_CHECK_EQUAL(queue.size(), 3);
    BOOST_CHECK_EQUAL(queue.front(), 10);
  }
  BOOST_AUTO_TEST_CASE(QueueDropRemovesFront)
  {
    Queue< int > queue;
    queue.push(100);
    queue.push(200);
    queue.push(300);
    int value = queue.drop();
    BOOST_CHECK_EQUAL(value, 100);
    BOOST_CHECK_EQUAL(queue.size(), 2);
    BOOST_CHECK_EQUAL(queue.front(), 200);
  }
  BOOST_AUTO_TEST_CASE(QueueFIFO)
  {
    Queue< int > queue;
    queue.push(1);
    queue.push(2);
    queue.push(3);
    BOOST_CHECK_EQUAL(queue.drop(), 1);
    BOOST_CHECK_EQUAL(queue.drop(), 2);
    BOOST_CHECK_EQUAL(queue.drop(), 3);
    BOOST_CHECK(queue.empty());
  }

  BOOST_AUTO_TEST_SUITE_END()
}
