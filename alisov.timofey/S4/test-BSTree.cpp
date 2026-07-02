#define BOOST_TEST_MODULE S4
#include <stdexcept>
#include <utility>
#include <boost/test/included/unit_test.hpp>
#include "BSTree.hpp"

BOOST_AUTO_TEST_CASE(new_tree_empty)
{
  alisov::BSTree< int, int, std::less< int > > tree;
  BOOST_TEST(tree.empty());
}

BOOST_AUTO_TEST_CASE(after_insert_tree_non_empty)
{
  alisov::BSTree< int, int, std::less< int > > tree;
  BOOST_REQUIRE(tree.empty());
  BOOST_REQUIRE(tree.insert(0, 1));
  BOOST_TEST(!tree.empty());
}

BOOST_AUTO_TEST_CASE(add_correct_element)
{
  alisov::BSTree< int, int, std::less< int > > tree;
  tree.insert(0, 1);
  BOOST_TEST(tree.at(0) == 1);
}

BOOST_AUTO_TEST_CASE(no_duplicates)
{
  alisov::BSTree< int, int, std::less< int > > tree;
  tree.insert(0, 1);
  BOOST_REQUIRE(tree.at(0) == 1);
  BOOST_TEST(!tree.insert(0, 2));
  BOOST_TEST(tree.at(0) == 1);
}

