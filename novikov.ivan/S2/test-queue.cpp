#include <stdexcept>
#include <boost/test/unit_test.hpp>
#include "queue.hpp"

BOOST_AUTO_TEST_CASE(new_queue_is_empty)
{
  novikov::Queue< int > q;
  BOOST_TEST(q.empty());
}

BOOST_AUTO_TEST_CASE(copy_push_fills_queue)
{
  novikov::Queue< int > q;
  int a = 1;
  q.push(a);
  BOOST_TEST(!q.empty());
}

BOOST_AUTO_TEST_CASE(move_push_fills_queue)
{
  novikov::Queue< int > q;
  q.push(1);
  BOOST_TEST(!q.empty());
}

BOOST_AUTO_TEST_CASE(front_returns_first)
{
  novikov::Queue< int > q;
  q.push(1);
  q.push(2);
  q.push(3);
  const novikov::Queue< int > rq = q;
  BOOST_TEST(rq.front() == 1);
}

BOOST_AUTO_TEST_CASE(pop_exception_when_empty_queue)
{
  novikov::Queue< int > q;
  BOOST_REQUIRE(q.empty());
  BOOST_CHECK_THROW(q.pop(), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(front_returns_next_after_pop)
{
  novikov::Queue< int > q;
  q.push(1);
  q.push(2);
  q.push(3);
  q.pop();
  BOOST_TEST(q.front() == 2);
}
