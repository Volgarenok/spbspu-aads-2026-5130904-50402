#include <boost/test/unit_test.hpp>
#include "BSTree.hpp"

using namespace saldaev;

BOOST_AUTO_TEST_SUITE(BSTreeTests)

BOOST_AUTO_TEST_CASE(test_default_constructor)
{
  BSTree< int, std::string > tree;
  BOOST_CHECK(tree.empty());
  BOOST_CHECK_EQUAL(tree.size(), 0);
  BOOST_CHECK(tree.begin() == tree.end());
}

BOOST_AUTO_TEST_CASE(test_push_and_size)
{
  BSTree< int, std::string > tree;
  tree.push(10, "val_one");
  BOOST_CHECK_EQUAL(tree.size(), 1);
  BOOST_CHECK(!tree.empty());

  tree.push(20, "val_two");
  tree.push(30, "val_three");
  BOOST_CHECK_EQUAL(tree.size(), 3);
}

BOOST_AUTO_TEST_CASE(test_get_const)
{
  BSTree< int, std::string > tree;
  tree.push(10, "val_one");
  tree.push(20, "val_two");
  const std::string val = tree.get(10);
  BOOST_CHECK_EQUAL(val, "val_one");
}

BOOST_AUTO_TEST_CASE(test_get)
{
  BSTree< int, std::string > tree;
  tree.push(10, "val_one");
  tree.get(10) = "VAL_ONE";
  BOOST_CHECK_EQUAL(tree.get(10), "VAL_ONE");
}

