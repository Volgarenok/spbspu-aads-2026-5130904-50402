#include <boost/test/unit_test.hpp>
#include "bstree.hpp"
#include <string>

using namespace muhamadiarov;

BOOST_AUTO_TEST_SUITE(BSTreeTest)

BOOST_AUTO_TEST_CASE(DefaultConstructor)
{
  BSTree<int, std::string> tree;
  BOOST_CHECK_EQUAL(tree.size(), 0);
  BOOST_CHECK(tree.empty());
  BOOST_CHECK(tree.begin() == tree.end());
}

BOOST_AUTO_TEST_CASE(CopyConstructor)
{
  BSTree<int, std::string> tree1;
  tree1.push(10, "ten");
  tree1.push(5, "five");

  BSTree<int, std::string> tree2(tree1);

  BOOST_CHECK_EQUAL(tree1.size(), tree2.size());
  BOOST_CHECK(tree2.has(10));
  BOOST_CHECK(tree2.has(5));
  BOOST_CHECK_EQUAL(tree2.get(10), "ten");
}

BOOST_AUTO_TEST_CASE(MoveConstructor)
{
  BSTree<int, std::string> tree1;
  tree1.push(10, "ten");
  tree1.push(20, "twenty");

  BSTree<int, std::string> tree2(std::move(tree1));

  BOOST_CHECK_EQUAL(tree2.size(), 2);
  BOOST_CHECK(tree2.has(10));
  BOOST_CHECK(tree1.empty());
}

BOOST_AUTO_TEST_CASE(PushSingleElement)
{
  BSTree<int, std::string> tree;
  tree.push(42, "answer");

  BOOST_CHECK_EQUAL(tree.size(), 1);
  BOOST_CHECK(tree.has(42));
  BOOST_CHECK_EQUAL(tree.get(42), "answer");
}

BOOST_AUTO_TEST_CASE(PushDuplicateKeyUpdate)
{
  BSTree<int, std::string> tree;
  tree.push(1, "one");
  tree.push(1, "ONE");

  BOOST_CHECK_EQUAL(tree.size(), 1);
  BOOST_CHECK_EQUAL(tree.get(1), "ONE");
}

BOOST_AUTO_TEST_CASE(GetNonExistentThrows)
{
  BSTree<int, std::string> tree;
  tree.push(1, "one");

  BOOST_CHECK_THROW(tree.get(2), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(DropLeafNode)
{
  BSTree<int, std::string> tree;
  tree.push(10, "ten");
  tree.push(5, "five");
  tree.push(15, "fifteen");

  tree.drop(5);

  BOOST_CHECK_EQUAL(tree.size(), 2);
  BOOST_CHECK(!tree.has(5));
  BOOST_CHECK(tree.has(10));
  BOOST_CHECK(tree.has(15));
}

BOOST_AUTO_TEST_CASE(DropNodeWithTwoChildren)
{
  BSTree<int, std::string> tree;
  tree.push(50, "fifty");
  tree.push(30, "thirty");
  tree.push(70, "seventy");
  tree.push(20, "twenty");
  tree.push(40, "forty");
  tree.push(60, "sixty");
  tree.push(80, "eighty");

  tree.drop(50);

  BOOST_CHECK_EQUAL(tree.size(), 6);
  BOOST_CHECK(!tree.has(50));
  BOOST_CHECK(tree.has(30));
  BOOST_CHECK(tree.has(70));
}

BOOST_AUTO_TEST_CASE(DropRoot)
{
  BSTree<int, std::string> tree;
  tree.push(10, "ten");
  tree.push(5, "five");
  tree.push(15, "fifteen");

  tree.drop(10);

  BOOST_CHECK_EQUAL(tree.size(), 2);
  BOOST_CHECK(!tree.has(10));
  BOOST_CHECK(tree.has(5));
  BOOST_CHECK(tree.has(15));
}

BOOST_AUTO_TEST_CASE(DropNonExistentThrows)
{
  BSTree<int, std::string> tree;
  tree.push(1, "one");

  BOOST_CHECK_THROW(tree.drop(2), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(Iterator)
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

BOOST_AUTO_TEST_CASE(EmptyTreeHeight)
{
  BSTree<int, std::string> tree;
  BOOST_CHECK_EQUAL(tree.height(), 0);
}

BOOST_AUTO_TEST_CASE(SingleNodeHeight)
{
  BSTree<int, std::string> tree;
  tree.push(1, "one");
  BOOST_CHECK_EQUAL(tree.height(), 1);
}

BOOST_AUTO_TEST_CASE(BalancedTreeHeight)
{
  BSTree<int, std::string> tree;
  tree.push(50, "fifty");
  tree.push(30, "thirty");
  tree.push(70, "seventy");
  tree.push(20, "twenty");
  tree.push(40, "forty");
  tree.push(60, "sixty");
  tree.push(80, "eighty");

  BOOST_CHECK_EQUAL(tree.height(), 3);
}

BOOST_AUTO_TEST_CASE(Clear)
{
  BSTree<int, std::string> tree;
  tree.push(1, "one");
  tree.push(2, "two");
  tree.push(3, "three");

  tree.clear();

  BOOST_CHECK(tree.empty());
  BOOST_CHECK_EQUAL(tree.size(), 0);
  BOOST_CHECK(!tree.has(1));
}

BOOST_AUTO_TEST_CASE(Swap)
{
  BSTree<int, std::string> tree1;
  tree1.push(1, "one");
  tree1.push(2, "two");

  BSTree<int, std::string> tree2;
  tree2.push(3, "three");

  tree1.swap(tree2);

  BOOST_CHECK_EQUAL(tree1.size(), 1);
  BOOST_CHECK_EQUAL(tree2.size(), 2);
  BOOST_CHECK(tree1.has(3));
  BOOST_CHECK(tree2.has(1));
}

BOOST_AUTO_TEST_CASE(RotateLeft)
{
  BSTree<int, std::string> tree;
  tree.push(10, "ten");
  tree.push(20, "twenty");

  auto it = tree.cbegin();
  tree.rotateLeft(it);

  auto newIt = tree.cbegin();
  BOOST_CHECK_EQUAL(newIt->first, 10);
  ++newIt;
  BOOST_CHECK_EQUAL(newIt->first, 20);
}

BOOST_AUTO_TEST_CASE(RotateRight)
{
  BSTree<int, std::string> tree;
  tree.push(20, "twenty");
  tree.push(10, "ten");

  auto it = tree.cbegin();
  ++it;
  tree.rotateRight(it);

  BOOST_CHECK(tree.has(10));
  BOOST_CHECK(tree.has(20));
}

BOOST_AUTO_TEST_SUITE_END()
