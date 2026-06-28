#include <boost/test/unit_test.hpp>
#include <stdexcept>
#include <string>
#include <vector>
#include <utility>
#include "bstree.hpp"

using petrov::BSTree;

BOOST_AUTO_TEST_SUITE(BSTreeTests)

BOOST_AUTO_TEST_CASE(test_default_constructor)
{
  BSTree< int, std::string > tree;
  BOOST_CHECK(tree.empty());
  BOOST_CHECK(tree.begin() == tree.end());
}

BOOST_AUTO_TEST_CASE(test_push_basic)
{
  BSTree< int, std::string > tree;
  tree.push(1, "one");
  BOOST_CHECK(!tree.empty());
  BOOST_CHECK(tree.has(1));
}

BOOST_AUTO_TEST_CASE(test_get_const)
{
  BSTree< int, std::string > tree;
  tree.push(1, "one");
  tree.push(2, "two");

  BOOST_CHECK_EQUAL(tree.get(1), "one");
  BOOST_CHECK_EQUAL(tree.get(2), "two");
}

BOOST_AUTO_TEST_CASE(test_get_non_const)
{
  BSTree< int, std::string > tree;
  tree.push(1, "one");
  tree.get(1) = "ONE";
  BOOST_CHECK_EQUAL(tree.get(1), "ONE");
}

