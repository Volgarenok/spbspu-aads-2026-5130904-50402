#include <stdexcept>
#define BOOST_TEST_MODULE StackQueueTest
#include <boost/test/included/unit_test.hpp>
#include "BSTree.hpp"
#include <functional>
#include <sstream>

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

BOOST_AUTO_TEST_CASE(has_test)
{
  lavrentev::BSTree<std::string, size_t, std::less<std::string>> bst{};
  BOOST_TEST(bst.has("a") == false);
  bst.push("a", 1);
  BOOST_TEST(bst.has("a") == true);
}

BOOST_AUTO_TEST_CASE(setGetName_test)
{
  lavrentev::BSTree<std::string, size_t, std::less<std::string>> bst{};
  bst.setName("graph");
  BOOST_TEST(bst.getName() == "graph");
}

BOOST_AUTO_TEST_CASE(height_test)
{
  lavrentev::BSTree<std::string, size_t, std::less<std::string>> bst{};
  lavrentev::BSTConstIterator< std::string, size_t > it = bst.cbegin();
  BOOST_TEST(bst.height() == 0);
  BOOST_TEST(bst.height(it) == 0);
  bst.push("a", 1);
  BOOST_TEST(bst.height() == 1);
  BOOST_TEST(bst.height(it) == 1);
}

BOOST_AUTO_TEST_CASE(print_test)
{
  lavrentev::BSTree<size_t, std::string, std::less<size_t>> bst{};
  bst.setName("graph");
  bst.push(1, "a");
  bst.push(2, "b");
  bst.push(3, "c");

  BSTList bstl;
  bstl.pushFront(std::move(bst));
  std::istringstream in("graph");
  std::ostringstream out;
  lavrentev::print(in, out, bstl);
  BOOST_CHECK_EQUAL(out.str(), "graph 1 a 2 b 3 c\n");
  out.clear();

  lavrentev::BSTree<size_t, std::string, std::less<size_t>> bst2{};
  bst2.setName("graph2");
  bstl.pushFront(std::move(bst2));
  in.str("graph2");
  lavrentev::print(in, out, bstl);
  BOOST_CHECK_EQUAL(out.str(), "<EMPTY>>\n");
}
