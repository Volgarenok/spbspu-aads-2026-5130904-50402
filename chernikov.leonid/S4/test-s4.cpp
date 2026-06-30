#include <boost/test/unit_test.hpp>
#include "bstree.hpp"
#include <string>
#include <stdexcept>

namespace chernikov {

  BOOST_AUTO_TEST_SUITE(BSTreeTests)

  BOOST_AUTO_TEST_CASE(BSTreeEmptyInitially)
  {
    BSTree< int, std::string > tree;
    BOOST_CHECK(tree.empty());
    BOOST_CHECK_EQUAL(tree.size(), 0);
    BOOST_CHECK_EQUAL(tree.height(), 0);
  }

  BOOST_AUTO_TEST_CASE(BSTreePushAndGet)
  {
    BSTree< int, std::string > tree;
    tree.push(10, "ten");
    BOOST_CHECK(!tree.empty());
    BOOST_CHECK_EQUAL(tree.size(), 1);
    BOOST_CHECK_EQUAL(tree.get(10), "ten");
  }

  BOOST_AUTO_TEST_CASE(BSTreePushUpdate)
  {
    BSTree< int, std::string > tree;
    tree.push(10, "ten");
    tree.push(10, "TEN");
    BOOST_CHECK_EQUAL(tree.size(), 1);
    BOOST_CHECK_EQUAL(tree.get(10), "TEN");
  }

  BOOST_AUTO_TEST_CASE(BSTreeMultiplePush)
  {
    BSTree< int, std::string > tree;
    tree.push(50, "fifty");
    tree.push(30, "thirty");
    tree.push(70, "seventy");
    tree.push(20, "twenty");
    tree.push(40, "forty");

    BOOST_CHECK_EQUAL(tree.size(), 5);
    BOOST_CHECK(tree.has(20));
    BOOST_CHECK(tree.has(30));
    BOOST_CHECK(tree.has(40));
    BOOST_CHECK(tree.has(50));
    BOOST_CHECK(tree.has(70));
  }

  BOOST_AUTO_TEST_CASE(BSTreeHas)
  {
    BSTree< int, std::string > tree;
    tree.push(1, "one");
    BOOST_CHECK(tree.has(1));
    BOOST_CHECK(!tree.has(2));
  }

  BOOST_AUTO_TEST_CASE(BSTreeGetThrows)
  {
    BSTree< int, std::string > tree;
    BOOST_CHECK_THROW(tree.get(999), std::out_of_range);
  }

  BOOST_AUTO_TEST_CASE(BSTreeDrop)
  {
    BSTree< int, std::string > tree;
    tree.push(10, "ten");
    tree.push(20, "twenty");

    std::string val = tree.drop(10);
    BOOST_CHECK_EQUAL(val, "ten");
    BOOST_CHECK(!tree.has(10));
    BOOST_CHECK_EQUAL(tree.size(), 1);
    BOOST_CHECK(tree.has(20));
  }

  BOOST_AUTO_TEST_CASE(BSTreeDropThrows)
  {
    BSTree< int, std::string > tree;
    BOOST_CHECK_THROW(tree.drop(999), std::out_of_range);
  }

  BOOST_AUTO_TEST_CASE(BSTreeDropRoot)
  {
    BSTree< int, std::string > tree;
    tree.push(50, "root");
    tree.push(30, "left");
    tree.push(70, "right");

    tree.drop(50);
    BOOST_CHECK(!tree.has(50));
    BOOST_CHECK_EQUAL(tree.size(), 2);
    BOOST_CHECK(tree.has(30));
    BOOST_CHECK(tree.has(70));
  }

  BOOST_AUTO_TEST_CASE(BSTreeDropLeaf)
  {
    BSTree< int, std::string > tree;
    tree.push(50, "root");
    tree.push(30, "leaf");

    tree.drop(30);
    BOOST_CHECK(!tree.has(30));
    BOOST_CHECK_EQUAL(tree.size(), 1);
  }

  BOOST_AUTO_TEST_CASE(BSTreeDropNodeWithOneChild)
  {
    BSTree< int, std::string > tree;
    tree.push(50, "root");
    tree.push(30, "parent");
    tree.push(20, "child");

    tree.drop(30);
    BOOST_CHECK(!tree.has(30));
    BOOST_CHECK(tree.has(50));
    BOOST_CHECK(tree.has(20));
  }

  BOOST_AUTO_TEST_CASE(BSTreeDropNodeWithTwoChildren)
  {
    BSTree< int, std::string > tree;
    tree.push(50, "root");
    tree.push(30, "target");
    tree.push(20, "left");
    tree.push(40, "right");

    tree.drop(30);
    BOOST_CHECK(!tree.has(30));
    BOOST_CHECK_EQUAL(tree.size(), 3);
  }

  BOOST_AUTO_TEST_CASE(BSTreeConstIterator)
  {
    BSTree< int, std::string > tree;
    tree.push(1, "one");
    tree.push(2, "two");

    const auto &ctree = tree;
    int count = 0;
    for (auto it = ctree.begin(); it != ctree.end(); ++it)
    {
      ++count;
    }
    BOOST_CHECK_EQUAL(count, 2);
  }

  BOOST_AUTO_TEST_CASE(BSTreeCopyConstructor)
  {
    BSTree< int, std::string > tree1;
    tree1.push(1, "one");
    tree1.push(2, "two");

    BSTree< int, std::string > tree2(tree1);
    BOOST_CHECK_EQUAL(tree2.size(), 2);
    BOOST_CHECK(tree2.has(1));
    BOOST_CHECK_EQUAL(tree2.get(1), "one");

    tree2.push(3, "three");
    BOOST_CHECK(!tree1.has(3));
  }

