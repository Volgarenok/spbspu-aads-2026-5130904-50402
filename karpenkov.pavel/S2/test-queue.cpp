#include <boost/test/unit_test.hpp>

#include "queue.hpp"

BOOST_AUTO_TEST_CASE(push_front_test) {
  karpenkov::Queue<int> queue;

  queue.push(1);
  queue.push(2);

  BOOST_TEST(queue.front() == 1);
}

BOOST_AUTO_TEST_CASE(fifo_test) {
  karpenkov::Queue<int> queue;

  queue.push(10);
  queue.push(20);
  queue.push(30);

  queue.pop();

  BOOST_TEST(queue.front() == 20);
}

BOOST_AUTO_TEST_CASE(empty_test) {
  karpenkov::Queue<int> queue;

  BOOST_TEST(queue.empty());

  queue.push(1);

  BOOST_TEST(!queue.empty());
}

BOOST_AUTO_TEST_CASE(front_empty_test) {
  karpenkov::Queue<int> queue;

  BOOST_CHECK_THROW(queue.front(), std::out_of_range);
}