BOOST_AUTO_TEST_CASE(test_get_not_found)
{
  BSTree< int, std::string > tree;
  tree.push(1, "one");

  BOOST_CHECK_THROW(tree.get(2), std::out_of_range);

  const BSTree< int, std::string > & ctree = tree;
  BOOST_CHECK_THROW(ctree.get(2), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(test_push_update_existing)
{
  BSTree< int, std::string > tree;
  tree.push(1, "one");
  tree.push(1, "ONE");

  BOOST_CHECK_EQUAL(tree.get(1), "ONE");
}

BOOST_AUTO_TEST_CASE(test_drop_leaf)
{
  BSTree< int, std::string > tree;
  tree.push(1, "one");
  tree.push(2, "two");
  tree.push(3, "three");

  std::string val = tree.drop(3);
  BOOST_CHECK_EQUAL(val, "three");
  BOOST_CHECK(!tree.has(3));
  BOOST_CHECK(tree.has(1));
  BOOST_CHECK(tree.has(2));
}

BOOST_AUTO_TEST_CASE(test_drop_middle)
{
  BSTree< int, std::string > tree;
  tree.push(1, "one");
  tree.push(2, "two");
  tree.push(3, "three");

  std::string val = tree.drop(2);
  BOOST_CHECK_EQUAL(val, "two");
  BOOST_CHECK(!tree.has(2));
  BOOST_CHECK(tree.has(1));
  BOOST_CHECK(tree.has(3));
}

BOOST_AUTO_TEST_CASE(test_drop_root_with_two_children)
{
  BSTree< int, std::string > tree;
  tree.push(2, "two");
  tree.push(1, "one");
  tree.push(3, "three");

  tree.drop(2);
  BOOST_CHECK(!tree.has(2));
  BOOST_CHECK(tree.has(1));
  BOOST_CHECK(tree.has(3));
}

BOOST_AUTO_TEST_CASE(test_copy_constructor)
{
  BSTree< int, std::string > tree1;
  tree1.push(1, "one");
  tree1.push(2, "two");
  tree1.push(3, "three");

  BSTree< int, std::string > tree2(tree1);

  BOOST_CHECK_EQUAL(tree2.get(1), "one");
  BOOST_CHECK_EQUAL(tree2.get(2), "two");
  BOOST_CHECK_EQUAL(tree2.get(3), "three");

  tree2.get(1) = "ONE";
  BOOST_CHECK_EQUAL(tree1.get(1), "one");
  BOOST_CHECK_EQUAL(tree2.get(1), "ONE");
}

BOOST_AUTO_TEST_CASE(test_copy_assignment)
{
  BSTree< int, std::string > tree1;
  tree1.push(1, "one");

  BSTree< int, std::string > tree2;
  tree2.push(99, "ninety-nine");

  tree2 = tree1;

  BOOST_CHECK_EQUAL(tree2.get(1), "one");
  BOOST_CHECK(!tree2.has(99));
}

BOOST_AUTO_TEST_CASE(test_move_constructor)
{
  BSTree< int, std::string > tree1;
  tree1.push(1, "one");
  tree1.push(2, "two");

  BSTree< int, std::string > tree2(std::move(tree1));

  BOOST_CHECK_EQUAL(tree2.get(1), "one");
  BOOST_CHECK_EQUAL(tree2.get(2), "two");
  BOOST_CHECK(tree1.empty());
}

BOOST_AUTO_TEST_CASE(test_move_assignment)
{
  BSTree< int, std::string > tree1;
  tree1.push(1, "one");

  BSTree< int, std::string > tree2;
  tree2.push(99, "ninety-nine");

  tree2 = std::move(tree1);

  BOOST_CHECK_EQUAL(tree2.get(1), "one");
  BOOST_CHECK(tree1.empty());
}

BOOST_AUTO_TEST_CASE(test_iterator_traversal_sorted)
{
  BSTree< int, std::string > tree;
  tree.push(3, "three");
  tree.push(1, "one");
  tree.push(2, "two");
  tree.push(4, "four");

  std::vector< int > keys;
  for (auto it = tree.begin(); it != tree.end(); ++it)
  {
    keys.push_back(it->first);
  }

  BOOST_REQUIRE_EQUAL(keys.size(), 4u);
  BOOST_CHECK_EQUAL(keys[0], 1);
  BOOST_CHECK_EQUAL(keys[1], 2);
  BOOST_CHECK_EQUAL(keys[2], 3);
  BOOST_CHECK_EQUAL(keys[3], 4);
}

BOOST_AUTO_TEST_CASE(test_const_iterator_empty)
{
  const BSTree< int, std::string > tree;
  BOOST_CHECK(tree.begin() == tree.end());
}

BOOST_AUTO_TEST_CASE(test_height_empty)
{
  BSTree< int, std::string > tree;
  BOOST_CHECK_EQUAL(tree.height(), 0u);
}

BOOST_AUTO_TEST_CASE(test_height_single)
{
  BSTree< int, std::string > tree;
  tree.push(1, "one");
  BOOST_CHECK_EQUAL(tree.height(), 1u);
}

BOOST_AUTO_TEST_CASE(test_height_balanced)
{
  BSTree< int, std::string > tree;
  tree.push(2, "two");
  tree.push(1, "one");
  tree.push(3, "three");
  BOOST_CHECK_EQUAL(tree.height(), 2u);
}

BOOST_AUTO_TEST_CASE(test_height_degenerate)
{
  BSTree< int, std::string > tree;
  tree.push(1, "one");
  tree.push(2, "two");
  tree.push(3, "three");
  tree.push(4, "four");
  BOOST_CHECK_EQUAL(tree.height(), 4u);
}

BOOST_AUTO_TEST_CASE(test_rotate_left)
{
  BSTree< int, std::string > tree;
  tree.push(1, "one");
  tree.push(2, "two");

  auto it = tree.begin();
  tree.rotateLeft(it);

  BOOST_CHECK(tree.has(1));
  BOOST_CHECK(tree.has(2));
}

BOOST_AUTO_TEST_CASE(test_rotate_right)
{
  BSTree< int, std::string > tree;
  tree.push(2, "two");
  tree.push(1, "one");

  auto it = tree.begin();
  ++it;
  tree.rotateRight(it);

  BOOST_CHECK(tree.has(1));
  BOOST_CHECK(tree.has(2));
}

BOOST_AUTO_TEST_CASE(test_rotate_large_left)
{
  BSTree< int, std::string > tree;
  tree.push(3, "three");
  tree.push(1, "one");
  tree.push(2, "two");

  auto it = tree.begin();
  tree.rotateLargeLeft(it);

  BOOST_CHECK(tree.has(1));
  BOOST_CHECK(tree.has(2));
  BOOST_CHECK(tree.has(3));
}

BOOST_AUTO_TEST_CASE(test_rotate_large_right)
{
  BSTree< int, std::string > tree;
  tree.push(1, "one");
  tree.push(3, "three");
  tree.push(2, "two");

  auto it = tree.begin();
  tree.rotateLargeRight(it);

  BOOST_CHECK(tree.has(1));
  BOOST_CHECK(tree.has(2));
  BOOST_CHECK(tree.has(3));
}

BOOST_AUTO_TEST_CASE(test_iterator_equality)
{
  BSTree< int, std::string > tree;
  tree.push(1, "one");

  auto it1 = tree.begin();
  auto it2 = tree.begin();
  auto it3 = tree.end();

  BOOST_CHECK(it1 == it2);
  BOOST_CHECK(it1 != it3);
  BOOST_CHECK(tree.begin() != tree.end());
}

BOOST_AUTO_TEST_SUITE_END()

