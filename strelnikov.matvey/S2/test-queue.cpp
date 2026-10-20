#include "boost/test/unit_test.hpp"
#include "../common/queue.hpp"

using namespace strelnikov;

BOOST_AUTO_TEST_SUITE(QUEUE_TESTS)

BOOST_AUTO_TEST_CASE(QUEUE_EMPTY_TEST)
{
  Queue< int > q;

  BOOST_CHECK_EQUAL(q.empty(), true);
  q.push(5);
  BOOST_CHECK_EQUAL(q.empty(), false);
}

BOOST_AUTO_TEST_CASE(QUEUE_GET_TEST)
{
  Queue< int > q;
  q.push(5);
  BOOST_CHECK_EQUAL(q.get(), 5);
}

BOOST_AUTO_TEST_CASE(QUEUE_PUSH_TEST)
{
  Queue< int > q;
  int to_push = 5;
  q.push(to_push);
  BOOST_CHECK_EQUAL(q.get(), to_push);
}

BOOST_AUTO_TEST_CASE(QUEUE_NO_COPY_PUSH_TEST)
{
  Queue< int > q;
  int to_push = 5;
  q.push(std::move(to_push));
  BOOST_CHECK_EQUAL(q.get(), to_push);
}

BOOST_AUTO_TEST_CASE(QUEUE_POP_TEST)
{
  Queue< int > q;
  q.push(5);
  BOOST_CHECK_EQUAL(q.get(), 5);
  q.pop();
  BOOST_CHECK_EQUAL(q.empty(), true);
}

BOOST_AUTO_TEST_SUITE_END()
