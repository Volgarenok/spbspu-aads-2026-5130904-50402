#include <stdexcept>
#include <boost/test/tools/old/interface.hpp>
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

BOOST_AUTO_TEST_CASE(move_push_make_queue_non_empty)
{
  shirokov::Queue< int > q;
  q.push(1);
  BOOST_TEST(!q.empty());
}

BOOST_AUTO_TEST_CASE(front_method_return_first_elem)
{
  shirokov::Queue< int > q;
  q.push(1);
  q.push(2);
  q.push(3);
  const shirokov::Queue< int > rq = q;
  BOOST_TEST(rq.front() == 1);
}

BOOST_AUTO_TEST_CASE(pop_throws_exception_if_container_is_empty)
{
  shirokov::Queue< int > q;
  BOOST_REQUIRE(q.empty());
  BOOST_CHECK_THROW(q.pop(), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(when_using_pop_front_returns_following_element)
{
  shirokov::Queue< int > q;
  q.push(1);
  q.push(2);
  q.push(3);
  q.pop();
  BOOST_TEST(q.front() == 2);
}