BOOST_AUTO_TEST_CASE(test_at_not_found)
{
  BSTree< int, std::string > tree;
  tree.push(10, "val_one");

  BOOST_CHECK_THROW(tree.get(20), std::out_of_range);
  const BSTree< int, std::string > &ctree = tree;
  BOOST_CHECK_THROW(ctree.get(20), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(test_push_update_existing)
{
  BSTree< int, std::string > tree;
  tree.push(10, "val_one");
  tree.push(10, "VAL_ONE");

  BOOST_CHECK_EQUAL(tree.size(), 1);
  BOOST_CHECK_EQUAL(tree.get(10), "VAL_ONE");
}

BOOST_AUTO_TEST_CASE(test_drop)
{
  BSTree< int, std::string > tree;
  tree.push(10, "val_one");
  tree.push(20, "val_two");
  tree.push(30, "val_three");

  tree.drop(20);
  BOOST_CHECK_EQUAL(tree.size(), 2);
  BOOST_CHECK_EQUAL(tree.get(10), "val_one");
  BOOST_CHECK_EQUAL(tree.get(30), "val_three");
  BOOST_CHECK_THROW(tree.get(20), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(test_drop_root_with_two_children)
{
  BSTree< int, std::string > tree;
  tree.push(20, "val_two");
  tree.push(10, "val_one");
  tree.push(30, "val_three");

  tree.drop(20);
  BOOST_CHECK_EQUAL(tree.size(), 2);
  BOOST_CHECK((tree.get(10) == "val_one" && tree.get(30) == "val_three")
              || (tree.get(30) == "val_three" && tree.get(10) == "val_one"));
}

BOOST_AUTO_TEST_CASE(test_clear)
{
  BSTree< int, std::string > tree;
  tree.push(10, "val_one");
  tree.push(20, "val_two");
  tree.push(30, "val_three");

  tree.clear();
  BOOST_CHECK(tree.empty());
  BOOST_CHECK_EQUAL(tree.size(), 0);
  BOOST_CHECK_THROW(tree.get(10), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(test_copy_constructor)
{
  BSTree< int, std::string > tree1;
  tree1.push(10, "val_one");
  tree1.push(20, "val_two");
  tree1.push(30, "val_three");

  BSTree< int, std::string > tree2(tree1);

  BOOST_CHECK_EQUAL(tree2.size(), 3);
  BOOST_CHECK_EQUAL(tree2.get(10), "val_one");
  BOOST_CHECK_EQUAL(tree2.get(20), "val_two");
  BOOST_CHECK_EQUAL(tree2.get(30), "val_three");

  tree2.get(10) = "VAL_ONE";
  BOOST_CHECK_EQUAL(tree1.get(10), "val_one");
  BOOST_CHECK_EQUAL(tree2.get(10), "VAL_ONE");
}

BOOST_AUTO_TEST_CASE(test_copy_assignment)
{
  BSTree< int, std::string > tree1;
  tree1.push(10, "val_one");

  BSTree< int, std::string > tree2;
  tree2.push(990, "val_ninety-nine");

  tree2 = tree1;

  BOOST_CHECK_EQUAL(tree2.size(), 1);
  BOOST_CHECK_EQUAL(tree2.get(10), "val_one");
  BOOST_CHECK_THROW(tree2.get(990), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(test_move_constructor)
{
  BSTree< int, std::string > tree1;
  tree1.push(10, "val_one");
  tree1.push(20, "val_two");

  BSTree< int, std::string > tree2(std::move(tree1));

  BOOST_CHECK_EQUAL(tree2.size(), 2);
  BOOST_CHECK_EQUAL(tree2.get(10), "val_one");
  BOOST_CHECK_EQUAL(tree2.get(20), "val_two");
  BOOST_CHECK(tree1.empty());
}

BOOST_AUTO_TEST_CASE(test_move_assignment)
{
  BSTree< int, std::string > tree1;
  tree1.push(10, "val_one");

  BSTree< int, std::string > tree2;
  tree2.push(990, "val_ninety-nine");

  tree2 = std::move(tree1);

  BOOST_CHECK_EQUAL(tree2.size(), 1);
  BOOST_CHECK_EQUAL(tree2.get(10), "val_one");
  BOOST_CHECK(tree1.empty());
}

BOOST_AUTO_TEST_CASE(test_iterator_traversal)
{
  BSTree< int, std::string > tree;
  tree.push(30, "val_three");
  tree.push(10, "val_one");
  tree.push(20, "val_two");
  tree.push(40, "val_four");

  std::vector< std::pair< const int, std::string > > result;
  for (auto it = tree.begin(); it != tree.end(); ++it) {
    result.push_back(*it);
  }

  BOOST_CHECK_EQUAL(result.size(), 4);
  BOOST_CHECK_EQUAL(result[0].first, 10);
  BOOST_CHECK_EQUAL(result[1].first, 20);
  BOOST_CHECK_EQUAL(result[2].first, 30);
  BOOST_CHECK_EQUAL(result[3].first, 40);
}

BOOST_AUTO_TEST_CASE(test_const_iterator)
{
  const BSTree< int, std::string > tree;
  BOOST_CHECK(tree.cbegin() == tree.cend());
}

BOOST_AUTO_TEST_CASE(test_iterator_modification)
{
  BSTree< int, std::string > tree;
  tree.push(10, "val_one");
  tree.push(20, "val_two");

  for (auto it = tree.begin(); it != tree.end(); ++it) {
    auto pair = *it;
    if (pair.first == 10) {
      tree.get(10) = "VAL_ONE";
    }
  }
  BOOST_CHECK_EQUAL(tree.get(10), "VAL_ONE");
}

BOOST_AUTO_TEST_CASE(test_height_empty)
{
  BSTree< int, std::string > tree;
  BOOST_CHECK_EQUAL(tree.height(), 0);
}

BOOST_AUTO_TEST_CASE(test_height_single)
{
  BSTree< int, std::string > tree;
  tree.push(10, "val_one");
  BOOST_CHECK_EQUAL(tree.height(), 1);
}

BOOST_AUTO_TEST_CASE(test_height_balanced)
{
  BSTree< int, std::string > tree;
  tree.push(20, "val_two");
  tree.push(10, "val_one");
  tree.push(30, "val_three");
  BOOST_CHECK_EQUAL(tree.height(), 2);
}

BOOST_AUTO_TEST_CASE(test_height_degenerate)
{
  BSTree< int, std::string > tree;
  tree.push(10, "val_one");
  tree.push(20, "val_two");
  tree.push(30, "val_three");
  tree.push(40, "val_four");
  BOOST_CHECK_EQUAL(tree.height(), 4);
}

BOOST_AUTO_TEST_CASE(test_height_iterator)
{
  BSTree< int, std::string > tree;
  tree.push(20, "val_two");
  tree.push(10, "val_one");
  tree.push(30, "val_three");

  auto it = tree.cbegin();
  ++it;
  BOOST_CHECK_EQUAL(tree.height(it), 2);
}

BOOST_AUTO_TEST_CASE(test_rotate_left)
{
  BSTree< int, std::string > tree;
  tree.push(10, "val_one");
  tree.push(20, "val_two");

  auto it = tree.cbegin();
  ++it;
  tree.rotateLeft(it);

  BOOST_CHECK_EQUAL(tree.size(), 2);
  BOOST_CHECK_EQUAL(tree.get(10), "val_one");
  BOOST_CHECK_EQUAL(tree.get(20), "val_two");
}

BOOST_AUTO_TEST_CASE(test_rotate_right)
{
  BSTree< int, std::string > tree;
  tree.push(20, "val_two");
  tree.push(10, "val_one");

  auto it = tree.cbegin();
  ++it;
  tree.rotateRight(it);

  BOOST_CHECK_EQUAL(tree.size(), 2);
  BOOST_CHECK_EQUAL(tree.get(10), "val_one");
  BOOST_CHECK_EQUAL(tree.get(20), "val_two");
}

BOOST_AUTO_TEST_CASE(test_rotate_large_left)
{
  BSTree< int, std::string > tree;
  tree.push(30, "val_three");
  tree.push(10, "val_one");
  tree.push(20, "val_two");

  auto it = tree.cbegin();
  ++it;
  ++it;
  tree.rotateLargeLeft(it);

  BOOST_CHECK_EQUAL(tree.size(), 3);
  BOOST_CHECK_EQUAL(tree.get(10), "val_one");
  BOOST_CHECK_EQUAL(tree.get(20), "val_two");
  BOOST_CHECK_EQUAL(tree.get(30), "val_three");
}

BOOST_AUTO_TEST_CASE(test_rotate_large_right)
{
  BSTree< int, std::string > tree;
  tree.push(10, "val_one");
  tree.push(30, "val_three");
  tree.push(20, "val_two");

  auto it = tree.cbegin();
  tree.rotateLargeRight(it);

  BOOST_CHECK_EQUAL(tree.size(), 3);
  BOOST_CHECK_EQUAL(tree.get(10), "val_one");
  BOOST_CHECK_EQUAL(tree.get(20), "val_two");
  BOOST_CHECK_EQUAL(tree.get(30), "val_three");
}

BOOST_AUTO_TEST_CASE(test_push_rvalue)
{
  BSTree< int, std::string > tree;
  std::string val = "val_rvalue";
  tree.push(10, std::move(val));
  BOOST_CHECK_EQUAL(tree.get(10), "val_rvalue");
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
  tree.push(10, "val_one");

  auto it1 = tree.begin();
  auto it2 = tree.begin();
  auto it3 = tree.end();

  BOOST_CHECK(it1 == it2);
  BOOST_CHECK(it1 != it3);
  BOOST_CHECK(tree.begin() != tree.end());
}

BOOST_AUTO_TEST_SUITE_END()
