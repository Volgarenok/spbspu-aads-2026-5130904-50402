#include <stdexcept>
#include <boost/test/unit_test.hpp>
#include "bstree.hpp"

namespace pozdnyakov
{
  BSTree< int, std::string > createTestTree()
  {
    BSTree< int, std::string > tree;
    tree.push(3, "three");
    tree.push(1, "one");
    tree.push(2, "two");
    tree.push(4, "four");
    return tree;
  }
}

BOOST_AUTO_TEST_SUITE(BSTreeTests)

BOOST_AUTO_TEST_CASE(test_default_constructor)
{
  pozdnyakov::BSTree< int, std::string > tree;
  BOOST_CHECK(tree.begin() == tree.end());
}

BOOST_AUTO_TEST_CASE(test_push)
{
  pozdnyakov::BSTree< int, std::string > tree;
  tree.push(1, "one");
  BOOST_CHECK(tree.begin() != tree.end());
  BOOST_CHECK_EQUAL(tree.get(1), "one");

  tree.push(2, "two");
  tree.push(3, "three");
  BOOST_CHECK_EQUAL(tree.get(2), "two");
  BOOST_CHECK_EQUAL(tree.get(3), "three");
}

BOOST_AUTO_TEST_CASE(test_get_const)
{
  const pozdnyakov::BSTree< int, std::string > tree = pozdnyakov::createTestTree();
  BOOST_CHECK_EQUAL(tree.get(1), "one");
  BOOST_CHECK_EQUAL(tree.get(2), "two");
  BOOST_CHECK_EQUAL(tree.get(3), "three");
  BOOST_CHECK_EQUAL(tree.get(4), "four");
}

BOOST_AUTO_TEST_CASE(test_get_exception)
{
  pozdnyakov::BSTree< int, std::string > tree;
  tree.push(1, "one");
  BOOST_CHECK_THROW(tree.get(2), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(test_contains)
{
  pozdnyakov::BSTree< int, std::string > tree;
  tree.push(1, "one");
  BOOST_CHECK(tree.contains(1));
  BOOST_CHECK(!tree.contains(2));
}

BOOST_AUTO_TEST_CASE(test_intersect)
{
  pozdnyakov::BSTree< int, std::string > tree1;
  tree1.push(1, "one");
  tree1.push(2, "two");

  pozdnyakov::BSTree< int, std::string > tree2;
  tree2.push(2, "two");
  tree2.push(3, "three");

  const pozdnyakov::BSTree< int, std::string > result = pozdnyakov::intersect(tree1, tree2);
  BOOST_CHECK(result.contains(2));
  BOOST_CHECK(!result.contains(1));
  BOOST_CHECK(!result.contains(3));
}

BOOST_AUTO_TEST_CASE(test_union)
{
  pozdnyakov::BSTree< int, std::string > tree1;
  tree1.push(1, "one");
  tree1.push(2, "two");

  pozdnyakov::BSTree< int, std::string > tree2;
  tree2.push(2, "two_new");
  tree2.push(3, "three");

  const pozdnyakov::BSTree< int, std::string > result = pozdnyakov::union_(tree1, tree2);
  BOOST_CHECK(result.contains(1));
  BOOST_CHECK(result.contains(2));
  BOOST_CHECK(result.contains(3));
}

BOOST_AUTO_TEST_SUITE_END()
