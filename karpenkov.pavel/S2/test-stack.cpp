#include <boost/test/unit_test.hpp>

#include "stack.hpp"

BOOST_AUTO_TEST_CASE(push_and_top_test) {
  karpenkov::Stack<int> stack;

  stack.push(10);
  stack.push(20);

  BOOST_TEST(stack.top() == 20);
}

BOOST_AUTO_TEST_CASE(pop_test) {
  karpenkov::Stack<int> stack;

  stack.push(1);
  stack.push(2);

  stack.pop();

  BOOST_TEST(stack.top() == 1);
}

BOOST_AUTO_TEST_CASE(emptyStack_test) {
  karpenkov::Stack<int> stack;

  BOOST_TEST(stack.empty());

  stack.push(5);

  BOOST_TEST(!stack.empty());
}

BOOST_AUTO_TEST_CASE(top_empty_test) {
  karpenkov::Stack<int> stack;

  BOOST_CHECK_THROW(stack.top(), std::out_of_range);
}
