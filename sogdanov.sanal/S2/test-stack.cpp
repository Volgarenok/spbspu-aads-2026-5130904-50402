#include <boost/test/unit_test.hpp>
#include <stdexcept>
#include <string>
#include "stack.hpp"
BOOST_AUTO_TEST_SUITE(stack_suite)
BOOST_AUTO_TEST_CASE(push_and_top) {
  sogdanov::Stack<int> s;
  BOOST_CHECK(s.empty());
  BOOST_CHECK_EQUAL(s.size(), 0u);
  s.push(10);
  BOOST_CHECK(!s.empty());
  BOOST_CHECK_EQUAL(s.size(), 1u);
  BOOST_CHECK_EQUAL(s.top(), 10);
}
BOOST_AUTO_TEST_CASE(lifo_order) {
  sogdanov::Stack<int> s;
  s.push(1); s.push(2); s.push(3);
  BOOST_CHECK_EQUAL(s.top(), 3); s.pop();
  BOOST_CHECK_EQUAL(s.top(), 2); s.pop();
  BOOST_CHECK_EQUAL(s.top(), 1); s.pop();
  BOOST_CHECK(s.empty());
}
BOOST_AUTO_TEST_CASE(size_tracking)
{
  sogdanov::Stack<int> s;
  for (int i = 0; i < 5; ++i) {
    s.push(i);
  }
  BOOST_CHECK_EQUAL(s.size(), 5u);
  s.pop();
  BOOST_CHECK_EQUAL(s.size(), 4u);
}
BOOST_AUTO_TEST_CASE(top_on_empty_throws) {
  sogdanov::Stack<int> s;
  BOOST_CHECK_THROW(s.top(), std::runtime_error);
}
BOOST_AUTO_TEST_CASE(pop_on_empty_throws) {
  sogdanov::Stack<int> s;
  BOOST_CHECK_THROW(s.pop(), std::runtime_error);
}
BOOST_AUTO_TEST_CASE(copy_is_independent) {
  sogdanov::Stack<int> s;
  s.push(1); s.push(2); s.push(3);
  sogdanov::Stack<int> s2 = s;
  BOOST_CHECK_EQUAL(s2.top(), 3); s2.pop();
  BOOST_CHECK_EQUAL(s2.top(), 2); s2.pop();
  BOOST_CHECK_EQUAL(s2.top(), 1); s2.pop();
  BOOST_CHECK_EQUAL(s.size(), 3u);
}
BOOST_AUTO_TEST_CASE(move_leaves_source_empty) {
  sogdanov::Stack<int> s;
  s.push(10); s.push(20);
  sogdanov::Stack<int> s2 = std::move(s);
  BOOST_CHECK_EQUAL(s2.top(), 20);
  BOOST_CHECK(s.empty());
}
BOOST_AUTO_TEST_SUITE_END()
