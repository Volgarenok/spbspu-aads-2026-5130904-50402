#include <iostream>
#include <vector>
#include <string>
#include "BiTree/BiTree.hpp"
#include <boost/test/unit_test.hpp>

using namespace afanasev;

BOOST_AUTO_TEST_SUITE(BSTreeTests)

BOOST_AUTO_TEST_CASE(test_default_constructor)
{
  std::cout << "test_default_constructor\n";
  BSTree<int, std::string> tree;
  BOOST_CHECK_EQUAL(tree.size(), 0);
  BOOST_CHECK(tree.begin() == tree.end());
}

BOOST_AUTO_TEST_CASE(test_push_and_size)
{
  std::cout << "test_push_and_size\n";
  BSTree<int, std::string> tree;
  tree.push(1, "one");
  BOOST_CHECK_EQUAL(tree.size(), 1);
  tree.push(2, "two");
  tree.push(3, "three");
  BOOST_CHECK_EQUAL(tree.size(), 3);
}

BOOST_AUTO_TEST_CASE(test_get_const)
{
  std::cout << "test_get_const\n";
  BSTree<int, std::string> tree;
  tree.push(1, "one");
  tree.push(2, "two");
  const BSTree<int, std::string>& ctree = tree;
  BOOST_CHECK_EQUAL(ctree.get(1), "one");
  BOOST_CHECK_EQUAL(ctree.get(2), "two");
}

BOOST_AUTO_TEST_CASE(test_get_non_const)
{
  std::cout << "test_get_non_const\n";
  BSTree<int, std::string> tree;
  tree.push(1, "one");
  tree.get(1) = "ONE";
  BOOST_CHECK_EQUAL(tree.get(1), "ONE");
}

