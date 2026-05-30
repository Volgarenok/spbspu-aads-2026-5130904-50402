#define BOOST_TEST_MODULE S4
#include <boost/test/included/unit_test.hpp>
#include "BSTree.hpp"

BOOST_AUTO_TEST_CASE(new_tree_is_empty)
{
  shirokov::BSTree< int, int, std::less< int > > tree;
  BOOST_TEST(tree.empty());
}

BOOST_AUTO_TEST_CASE(insert_value_make_tree_non_empty)
{
  shirokov::BSTree< int, int, std::less< int > > tree;
  BOOST_REQUIRE(tree.empty());
  BOOST_REQUIRE(tree.insert(0, 1));
  BOOST_TEST(!tree.empty());
}
