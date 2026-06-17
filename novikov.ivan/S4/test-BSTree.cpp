#define BOOST_TEST_MODULE S4
#include <functional>
#include <stdexcept>
#include <boost/test/included/unit_test.hpp>
#include "BSTree.hpp"

BOOST_AUTO_TEST_CASE(push)
{
  novikov::BSTree< std::string, size_t, std::less< std::string > > tree;
  tree.push("a", 1);
  BOOST_TEST(tree["a"] == 1);
}

BOOST_AUTO_TEST_CASE(get)
{
  novikov::BSTree< std::string, size_t, std::less< std::string > > tree;
  BOOST_CHECK_THROW(tree.get("a"), std::out_of_range);
  tree.push("a", 1);
  BOOST_TEST(tree.get("a") == 1);
}

BOOST_AUTO_TEST_CASE(drop)
{
  novikov::BSTree< std::string, size_t, std::less< std::string > > tree;
  tree.push("a", 1);
  tree.drop("a");
  BOOST_CHECK_THROW(tree.get("a"), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(has)
{
  novikov::BSTree< std::string, size_t, std::less< std::string > > tree;
  BOOST_TEST(tree.has("a") == false);
  tree.push("a", 1);
  BOOST_TEST(tree.has("a") == true);
}

BOOST_AUTO_TEST_CASE(square_brackets_modify)
{
  novikov::BSTree< std::string, size_t, std::less< std::string > > tree;
  tree.push("a", 1);
  tree["a"] = 5;
  BOOST_TEST(tree["a"] == 5);
  BOOST_TEST(tree.get("a") == 5);
}

BOOST_AUTO_TEST_CASE(copy_constructor)
{
  novikov::BSTree< std::string, size_t, std::less< std::string > > tree1;
  tree1.push("a", 1);
  tree1.push("b", 2);

  novikov::BSTree< std::string, size_t, std::less< std::string > > tree2{tree1};
  BOOST_TEST(tree2.height() == tree1.height());
  BOOST_TEST(tree2.get("b") == 2);
  BOOST_TEST(tree2.has("a") == true);
}

BOOST_AUTO_TEST_CASE(copy_assignment)
{
  novikov::BSTree< std::string, size_t, std::less< std::string > > tree1;
  tree1.push("a", 1);
  tree1.push("b", 2);

  novikov::BSTree< std::string, size_t, std::less< std::string > > tree2;
  tree2 = tree1;
  BOOST_TEST(tree2.get("a") == 1);
}

BOOST_AUTO_TEST_CASE(move_constructor)
{
  novikov::BSTree< std::string, size_t, std::less< std::string > > tree1;
  tree1.push("a", 1);

  novikov::BSTree< std::string, size_t, std::less< std::string > > tree2{std::move(tree1)};
  BOOST_TEST(tree2.get("a") == 1);
}

BOOST_AUTO_TEST_CASE(move_assignment)
{
  novikov::BSTree< std::string, size_t, std::less< std::string > > tree1;
  tree1.push("a", 1);

  novikov::BSTree< std::string, size_t, std::less< std::string > > tree2;
  tree2 = std::move(tree1);
  BOOST_TEST(tree2.get("a") == 1);
}

BOOST_AUTO_TEST_CASE(setName_and_getName)
{
  novikov::BSTree< std::string, size_t, std::less< std::string > > tree;
  tree.setName("tree");
  BOOST_TEST(tree.getName() == "tree");
}

BOOST_AUTO_TEST_CASE(height)
{
  novikov::BSTree< std::string, size_t, std::less< std::string > > tree;
  novikov::BSTConstIterator< std::string, size_t > it = tree.cbegin();
  BOOST_TEST(tree.height() == 0);
  BOOST_TEST(tree.height(it) == 0);
  tree.push("a", 1);
  it = tree.cbegin();
  BOOST_TEST(tree.height() == 1);
  BOOST_TEST(tree.height(it) == 1);
}

BOOST_AUTO_TEST_CASE(begin_end)
{
  novikov::BSTree< std::string, size_t, std::less< std::string > > tree;
  BOOST_CHECK(tree.begin() == tree.end());
  tree.push("a", 1);
  tree.push("b", 2);
  tree.push("c", 3);
  auto it = tree.begin();
  BOOST_CHECK_EQUAL((*it).first, "a");
  BOOST_CHECK_EQUAL((*it).second, 1);
  ++it;
  BOOST_CHECK_EQUAL((*it).first, "b");
  BOOST_CHECK(it != tree.end());
}

BOOST_AUTO_TEST_CASE(rotate_left)
{
  novikov::BSTree< size_t, std::string, std::less< size_t > > tree;
  tree.push(1, "a");
  tree.push(2, "b");
  tree.push(3, "c");
  novikov::BSTIterator< size_t, std::string > it = tree.begin();
  ++it;
  tree.rotateLeft(it);
  BOOST_CHECK_EQUAL(tree.height(), 2);
}

BOOST_AUTO_TEST_CASE(rotate_right)
{
  novikov::BSTree< size_t, std::string, std::less< size_t > > tree;
  tree.push(3, "a");
  tree.push(2, "b");
  tree.push(1, "c");
  novikov::BSTIterator< size_t, std::string > it = tree.begin();
  ++it;
  tree.rotateRight(it);
  BOOST_CHECK_EQUAL(tree.height(), 2);
}

BOOST_AUTO_TEST_CASE(rotate_large_left)
{
  novikov::BSTree< size_t, std::string, std::less< size_t > > tree;
  tree.push(1, "a");
  tree.push(3, "b");
  tree.push(2, "c");
  novikov::BSTIterator< size_t, std::string > it = tree.begin();
  ++it;
  tree.rotateLargeLeft(it);
  BOOST_CHECK_EQUAL(tree.height(), 2);
}

BOOST_AUTO_TEST_CASE(rotate_large_right)
{
  novikov::BSTree< size_t, std::string, std::less< size_t > > tree;
  tree.push(3, "a");
  tree.push(1, "b");
  tree.push(2, "c");
  novikov::BSTIterator< size_t, std::string > it = tree.begin();
  ++it;
  tree.rotateLargeRight(it);
  BOOST_CHECK_EQUAL(tree.height(), 2);
}
