#include <stdexcept>
#define BOOST_TEST_MODULE StackQueueTest
#include <boost/test/included/unit_test.hpp>
#include "BSTree.hpp"
#include <functional>

BOOST_AUTO_TEST_CASE(push_test)
{
  lavrentev::BSTree<std::string, size_t, std::less<std::string>> bst{};
  bst.push("a", 1);
  BOOST_TEST(bst["a"] == 1);
  BOOST_CHECK_THROW(bst.push("a", 1), std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(get_test)
{
  lavrentev::BSTree<std::string, size_t, std::less<std::string>> bst{};
  BOOST_CHECK_THROW(bst.get("a"), std::out_of_range);
  bst.push("a", 1);
  BOOST_TEST(bst.get("a") == 1);
}

BOOST_AUTO_TEST_CASE(drop_test)
{
  lavrentev::BSTree<std::string, size_t, std::less<std::string>> bst{};
  BOOST_CHECK_THROW(bst.drop("a"), std::out_of_range);
  bst.push("a", 1);
  bst.drop("a");
  BOOST_CHECK_THROW(bst.get("a"), std::out_of_range);
}