BOOST_AUTO_TEST_CASE(test_get_not_found)
{
  std::cout << "test_get_not_found\n";
  BSTree<int, std::string> tree;
  tree.push(1, "one");
  BOOST_CHECK_THROW(tree.get(2), std::out_of_range);
  const BSTree<int, std::string>& ctree = tree;
  BOOST_CHECK_THROW(ctree.get(2), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(test_push_update_existing)
{
  std::cout << "test_push_update_existing\n";
  BSTree<int, std::string> tree;
  tree.push(1, "one");
  tree.push(1, "ONE");
  BOOST_CHECK_EQUAL(tree.size(), 1);
  BOOST_CHECK_EQUAL(tree.get(1), "ONE");
}

BOOST_AUTO_TEST_CASE(test_drop)
{
  std::cout << "test_drop\n";
  BSTree<int, std::string> tree;
  tree.push(1, "one");
  tree.push(2, "two");
  tree.push(3, "three");
  std::string val = tree.drop(2);
  BOOST_CHECK_EQUAL(val, "two");
  BOOST_CHECK_EQUAL(tree.size(), 2);
  BOOST_CHECK_EQUAL(tree.get(1), "one");
  BOOST_CHECK_EQUAL(tree.get(3), "three");
  BOOST_CHECK_THROW(tree.get(2), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(test_drop_root_with_two_children)
{
  std::cout << "test_drop_root_with_two_children\n";
  BSTree<int, std::string> tree;
  tree.push(2, "two");
  tree.push(1, "one");
  tree.push(3, "three");
  tree.drop(2);
  BOOST_CHECK_EQUAL(tree.size(), 2);
  BOOST_CHECK((tree.get(1) == "one" && tree.get(3) == "three") ||
    (tree.get(3) == "three" && tree.get(1) == "one"));
}

BOOST_AUTO_TEST_CASE(test_clear)
{
  std::cout << "test_clear\n";
  BSTree<int, std::string> tree;
  tree.push(1, "one");
  tree.push(2, "two");
  tree.push(3, "three");
  tree.clear();
  BOOST_CHECK_EQUAL(tree.size(), 0);
  BOOST_CHECK_THROW(tree.get(1), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(test_copy_constructor)
{
  std::cout << "test_copy_constructor\n";
  BSTree<int, std::string> tree1;
  tree1.push(1, "one");
  tree1.push(2, "two");
  tree1.push(3, "three");
  BSTree<int, std::string> tree2(tree1);
  BOOST_CHECK_EQUAL(tree2.size(), 3);
  BOOST_CHECK_EQUAL(tree2.get(1), "one");
  BOOST_CHECK_EQUAL(tree2.get(2), "two");
  BOOST_CHECK_EQUAL(tree2.get(3), "three");
  tree2.get(1) = "ONE";
  BOOST_CHECK_EQUAL(tree1.get(1), "one");
  BOOST_CHECK_EQUAL(tree2.get(1), "ONE");
}

BOOST_AUTO_TEST_CASE(test_copy_assignment)
{
  std::cout << "test_copy_assignment\n";
  BSTree<int, std::string> tree1;
  tree1.push(1, "one");
  BSTree<int, std::string> tree2;
  tree2.push(99, "ninety-nine");
  tree2 = tree1;
  BOOST_CHECK_EQUAL(tree2.size(), 1);
  BOOST_CHECK_EQUAL(tree2.get(1), "one");
  BOOST_CHECK_THROW(tree2.get(99), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(test_move_constructor)
{
  std::cout << "test_move_constructor\n";
  BSTree<int, std::string> tree1;
  tree1.push(1, "one");
  tree1.push(2, "two");
  BSTree<int, std::string> tree2(std::move(tree1));
  BOOST_CHECK_EQUAL(tree2.size(), 2);
  BOOST_CHECK_EQUAL(tree2.get(1), "one");
  BOOST_CHECK_EQUAL(tree2.get(2), "two");
  BOOST_CHECK_EQUAL(tree1.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_move_assignment)
{
  std::cout << "test_move_assignment\n";
  BSTree<int, std::string> tree1;
  tree1.push(1, "one");
  BSTree<int, std::string> tree2;
  tree2.push(99, "ninety-nine");
  tree2 = std::move(tree1);
  BOOST_CHECK_EQUAL(tree2.size(), 1);
  BOOST_CHECK_EQUAL(tree2.get(1), "one");
  BOOST_CHECK_EQUAL(tree1.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_swap)
{
  std::cout << "test_swap\n";
  BSTree<int, std::string> tree1;
  tree1.push(1, "one");
  BSTree<int, std::string> tree2;
  tree2.push(2, "two");
  tree2.push(3, "three");
  tree1.swap(tree2);
  BOOST_CHECK_EQUAL(tree1.size(), 2);
  BOOST_CHECK_EQUAL(tree1.get(2), "two");
  BOOST_CHECK_EQUAL(tree2.size(), 1);
  BOOST_CHECK_EQUAL(tree2.get(1), "one");
}

BOOST_AUTO_TEST_CASE(test_iterator_traversal)
{
  std::cout << "test_iterator_traversal\n";
  BSTree<int, std::string> tree;
  tree.push(3, "three");
  tree.push(1, "one");
  tree.push(2, "two");
  tree.push(4, "four");
  std::vector<std::pair<const int, std::string>> result;
  for (auto it = tree.begin(); it != tree.end(); ++it)
  {
    result.push_back(*it);
  }
  BOOST_REQUIRE_EQUAL(result.size(), 4);
  BOOST_CHECK_EQUAL(result[0].first, 1);
  BOOST_CHECK_EQUAL(result[1].first, 2);
  BOOST_CHECK_EQUAL(result[2].first, 3);
  BOOST_CHECK_EQUAL(result[3].first, 4);
}

BOOST_AUTO_TEST_CASE(test_const_iterator)
{
  std::cout << "test_const_iterator\n";
  const BSTree<int, std::string> tree;
  BOOST_CHECK(tree.begin() == tree.end());
}

BOOST_AUTO_TEST_CASE(test_iterator_modification)
{
  std::cout << "test_iterator_modification\n";
  BSTree<int, std::string> tree;
  tree.push(1, "one");
  tree.push(2, "two");
  for (auto it = tree.begin(); it != tree.end(); ++it)
  {
    std::pair<const int, std::string> pair = *it;
    if (pair.first == 1)
    {
      tree.get(1) = "ONE";
    }
  }
  BOOST_CHECK_EQUAL(tree.get(1), "ONE");
}

BOOST_AUTO_TEST_CASE(test_height_empty)
{
  std::cout << "test_height_empty\n";
  BSTree<int, std::string> tree;
  BOOST_CHECK_EQUAL(tree.height(), 0);
}

BOOST_AUTO_TEST_CASE(test_height_single)
{
  std::cout << "test_height_single\n";
  BSTree<int, std::string> tree;
  tree.push(1, "one");
  BOOST_CHECK_EQUAL(tree.height(), 1);
}

BOOST_AUTO_TEST_CASE(test_height_balanced)
{
  std::cout << "test_height_balanced\n";
  BSTree<int, std::string> tree;
  tree.push(2, "two");
  tree.push(1, "one");
  tree.push(3, "three");
  BOOST_CHECK_EQUAL(tree.height(), 2);
}

BOOST_AUTO_TEST_CASE(test_height_degenerate)
{
  std::cout << "test_height_degenerate\n";
  BSTree<int, std::string> tree;
  tree.push(1, "one");
  tree.push(2, "two");
  tree.push(3, "three");
  tree.push(4, "four");
  BOOST_CHECK_EQUAL(tree.height(), 4);
}

BOOST_AUTO_TEST_CASE(test_height_iterator)
{
  std::cout << "test_height_iterator\n";
  BSTree<int, std::string> tree;
  tree.push(2, "two");
  tree.push(1, "one");
  tree.push(3, "three");
  const auto& ctree = tree;
  auto it = ctree.begin();
  ++it;
  BOOST_CHECK_EQUAL(tree.height(it), 2);
}

BOOST_AUTO_TEST_CASE(test_rotate_left)
{
  std::cout << "test_rotate_left\n";
  BSTree<int, std::string> tree;
  tree.push(1, "one");
  tree.push(2, "two");
  const auto& ctree = tree;
  auto it = ctree.begin();
  ++it;
  tree.rotateLeft(it);
  BOOST_CHECK_EQUAL(tree.size(), 2);
  BOOST_CHECK_EQUAL(tree.get(1), "one");
  BOOST_CHECK_EQUAL(tree.get(2), "two");
}

BOOST_AUTO_TEST_CASE(test_rotate_right)
{
  std::cout << "test_rotate_right\n";
  BSTree<int, std::string> tree;
  tree.push(2, "two");
  tree.push(1, "one");
  const auto& ctree = tree;
  auto it = ctree.begin();
  ++it;
  tree.rotateRight(it);
  BOOST_CHECK_EQUAL(tree.size(), 2);
  BOOST_CHECK_EQUAL(tree.get(1), "one");
  BOOST_CHECK_EQUAL(tree.get(2), "two");
}

BOOST_AUTO_TEST_CASE(test_rotate_large_left)
{
  std::cout << "test_rotate_large_left\n";
  BSTree<int, std::string> tree;
  tree.push(3, "three");
  tree.push(1, "one");
  tree.push(2, "two");
  const auto& ctree = tree;
  auto it = ctree.begin();
  ++it;
  ++it;
  tree.rotateLargeLeft(it);
  BOOST_CHECK_EQUAL(tree.size(), 3);
  BOOST_CHECK_EQUAL(tree.get(1), "one");
  BOOST_CHECK_EQUAL(tree.get(2), "two");
  BOOST_CHECK_EQUAL(tree.get(3), "three");
}

BOOST_AUTO_TEST_CASE(test_rotate_large_right)
{
  std::cout << "test_rotate_large_right\n";
  BSTree<int, std::string> tree;
  tree.push(1, "one");
  tree.push(3, "three");
  tree.push(2, "two");
  const auto& ctree = tree;
  auto it = ctree.begin();
  tree.rotateLargeRight(it);
  BOOST_CHECK_EQUAL(tree.size(), 3);
  BOOST_CHECK_EQUAL(tree.get(1), "one");
  BOOST_CHECK_EQUAL(tree.get(2), "two");
  BOOST_CHECK_EQUAL(tree.get(3), "three");
}

BOOST_AUTO_TEST_CASE(test_push_rvalue)
{
  std::cout << "test_push_rvalue\n";
  BSTree<int, std::string> tree;
  std::string val = "rvalue";
  tree.push(1, std::move(val));
  BOOST_CHECK_EQUAL(tree.get(1), "rvalue");
}

BOOST_AUTO_TEST_CASE(test_empty_tree_operations)
{
  std::cout << "test_empty_tree_operations\n";
  BSTree<int, std::string> tree;
  BOOST_CHECK(tree.begin() == tree.end());
  BOOST_CHECK_EQUAL(tree.height(), 0);
}

BOOST_AUTO_TEST_CASE(test_iterator_equality)
{
  std::cout << "test_iterator_equality\n";
  BSTree<int, std::string> tree;
  tree.push(1, "one");
  auto it1 = tree.begin();
  auto it2 = tree.begin();
  auto it3 = tree.end();
  BOOST_CHECK(it1 == it2);
  BOOST_CHECK(it1 != it3);
  BOOST_CHECK(tree.begin() != tree.end());
}

BOOST_AUTO_TEST_SUITE_END()