  BOOST_AUTO_TEST_CASE(BSTreeMoveConstructor)
  {
    BSTree< int, std::string > tree1;
    tree1.push(1, "one");

    BSTree< int, std::string > tree2(std::move(tree1));
    BOOST_CHECK(tree1.empty());
    BOOST_CHECK_EQUAL(tree2.size(), 1);
  }

  BOOST_AUTO_TEST_CASE(BSTreeAssignment)
  {
    BSTree< int, std::string > tree1;
    tree1.push(1, "one");

    BSTree< int, std::string > tree2;
    tree2 = tree1;
    BOOST_CHECK_EQUAL(tree2.size(), 1);
    BOOST_CHECK(tree2.has(1));
  }

  BOOST_AUTO_TEST_CASE(BSTreeMoveAssignment)
  {
    BSTree< int, std::string > tree1;
    tree1.push(1, "one");

    BSTree< int, std::string > tree2;
    tree2 = std::move(tree1);
    BOOST_CHECK(tree1.empty());
    BOOST_CHECK_EQUAL(tree2.size(), 1);
  }

  BOOST_AUTO_TEST_CASE(BSTreeHeight)
  {
    BSTree< int, std::string > tree;
    BOOST_CHECK_EQUAL(tree.height(), 0);

    tree.push(50, "root");
    BOOST_CHECK_EQUAL(tree.height(), 1);

    tree.push(30, "left");
    BOOST_CHECK_EQUAL(tree.height(), 2);

    tree.push(70, "right");
    BOOST_CHECK_EQUAL(tree.height(), 2);

    tree.push(20, "left-left");
    BOOST_CHECK_EQUAL(tree.height(), 3);
  }

  BOOST_AUTO_TEST_CASE(BSTreeSubtreeHeight)
  {
    BSTree< int, std::string > tree;
    tree.push(50, "root");
    tree.push(30, "left");
    tree.push(20, "left-left");
    tree.push(40, "left-right");
    tree.push(70, "right");

    auto it = tree.find(30);
    BOOST_CHECK(it != tree.end());
    BOOST_CHECK_EQUAL(tree.height(it), 2);
  }

  BOOST_AUTO_TEST_CASE(BSTreeRotateRight)
  {
    BSTree< int, std::string > tree;
    tree.push(50, "Q");
    tree.push(30, "P");
    tree.push(20, "A");
    tree.push(40, "B");
    tree.push(70, "C");

    auto it = tree.find(50);
    tree.rotateRight(it);

    BOOST_CHECK_EQUAL(tree.height(), 3);
    BOOST_CHECK(tree.has(20));
    BOOST_CHECK(tree.has(30));
    BOOST_CHECK(tree.has(40));
    BOOST_CHECK(tree.has(50));
    BOOST_CHECK(tree.has(70));
  }

  BOOST_AUTO_TEST_CASE(BSTreeRotateLeft)
  {
    BSTree< int, std::string > tree;
    tree.push(30, "P");
    tree.push(20, "A");
    tree.push(50, "Q");
    tree.push(40, "B");
    tree.push(70, "C");

    auto it = tree.find(30);
    tree.rotateLeft(it);

    BOOST_CHECK(tree.has(20));
    BOOST_CHECK(tree.has(30));
    BOOST_CHECK(tree.has(40));
    BOOST_CHECK(tree.has(50));
    BOOST_CHECK(tree.has(70));
  }

  BOOST_AUTO_TEST_CASE(BSTreeRotateLargeRight)
  {
    BSTree< int, std::string > tree;
    tree.push(50, "P");
    tree.push(30, "A");
    tree.push(70, "Q");
    tree.push(60, "B");
    tree.push(55, "D");
    tree.push(65, "E");
    tree.push(80, "C");

    auto it = tree.find(50);
    tree.rotateLargeRight(it);

    BOOST_CHECK(tree.has(30));
    BOOST_CHECK(tree.has(50));
    BOOST_CHECK(tree.has(55));
    BOOST_CHECK(tree.has(60));
    BOOST_CHECK(tree.has(65));
    BOOST_CHECK(tree.has(70));
    BOOST_CHECK(tree.has(80));
  }

  BOOST_AUTO_TEST_CASE(BSTreeRotateLargeLeft)
  {
    BSTree< int, std::string > tree;
    tree.push(50, "P");
    tree.push(30, "A");
    tree.push(80, "Q");
    tree.push(70, "B");
    tree.push(60, "D");
    tree.push(75, "E");
    tree.push(90, "C");

    auto it = tree.find(50);
    tree.rotateLargeLeft(it);

    BOOST_CHECK(tree.has(30));
    BOOST_CHECK(tree.has(50));
    BOOST_CHECK(tree.has(60));
    BOOST_CHECK(tree.has(70));
    BOOST_CHECK(tree.has(75));
    BOOST_CHECK(tree.has(80));
    BOOST_CHECK(tree.has(90));
  }

  BOOST_AUTO_TEST_CASE(BSTreeIteratorStabilityAfterRotate)
  {
    BSTree< int, std::string > tree;
    tree.push(50, "fifty");
    tree.push(30, "thirty");
    tree.push(70, "seventy");

    auto it30 = tree.find(30);
    BOOST_CHECK_EQUAL((*it30).second, "thirty");

    tree.rotateRight(tree.find(50));

    BOOST_CHECK_EQUAL((*it30).second, "thirty");
  }

  BOOST_AUTO_TEST_CASE(BSTreeClear)
  {
    BSTree< int, std::string > tree;
    tree.push(1, "one");
    tree.push(2, "two");
    tree.clear();

    BOOST_CHECK(tree.empty());
    BOOST_CHECK_EQUAL(tree.size(), 0);
  }

  BOOST_AUTO_TEST_SUITE_END()

}
