#include <boost/test/unit_test.hpp>
#include <stdexcept>
#include "queue.hpp"
BOOST_AUTO_TEST_SUITE(queue_suite)
BOOST_AUTO_TEST_CASE(push_and_front) {
  sogdanov::Queue<int> q;
  BOOST_CHECK(q.empty());
  q.push(99);
  BOOST_CHECK(!q.empty());
  BOOST_CHECK_EQUAL(q.size(), 1u);
  BOOST_CHECK_EQUAL(q.front(), 99);
}
BOOST_AUTO_TEST_CASE(fifo_order) {
  sogdanov::Queue<int> q;
  q.push(1); q.push(2); q.push(3);
  BOOST_CHECK_EQUAL(q.front(), 1); q.pop();
  BOOST_CHECK_EQUAL(q.front(), 2); q.pop();
  BOOST_CHECK_EQUAL(q.front(), 3); q.pop();
  BOOST_CHECK(q.empty());
}
BOOST_AUTO_TEST_CASE(size_tracking) {
  sogdanov::Queue<int> q;
  for (int i = 0; i < 4; ++i) {
    q.push(i);
  }
  BOOST_CHECK_EQUAL(q.size(), 4u);
  q.pop();
  BOOST_CHECK_EQUAL(q.size(), 3u);
}
BOOST_AUTO_TEST_CASE(front_on_empty_throws) {
  sogdanov::Queue<int> q;
  BOOST_CHECK_THROW(q.front(), std::runtime_error);
}
BOOST_AUTO_TEST_CASE(pop_on_empty_throws) {
 sogdanov::Queue<int> q;
 BOOST_CHECK_THROW(q.pop(), std::runtime_error);
}
BOOST_AUTO_TEST_CASE(copy_is_independent) {
  sogdanov::Queue<int> q;
  q.push(1); q.push(2); q.push(3);
  sogdanov::Queue<int> q2 = q;
  BOOST_CHECK_EQUAL(q2.front(), 1); q2.pop();
  BOOST_CHECK_EQUAL(q2.front(), 2); q2.pop();
  BOOST_CHECK_EQUAL(q2.front(), 3); q2.pop();
  BOOST_CHECK_EQUAL(q.size(), 3u);
}
BOOST_AUTO_TEST_CASE(move_leaves_source_empty) {
  sogdanov::Queue<int> q;
  q.push(5); q.push(6);
  sogdanov::Queue<int> q2 = std::move(q);
  BOOST_CHECK_EQUAL(q2.front(), 5);
  BOOST_CHECK(q.empty());
}
BOOST_AUTO_TEST_CASE(interleaved_push_pop) {
  sogdanov::Queue<int> q;
  q.push(1); q.push(2);
  BOOST_CHECK_EQUAL(q.front(), 1); q.pop();
  q.push(3);
  BOOST_CHECK_EQUAL(q.front(), 2); q.pop();
  BOOST_CHECK_EQUAL(q.front(), 3); q.pop();
  BOOST_CHECK(q.empty());
}
BOOST_AUTO_TEST_SUITE_END()
