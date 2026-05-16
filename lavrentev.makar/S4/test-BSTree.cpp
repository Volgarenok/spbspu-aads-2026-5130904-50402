#include <stdexcept>
#define BOOST_TEST_MODULE StackQueueTest
#include <boost/test/included/unit_test.hpp>
#include "BSTree.hpp"
#include <functional>

BOOST_AUTO_TEST_CASE(push_test)
{
  lavrentev::BSTree<std::string, size_t, std::less<size_t>> bst{};
  bst.push("a", 1);
  BOOST_TEST(bst["a"] == 1);
  BOOST_CHECK_THROW(bst.push("a", 1), std::out_of_range);
}
