#include <boost/test/unit_test.hpp>
#include "bstree.hpp"

using namespace karpovich;

BOOST_AUTO_TEST_SUITE(BSTreeTests)

BOOST_AUTO_TEST_CASE(test_default_constructor)
{
  BSTree< int, std::string > tree;
  BOOST_CHECK(tree.empty());
  BOOST_CHECK_EQUAL(tree.size(), 0);
  BOOST_CHECK_EQUAL(tree.begin(), tree.end());
}

BOOST_AUTO_TEST_CASE(test_push_and_size)
{
  BSTree< int, std::string > tree;
  tree.push(1, "one");
  BOOST_CHECK_EQUAL(tree.size(), 1);
  BOOST_CHECK(!tree.empty());

  tree.push(2, "two");
  tree.push(3, "three");
  BOOST_CHECK_EQUAL(tree.size(), 3);
}

BOOST_AUTO_TEST_CASE(test_at_const)
{
  BSTree< int, std::string > tree;
  tree.push(1, "one");
  tree.push(2, "two");

  BOOST_CHECK_EQUAL(tree.at(1), "one");
  BOOST_CHECK_EQUAL(tree.at(2), "two");
}

BOOST_AUTO_TEST_CASE(test_at_non_const)
{
  BSTree< int, std::string > tree;
  tree.push(1, "one");
  tree.at(1) = "ONE";
  BOOST_CHECK_EQUAL(tree.at(1), "ONE");
}

