#include <boost/test/unit_test.hpp>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>
#include "tree.hpp"

namespace {
  using Tree = samarin::BSTree< int, std::string >;
}

BOOST_AUTO_TEST_SUITE(tree_tests)

BOOST_AUTO_TEST_CASE(push_and_get)
{
  Tree tree;
  tree.push(2, "two");
  tree.push(1, "one");
  tree.push(3, "three");
  BOOST_TEST(tree.size() == 3u);
  BOOST_TEST(tree.get(1) == "one");
  BOOST_TEST(tree.get(2) == "two");
  BOOST_TEST(tree.get(3) == "three");
}

BOOST_AUTO_TEST_CASE(push_overwrites_value)
{
  Tree tree;
  tree.push(1, "old");
  tree.push(1, "new");
  BOOST_TEST(tree.size() == 1u);
  BOOST_TEST(tree.get(1) == "new");
}

BOOST_AUTO_TEST_CASE(get_missing_throws)
{
  Tree tree;
  BOOST_CHECK_THROW(tree.get(7), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(iteration_is_sorted)
{
  Tree tree;
  tree.push(5, "e");
  tree.push(1, "a");
  tree.push(4, "d");
  tree.push(2, "b");
  tree.push(3, "c");
  std::string keys;
  for (Tree::const_iterator it = tree.cbegin(); it != tree.cend(); ++it) {
    keys += it->second;
  }
  BOOST_TEST(keys == "abcde");
}

BOOST_AUTO_TEST_CASE(contains_reports_membership)
{
  Tree tree;
  tree.push(10, "x");
  BOOST_TEST(tree.contains(10));
  BOOST_TEST(!tree.contains(11));
}

BOOST_AUTO_TEST_CASE(drop_returns_value_and_keeps_order)
{
  Tree tree;
  tree.push(5, "e");
  tree.push(3, "c");
  tree.push(8, "h");
  tree.push(1, "a");
  tree.push(4, "d");
  BOOST_TEST(tree.drop(3) == "c");
  BOOST_TEST(!tree.contains(3));
  BOOST_TEST(tree.size() == 4u);
  std::string keys;
  for (Tree::const_iterator it = tree.cbegin(); it != tree.cend(); ++it) {
    keys += it->second;
  }
  BOOST_TEST(keys == "adeh");
}

BOOST_AUTO_TEST_CASE(drop_missing_throws)
{
  Tree tree;
  tree.push(1, "a");
  BOOST_CHECK_THROW(tree.drop(2), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(drop_leaf_and_single_child)
{
  Tree tree;
  tree.push(5, "e");
  tree.push(3, "c");
  tree.push(8, "h");
  tree.push(2, "b");
  tree.drop(2);
  tree.drop(3);
  std::string keys;
  for (Tree::const_iterator it = tree.cbegin(); it != tree.cend(); ++it) {
    keys += it->second;
  }
  BOOST_TEST(keys == "eh");
  BOOST_TEST(tree.size() == 2u);
}

BOOST_AUTO_TEST_CASE(drop_root_with_two_children)
{
  Tree tree;
  tree.push(5, "e");
  tree.push(3, "c");
  tree.push(8, "h");
  BOOST_TEST(tree.drop(5) == "e");
  std::string keys;
  for (Tree::const_iterator it = tree.cbegin(); it != tree.cend(); ++it) {
    keys += it->second;
  }
  BOOST_TEST(keys == "ch");
}

BOOST_AUTO_TEST_CASE(copy_is_independent)
{
  Tree tree;
  tree.push(1, "a");
  tree.push(2, "b");
  Tree copy(tree);
  copy.push(3, "c");
  BOOST_TEST(tree.size() == 2u);
  BOOST_TEST(copy.size() == 3u);
  BOOST_TEST(copy.get(1) == "a");
}

BOOST_AUTO_TEST_CASE(move_transfers_contents)
{
  Tree tree;
  tree.push(1, "a");
  tree.push(2, "b");
  Tree moved(std::move(tree));
  BOOST_TEST(moved.size() == 2u);
  BOOST_TEST(moved.get(2) == "b");
}

BOOST_AUTO_TEST_CASE(copy_assignment_replaces_contents)
{
  Tree first;
  first.push(1, "a");
  Tree second;
  second.push(7, "g");
  second.push(8, "h");
  first = second;
  BOOST_TEST(first.size() == 2u);
  BOOST_TEST(first.get(7) == "g");
  BOOST_TEST(!first.contains(1));
}

BOOST_AUTO_TEST_CASE(clear_empties_tree)
{
  Tree tree;
  tree.push(1, "a");
  tree.push(2, "b");
  tree.clear();
  BOOST_TEST(tree.empty());
  BOOST_TEST(tree.size() == 0u);
  BOOST_TEST((tree.begin() == tree.end()));
}

BOOST_AUTO_TEST_CASE(height_measures_subtree)
{
  Tree tree;
  tree.push(5, "e");
  tree.push(3, "c");
  tree.push(8, "h");
  tree.push(1, "a");
  BOOST_TEST(tree.height() == 3u);
  BOOST_TEST(tree.height(tree.find(8)) == 1u);
  BOOST_TEST(tree.height(tree.find(3)) == 2u);
  BOOST_TEST(tree.height(tree.end()) == 0u);
}

BOOST_AUTO_TEST_SUITE_END()

namespace {
  std::string inorder(const Tree & tree)
  {
    std::string keys;
    for (Tree::const_iterator it = tree.cbegin(); it != tree.cend(); ++it) {
      keys += it->second;
    }
    return keys;
  }
}

BOOST_AUTO_TEST_SUITE(rotation_tests)

BOOST_AUTO_TEST_CASE(rotate_right_keeps_order_and_refs)
{
  Tree tree;
  tree.push(5, "e");
  tree.push(3, "c");
  tree.push(8, "h");
  tree.push(1, "a");
  tree.push(4, "d");
  const std::string * before = std::addressof(tree.find(5)->second);
  Tree::const_iterator risen = tree.rotateRight(tree.find(3));
  BOOST_TEST(risen->first == 3);
  BOOST_TEST(inorder(tree) == "acdeh");
  BOOST_TEST(std::addressof(tree.find(5)->second) == before);
  BOOST_TEST(tree.height() == 3u);
}

BOOST_AUTO_TEST_CASE(rotate_left_keeps_order)
{
  Tree tree;
  tree.push(5, "e");
  tree.push(8, "h");
  tree.push(3, "c");
  tree.push(6, "f");
  tree.push(9, "i");
  Tree::const_iterator risen = tree.rotateLeft(tree.find(8));
  BOOST_TEST(risen->first == 8);
  BOOST_TEST(inorder(tree) == "cefhi");
}

BOOST_AUTO_TEST_CASE(rotate_large_right_lifts_grandchild)
{
  Tree tree;
  tree.push(8, "h");
  tree.push(3, "c");
  tree.push(5, "e");
  Tree::const_iterator risen = tree.rotateLargeRight(tree.find(5));
  BOOST_TEST(risen->first == 5);
  BOOST_TEST(inorder(tree) == "ceh");
  BOOST_TEST(tree.height() == 2u);
}

BOOST_AUTO_TEST_CASE(rotate_large_left_lifts_grandchild)
{
  Tree tree;
  tree.push(3, "c");
  tree.push(8, "h");
  tree.push(5, "e");
  Tree::const_iterator risen = tree.rotateLargeLeft(tree.find(5));
  BOOST_TEST(risen->first == 5);
  BOOST_TEST(inorder(tree) == "ceh");
  BOOST_TEST(tree.height() == 2u);
}

BOOST_AUTO_TEST_CASE(rotate_root_throws)
{
  Tree tree;
  tree.push(1, "a");
  BOOST_CHECK_THROW(tree.rotateRight(tree.find(1)), std::logic_error);
}

BOOST_AUTO_TEST_SUITE_END()
