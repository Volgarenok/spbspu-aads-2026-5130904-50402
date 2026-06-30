#include <boost/test/unit_test.hpp>
#include "queue.hpp"

BOOST_AUTO_TEST_CASE(new_queue_is_empty)
{
  alisov::Queue< int > q;
  BOOST_TEST(q.empty());
}

BOOST_AUTO_TEST_CASE(push_make_queue_non_empty)
{
  alisov::Queue< int > q;
  q.push(1);
  BOOST_TEST(!q.empty());
}

BOOST_AUTO_TEST_CASE(get_method_return_first_inserted_elem)
{
  alisov::Queue< int > q;
  q.push(1);
  q.push(2);
  q.push(3);
  const alisov::Queue< int > rq = q;
  BOOST_TEST(rq.get() == 1);
}

BOOST_AUTO_TEST_CASE(pop_method_removes_first_element)
{
  alisov::Queue< int > q;
  q.push(1);
  q.push(2);
  q.pop();
  BOOST_TEST(q.get() == 2);
}

BOOST_AUTO_TEST_CASE(pop_throws_exception_if_queue_is_empty)
{
  alisov::Queue< int > q;
  BOOST_REQUIRE(q.empty());
  BOOST_CHECK_THROW(q.pop(), std::underflow_error);
}

BOOST_AUTO_TEST_CASE(get_throws_exception_if_queue_is_empty)
{
  alisov::Queue< int > q;
  BOOST_REQUIRE(q.empty());
  BOOST_CHECK_THROW(q.get(), std::underflow_error);
}
