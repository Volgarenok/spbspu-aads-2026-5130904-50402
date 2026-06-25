#define BOOST_TEST_MODULE S4
#include <boost/test/included/unit_test.hpp>
#include "TreeImpl.hpp"

bool intComp(int a, int b)
{
  return a < b;
}

using tree_t = vishnevskiy::BSTree<int, int, bool (*)(int, int)>;

BOOST_AUTO_TEST_CASE(constructor_test)
{
  tree_t tree(intComp);
  BOOST_CHECK_EQUAL(tree.height(), 0);
}

BOOST_AUTO_TEST_CASE(push_test)
{
  tree_t tree(intComp);
  int key1 = 10;
  int val1 = 10;
  int key2 = 5;
  int val2 = 5;
  int key3 = 15;
  int val3 = 15;
  tree.push(key1, val1);
  tree.push(key2, val2);
  tree.push(key3, val3);
  BOOST_CHECK(tree.has(10));
  BOOST_CHECK(tree.has(5));
  BOOST_CHECK(tree.has(15));
  BOOST_CHECK(!tree.has(20));
}

BOOST_AUTO_TEST_CASE(get_test)
{
  tree_t tree(intComp);
  int key1 = 10;
  int val1 = 10;
  int key2 = 5;
  int val2 = 5;
  tree.push(key1, val1);
  tree.push(key2, val2);
  int result1 = tree.get(10);
  int result2 = tree.get(5);
  BOOST_CHECK_EQUAL(result1, 10);
  BOOST_CHECK_EQUAL(result2, 5);
  BOOST_CHECK_THROW(tree.get(20), std::logic_error);
}

BOOST_AUTO_TEST_CASE(drop_test)
{
  tree_t tree(intComp);
  int key1 = 10;
  int val1 = 10;
  int key2 = 5;
  int val2 = 5;
  int key3 = 15;
  int val3 = 15;
  tree.push(key1, val1);
  tree.push(key2, val2);
  tree.push(key3, val3);
  int dropped = tree.drop(5);
  BOOST_CHECK_EQUAL(dropped, 5);
  BOOST_CHECK(tree.has(10));
  BOOST_CHECK(!tree.has(5));
  BOOST_CHECK(tree.has(15));
}

BOOST_AUTO_TEST_CASE(height_test)
{
  tree_t tree(intComp);
  BOOST_CHECK_EQUAL(tree.height(), 0);
  int key1 = 10;
  int val1 = 10;
  int key2 = 5;
  int val2 = 5;
  int key3 = 15;
  int val3 = 15;
  int key4 = 3;
  int val4 = 3;
  int key5 = 7;
  int val5 = 7;
  tree.push(key1, val1);
  tree.push(key2, val2);
  tree.push(key3, val3);
  tree.push(key4, val4);
  tree.push(key5, val5);
  BOOST_CHECK_GT(tree.height(), 0);
  BOOST_CHECK_LE(tree.height(), 3);
  vishnevskiy::BSIterator<int, int> it = tree.begin();
  BOOST_CHECK_GT(tree.height(it), 0);
}

BOOST_AUTO_TEST_CASE(rotateLeft_test)
{
  tree_t tree(intComp);
  int key1 = 10;
  int val1 = 10;
  int key2 = 15;
  int val2 = 15;
  int key3 = 20;
  int val3 = 20;
  tree.push(key1, val1);
  tree.push(key2, val2);
  tree.push(key3, val3);
  vishnevskiy::BSIterator<int, int> it = tree.begin();
  it = tree.rotateLeft(it);
  BOOST_CHECK(tree.has(10));
  BOOST_CHECK(tree.has(15));
  BOOST_CHECK(tree.has(20));
}

BOOST_AUTO_TEST_CASE(rotateRight_test)
{
  tree_t tree(intComp);
  int key1 = 20;
  int val1 = 20;
  int key2 = 15;
  int val2 = 15;
  int key3 = 10;
  int val3 = 10;
  tree.push(key1, val1);
  tree.push(key2, val2);
  tree.push(key3, val3);
  vishnevskiy::BSIterator<int, int> it = tree.begin();
  it = tree.rotateRight(it);
  BOOST_CHECK(tree.has(20));
  BOOST_CHECK(tree.has(15));
  BOOST_CHECK(tree.has(10));
}