BOOST_AUTO_TEST_CASE(at_undefined_key)
{
  alisov::BSTree< int, int, std::less< int > > tree;
  tree.insert(0, 1);
  tree.insert(5, 10);
  tree.insert(100, 50);
  BOOST_CHECK_THROW(tree.at(3), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(contains_empty)
{
  alisov::BSTree< int, int, std::less< int > > tree;
  BOOST_TEST(!tree.contains(0));
}

BOOST_AUTO_TEST_CASE(contains_true)
{
  alisov::BSTree< int, int, std::less< int > > tree;
  tree.insert(0, 1);
  BOOST_TEST(tree.contains(0));
}

BOOST_AUTO_TEST_CASE(contains_false)
{
  alisov::BSTree< int, int, std::less< int > > tree;
  tree.insert(1, 2);
  BOOST_TEST(!tree.contains(0));
}

BOOST_AUTO_TEST_CASE(empty_iterators)
{
  alisov::BSTree< int, int, std::less< int > > tree;
  BOOST_CHECK(tree.begin() == tree.end());
}

BOOST_AUTO_TEST_CASE(iterator_one)
{
  alisov::BSTree< int, int, std::less< int > > tree;
  tree.insert(0, 1);
  BOOST_CHECK(tree.begin() != tree.end());
  auto it = tree.begin();
  BOOST_CHECK((*it).first == 0 && (*it).second == 1);
  (*it).second = 2;
  it = tree.begin();
  BOOST_CHECK((*it).first == 0 && (*it).second == 2);
}

BOOST_AUTO_TEST_CASE(iterator_many)
{
  alisov::BSTree< int, int, std::less< int > > tree;
  tree.insert(0, 1);
  tree.insert(12, 34);
  tree.insert(5, 10);
  auto it = tree.begin();
  BOOST_CHECK((*it).first == 0 && (*it).second == 1);
  ++it;
  BOOST_CHECK((*it).first == 5 && (*it).second == 10);
  ++it;
  BOOST_CHECK((*it).first == 12 && (*it).second == 34);
  ++it;
  BOOST_CHECK(it == tree.end());
}

BOOST_AUTO_TEST_CASE(move_constructor)
{
  alisov::BSTree< int, int, std::less< int > > t1;
  t1.insert(10, 1);
  t1.insert(5, 2);
  t1.insert(15, 3);
  alisov::BSTree< int, int, std::less< int > > t2(std::move(t1));
  BOOST_TEST(t2.at(10) == 1);
  BOOST_TEST(t2.at(5) == 2);
  BOOST_TEST(t2.at(15) == 3);
}

BOOST_AUTO_TEST_CASE(get_elem)
{
  alisov::BSTree< int, int, std::less< int > > t1;
  t1.insert(10, 1);
  t1.insert(5, 2);
  t1.insert(15, 3);
  BOOST_TEST(t1[10] == 1);
  BOOST_TEST(t1[5] == 2);
  BOOST_TEST(t1[15] == 3);
}

BOOST_AUTO_TEST_CASE(move_assignment_operator)
{
  alisov::BSTree< int, int, std::less< int > > t1;
  t1.insert(10, 1);
  t1.insert(5, 2);
  t1.insert(15, 3);
  alisov::BSTree< int, int, std::less< int > > t2;
  t2.insert(11, 4);
  t2.insert(6, 5);
  t2.insert(16, 6);
  t2 = std::move(t1);
  BOOST_TEST(t2.at(10) == 1);
  BOOST_TEST(t2.at(5) == 2);
  BOOST_TEST(t2.at(15) == 3);
  BOOST_TEST(!t2.contains(11));
  BOOST_TEST(!t2.contains(6));
  BOOST_TEST(!t2.contains(16));
}

BOOST_AUTO_TEST_CASE(height_test)
{
  alisov::BSTree< int, int, std::less< int > > tree;
  tree.insert(10, 1);
  tree.insert(5, 2);
  tree.insert(6, 5);
  auto it = tree.cbegin();
  BOOST_CHECK((*it).first == 5 && tree.height(it) == 2);
  ++it;
  BOOST_CHECK((*it).first == 6 && tree.height(it) == 1);
  ++it;
  BOOST_CHECK((*it).first == 10 && tree.height(it) == 3);
}

BOOST_AUTO_TEST_CASE(copy_constructor_empty)
{
  alisov::BSTree< int, int, std::less< int > > t1;
  BOOST_REQUIRE(t1.empty());
  alisov::BSTree< int, int, std::less< int > > t2(t1);
  BOOST_TEST(t2.empty());
}

BOOST_AUTO_TEST_CASE(copy_constructor_many)
{
  alisov::BSTree< int, int, std::less< int > > t1;
  t1.insert(10, 1);
  t1.insert(5, 2);
  t1.insert(15, 3);
  t1.insert(11, 4);
  t1.insert(6, 5);
  t1.insert(16, 6);
  alisov::BSTree< int, int, std::less< int > > t2(t1);
  BOOST_TEST(!t2.empty());
  BOOST_TEST(t2.at(10) == 1);
  BOOST_TEST(t2.at(5) == 2);
  BOOST_TEST(t2.at(15) == 3);
  BOOST_TEST(t2.at(11) == 4);
  BOOST_TEST(t2.at(6) == 5);
  BOOST_TEST(t2.at(16) == 6);
}

BOOST_AUTO_TEST_CASE(copy_assignment)
{
  alisov::BSTree< int, int, std::less< int > > t1;
  t1.insert(10, 1);
  t1.insert(5, 2);
  t1.insert(15, 3);
  alisov::BSTree< int, int, std::less< int > > t2;
  t2.insert(11, 4);
  t2.insert(6, 5);
  t2.insert(16, 6);
  t2 = t1;
  BOOST_TEST(t2.at(10) == 1);
  BOOST_TEST(t2.at(5) == 2);
  BOOST_TEST(t2.at(15) == 3);
  BOOST_TEST(!t2.contains(11));
  BOOST_TEST(!t2.contains(6));
  BOOST_TEST(!t2.contains(16));
}

BOOST_AUTO_TEST_CASE(self_swap)
{
  alisov::BSTree< int, int, std::less< int > > tree;
  tree.insert(10, 1);
  tree.insert(5, 2);
  tree.insert(15, 3);
  tree.swap(tree);
  BOOST_TEST(tree.at(10) == 1);
  BOOST_TEST(tree.at(5) == 2);
  BOOST_TEST(tree.at(15) == 3);
}

BOOST_AUTO_TEST_CASE(swap_trees)
{
  alisov::BSTree< int, int, std::less< int > > t1;
  t1.insert(10, 1);
  t1.insert(5, 2);
  t1.insert(15, 3);
  alisov::BSTree< int, int, std::less< int > > t2;
  t2.insert(11, 4);
  t2.insert(6, 5);
  t2.insert(16, 6);
  t1.swap(t2);
  BOOST_TEST(t1.at(11) == 4);
  BOOST_TEST(t1.at(6) == 5);
  BOOST_TEST(t1.at(16) == 6);
  BOOST_TEST(t2.at(10) == 1);
  BOOST_TEST(t2.at(5) == 2);
  BOOST_TEST(t2.at(15) == 3);
}

BOOST_AUTO_TEST_CASE(erase_element)
{
  alisov::BSTree< int, int, std::less< int > > t1;
  t1.insert(10, 1);
  t1.insert(5, 2);
  t1.insert(15, 3);
  BOOST_TEST(t1.erase(10));
  BOOST_TEST(!t1.contains(10));
}

BOOST_AUTO_TEST_CASE(erase_non_existent)
{
  alisov::BSTree< int, int, std::less< int > > t1;
  t1.insert(10, 1);
  t1.insert(5, 2);
  t1.insert(15, 3);
  BOOST_TEST(!t1.erase(11));
}

BOOST_AUTO_TEST_CASE(rotate_left)
{
  alisov::BSTree< int, int, std::less< int > > tree;
  tree.insert(10, 1);
  tree.insert(5, 2);
  tree.insert(15, 3);
  tree.insert(13, 4);
  tree.insert(18, 5);
  auto it = tree.begin();
  size_t t1Height = tree.height(it);
  size_t xHeight = tree.height(++it);
  size_t t2Height = tree.height(++it);
  size_t yHeight = tree.height(++it);
  size_t t3Height = tree.height(++it);
  it = tree.begin();
  ++it;
  ++it;
  ++it;
  BOOST_REQUIRE((*it).first == 15);
  it = tree.rotateLeft(it);
  BOOST_TEST((*it).first == 13);
  it = tree.begin();
  BOOST_TEST(tree.height(it) == t1Height);
  ++it;
  BOOST_TEST(xHeight - tree.height(it) == 1);
  ++it;
  BOOST_TEST(tree.height(it) == t2Height);
  ++it;
  BOOST_TEST(tree.height(it) - yHeight == 1);
  ++it;
  BOOST_TEST(tree.height(it) == t3Height);
}

BOOST_AUTO_TEST_CASE(rotate_right)
{
  alisov::BSTree< int, int, std::less< int > > tree;
  tree.insert(10, 1);
  tree.insert(5, 2);
  tree.insert(15, 3);
  tree.insert(3, 4);
  tree.insert(8, 5);
  auto it = tree.begin();
  size_t t1Height = tree.height(it);
  size_t xHeight = tree.height(++it);
  size_t t2Height = tree.height(++it);
  size_t yHeight = tree.height(++it);
  size_t t3Height = tree.height(++it);
  it = tree.begin();
  ++it;
  BOOST_REQUIRE((*it).first == 5);
  it = tree.rotateRight(it);
  BOOST_TEST((*it).first == 8);
  it = tree.begin();
  BOOST_TEST(tree.height(it) == t1Height);
  ++it;
  BOOST_TEST(tree.height(it) - xHeight == 1);
  ++it;
  BOOST_TEST(tree.height(it) == t2Height);
  ++it;
  BOOST_TEST(yHeight - tree.height(it) == 1);
  ++it;
  BOOST_TEST(tree.height(it) == t3Height);
}

BOOST_AUTO_TEST_CASE(rotate_large_left)
{
  alisov::BSTree< int, int, std::less< int > > tree;
  tree[10] = 1;
  tree[5] = 2;
  tree[15] = 3;
  tree[13] = 4;
  tree[18] = 5;
  tree[12] = 6;
  tree[14] = 7;
  auto it = tree.begin();
  size_t t1Height = tree.height(it);
  size_t xHeight = tree.height(++it);
  size_t t2Height = tree.height(++it);
  size_t yHeight = tree.height(++it);
  size_t t3Height = tree.height(++it);
  size_t zHeight = tree.height(++it);
  size_t t4Height = tree.height(++it);
  it = tree.begin();
  ++it;
  ++it;
  ++it;
  BOOST_REQUIRE((*it).first == 13);
  it = tree.rotateLargeLeft(it);
  BOOST_TEST((*it).first == 14);
  it = tree.begin();
  BOOST_TEST(tree.height(it) == t1Height);
  ++it;
  BOOST_TEST(xHeight - tree.height(it) == 2);
  ++it;
  BOOST_TEST(tree.height(it) == t2Height);
  ++it;
  BOOST_TEST(tree.height(it) - yHeight == 1);
  ++it;
  BOOST_TEST(tree.height(it) == t3Height);
  ++it;
  BOOST_TEST(zHeight - tree.height(it) == 1);
  ++it;
  BOOST_TEST(tree.height(it) == t4Height);
}

BOOST_AUTO_TEST_CASE(rotate_large_right)
{
  alisov::BSTree< int, int, std::less< int > > tree;
  tree[10] = 1;
  tree[5] = 2;
  tree[15] = 3;
  tree[3] = 4;
  tree[8] = 5;
  tree[7] = 6;
  tree[9] = 7;
  auto it = tree.begin();
  size_t t1Height = tree.height(it);
  size_t xHeight = tree.height(++it);
  size_t t2Height = tree.height(++it);
  size_t yHeight = tree.height(++it);
  size_t t3Height = tree.height(++it);
  size_t zHeight = tree.height(++it);
  size_t t4Height = tree.height(++it);
  it = tree.begin();
  ++it;
  ++it;
  ++it;
  BOOST_REQUIRE((*it).first == 8);
  it = tree.rotateLargeRight(it);
  BOOST_TEST((*it).first == 7);
  it = tree.begin();
  BOOST_TEST(tree.height(it) == t1Height);
  ++it;
  BOOST_TEST(tree.height(it) - xHeight == 1);
  ++it;
  BOOST_TEST(tree.height(it) == t2Height);
  ++it;
  BOOST_TEST(tree.height(it) - yHeight == 1);
  ++it;
  BOOST_TEST(tree.height(it) == t3Height);
  ++it;
  BOOST_TEST(zHeight - tree.height(it) == 2);
  ++it;
  BOOST_TEST(tree.height(it) == t4Height);
}