BOOST_AUTO_TEST_CASE(test_at_not_found)
{
  BSTree< int, std::string > tree;
  tree.push(1, "one");

  BOOST_CHECK_THROW(tree.at(2), std::out_of_range);
  const BSTree< int, std::string > &ctree = tree;
  BOOST_CHECK_THROW(ctree.at(2), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(test_push_update_existing)
{
  BSTree< int, std::string > tree;
  tree.push(1, "one");
  tree.push(1, "ONE");

  BOOST_CHECK_EQUAL(tree.size(), 1);
  BOOST_CHECK_EQUAL(tree.at(1), "ONE");
}

BOOST_AUTO_TEST_CASE(test_drop)
{
  BSTree< int, std::string > tree;
  tree.push(1, "one");
  tree.push(2, "two");
  tree.push(3, "three");

  std::string val = tree.drop(2);
  BOOST_CHECK_EQUAL(val, "two");
  BOOST_CHECK_EQUAL(tree.size(), 2);
  BOOST_CHECK_EQUAL(tree.at(1), "one");
  BOOST_CHECK_EQUAL(tree.at(3), "three");
  BOOST_CHECK_THROW(tree.at(2), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(test_drop_root_with_two_children)
{
  BSTree< int, std::string > tree;
  tree.push(2, "two");
  tree.push(1, "one");
  tree.push(3, "three");

  tree.drop(2);
  BOOST_CHECK_EQUAL(tree.size(), 2);
  BOOST_CHECK((tree.at(1) == "one" && tree.at(3) == "three") || (tree.at(3) == "three" && tree.at(1) == "one"));
}

BOOST_AUTO_TEST_CASE(test_clear)
{
  BSTree< int, std::string > tree;
  tree.push(1, "one");
  tree.push(2, "two");
  tree.push(3, "three");

  tree.clear();
  BOOST_CHECK(tree.empty());
  BOOST_CHECK_EQUAL(tree.size(), 0);
  BOOST_CHECK_THROW(tree.at(1), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(test_copy_constructor)
{
  BSTree< int, std::string > tree1;
  tree1.push(1, "one");
  tree1.push(2, "two");
  tree1.push(3, "three");

  BSTree< int, std::string > tree2(tree1);

  BOOST_CHECK_EQUAL(tree2.size(), 3);
  BOOST_CHECK_EQUAL(tree2.at(1), "one");
  BOOST_CHECK_EQUAL(tree2.at(2), "two");
  BOOST_CHECK_EQUAL(tree2.at(3), "three");

  tree2.at(1) = "ONE";
  BOOST_CHECK_EQUAL(tree1.at(1), "one");
  BOOST_CHECK_EQUAL(tree2.at(1), "ONE");
}

BOOST_AUTO_TEST_CASE(test_copy_assignment)
{
  BSTree< int, std::string > tree1;
  tree1.push(1, "one");

  BSTree< int, std::string > tree2;
  tree2.push(99, "ninety-nine");

  tree2 = tree1;

  BOOST_CHECK_EQUAL(tree2.size(), 1);
  BOOST_CHECK_EQUAL(tree2.at(1), "one");
  BOOST_CHECK_THROW(tree2.at(99), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(test_move_constructor)
{
  BSTree< int, std::string > tree1;
  tree1.push(1, "one");
  tree1.push(2, "two");

  BSTree< int, std::string > tree2(std::move(tree1));

  BOOST_CHECK_EQUAL(tree2.size(), 2);
  BOOST_CHECK_EQUAL(tree2.at(1), "one");
  BOOST_CHECK_EQUAL(tree2.at(2), "two");
  BOOST_CHECK(tree1.empty());
}

BOOST_AUTO_TEST_CASE(test_move_assignment)
{
  BSTree< int, std::string > tree1;
  tree1.push(1, "one");

  BSTree< int, std::string > tree2;
  tree2.push(99, "ninety-nine");

  tree2 = std::move(tree1);

  BOOST_CHECK_EQUAL(tree2.size(), 1);
  BOOST_CHECK_EQUAL(tree2.at(1), "one");
  BOOST_CHECK(tree1.empty());
}

BOOST_AUTO_TEST_CASE(test_swap)
{
  BSTree< int, std::string > tree1;
  tree1.push(1, "one");

  BSTree< int, std::string > tree2;
  tree2.push(2, "two");
  tree2.push(3, "three");

  tree1.swap(tree2);

  BOOST_CHECK_EQUAL(tree1.size(), 2);
  BOOST_CHECK_EQUAL(tree1.at(2), "two");
  BOOST_CHECK_EQUAL(tree2.size(), 1);
  BOOST_CHECK_EQUAL(tree2.at(1), "one");
}

BOOST_AUTO_TEST_CASE(test_iterator_traversal)
{
  BSTree< int, std::string > tree;
  tree.push(3, "three");
  tree.push(1, "one");
  tree.push(2, "two");
  tree.push(4, "four");

  std::vector< std::pair< const int, std::string > > result;
  for (auto it = tree.begin(); it != tree.end(); ++it) {
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
  const BSTree< int, std::string > tree;
  BOOST_CHECK(tree.cbegin() == tree.cend());
}

BOOST_AUTO_TEST_CASE(test_iterator_modification)
{
  BSTree< int, std::string > tree;
  tree.push(1, "one");
  tree.push(2, "two");

  for (auto it = tree.begin(); it != tree.end(); ++it) {
    auto pair = *it;
    if (pair.first == 1) {
      tree.at(1) = "ONE";
    }
  }
  BOOST_CHECK_EQUAL(tree.at(1), "ONE");
}

BOOST_AUTO_TEST_CASE(test_height_empty)
{
  BSTree< int, std::string > tree;
  BOOST_CHECK_EQUAL(tree.height(), 0);
}

BOOST_AUTO_TEST_CASE(test_height_single)
{
  BSTree< int, std::string > tree;
  tree.push(1, "one");
  BOOST_CHECK_EQUAL(tree.height(), 1);
}

BOOST_AUTO_TEST_CASE(test_height_balanced)
{
  BSTree< int, std::string > tree;
  tree.push(2, "two");
  tree.push(1, "one");
  tree.push(3, "three");
  BOOST_CHECK_EQUAL(tree.height(), 2);
}

BOOST_AUTO_TEST_CASE(test_height_degenerate)
{
  BSTree< int, std::string > tree;
  tree.push(1, "one");
  tree.push(2, "two");
  tree.push(3, "three");
  tree.push(4, "four");
  BOOST_CHECK_EQUAL(tree.height(), 4);
}

BOOST_AUTO_TEST_CASE(test_height_iterator)
{
  BSTree< int, std::string > tree;
  tree.push(2, "two");
  tree.push(1, "one");
  tree.push(3, "three");

  auto it = tree.cbegin();
  ++it;
  BOOST_CHECK_EQUAL(tree.height(it), 2);
}

BOOST_AUTO_TEST_CASE(test_rotate_left)
{
  BSTree< int, std::string > tree;
  tree.push(1, "one");
  tree.push(2, "two");

  auto it = tree.cbegin();
  ++it;
  tree.rotateLeft(it);

  BOOST_CHECK_EQUAL(tree.size(), 2);
  BOOST_CHECK_EQUAL(tree.at(1), "one");
  BOOST_CHECK_EQUAL(tree.at(2), "two");
}

BOOST_AUTO_TEST_CASE(test_rotate_right)
{
  BSTree< int, std::string > tree;
  tree.push(2, "two");
  tree.push(1, "one");

  auto it = tree.cbegin();
  ++it;
  tree.rotateRight(it);

  BOOST_CHECK_EQUAL(tree.size(), 2);
  BOOST_CHECK_EQUAL(tree.at(1), "one");
  BOOST_CHECK_EQUAL(tree.at(2), "two");
}

BOOST_AUTO_TEST_CASE(test_rotate_large_left)
{
  BSTree< int, std::string > tree;
  tree.push(3, "three");
  tree.push(1, "one");
  tree.push(2, "two");

  auto it = tree.cbegin();
  ++it;
  ++it;
  tree.rotateLargeLeft(it);

  BOOST_CHECK_EQUAL(tree.size(), 3);
  BOOST_CHECK_EQUAL(tree.at(1), "one");
  BOOST_CHECK_EQUAL(tree.at(2), "two");
  BOOST_CHECK_EQUAL(tree.at(3), "three");
}

BOOST_AUTO_TEST_CASE(test_rotate_large_right)
{
  BSTree< int, std::string > tree;
  tree.push(1, "one");
  tree.push(3, "three");
  tree.push(2, "two");

  auto it = tree.cbegin();
  tree.rotateLargeRight(it);

  BOOST_CHECK_EQUAL(tree.size(), 3);
  BOOST_CHECK_EQUAL(tree.at(1), "one");
  BOOST_CHECK_EQUAL(tree.at(2), "two");
  BOOST_CHECK_EQUAL(tree.at(3), "three");
}

BOOST_AUTO_TEST_CASE(test_push_rvalue)
{
  BSTree< int, std::string > tree;
  std::string val = "rvalue";
  tree.push(1, std::move(val));
  BOOST_CHECK_EQUAL(tree.at(1), "rvalue");
}

BOOST_AUTO_TEST_CASE(test_empty_tree_operations)
{
  BSTree< int, std::string > tree;
  BOOST_CHECK(tree.begin() == tree.end());
  BOOST_CHECK(tree.cbegin() == tree.cend());
  BOOST_CHECK_EQUAL(tree.height(), 0);
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
