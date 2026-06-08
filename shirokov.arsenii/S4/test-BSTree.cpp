#define BOOST_TEST_MODULE S4
#include <stdexcept>
#include <utility>
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

BOOST_AUTO_TEST_CASE(insert_add_correct_elem)
{
  shirokov::BSTree< int, int, std::less< int > > tree;
  tree.insert(0, 1);
  BOOST_TEST(tree.at(0) == 1);
}

BOOST_AUTO_TEST_CASE(adding_an_existing_element_does_not_change_anything)
{
  shirokov::BSTree< int, int, std::less< int > > tree;
  tree.insert(0, 1);
  BOOST_REQUIRE(tree.at(0) == 1);
  BOOST_TEST(!tree.insert(0, 2));
  BOOST_TEST(tree.at(0) == 1);
}

BOOST_AUTO_TEST_CASE(insert_many_elems)
{
  shirokov::BSTree< int, int, std::less< int > > tree;
  tree.insert(0, 1);
  tree.insert(5, 10);
  tree.insert(100, 50);
  BOOST_TEST(tree.at(0) == 1);
  BOOST_TEST(tree.at(5) == 10);
  BOOST_TEST(tree.at(100) == 50);
}

BOOST_AUTO_TEST_CASE(at_undefined_key)
{
  shirokov::BSTree< int, int, std::less< int > > tree;
  tree.insert(0, 1);
  tree.insert(5, 10);
  tree.insert(100, 50);
  BOOST_CHECK_THROW(tree.at(3), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(contains_in_empty_table_return_false)
{
  shirokov::BSTree< int, int, std::less< int > > tree;
  BOOST_TEST(!tree.contains(0));
}

BOOST_AUTO_TEST_CASE(contains_existing_key_return_true)
{
  shirokov::BSTree< int, int, std::less< int > > tree;
  tree.insert(0, 1);
  BOOST_TEST(tree.contains(0));
}

BOOST_AUTO_TEST_CASE(contains_not_existing_key_return_false)
{
  shirokov::BSTree< int, int, std::less< int > > tree;
  tree.insert(1, 2);
  BOOST_TEST(!tree.contains(0));
}

BOOST_AUTO_TEST_CASE(begin_iter_in_empty_tree_equal_end_iter)
{
  shirokov::BSTree< int, int, std::less< int > > tree;
  BOOST_CHECK(tree.begin() == tree.end());
}

BOOST_AUTO_TEST_CASE(begin_iter_in_tree_with_one_elem)
{
  shirokov::BSTree< int, int, std::less< int > > tree;
  tree.insert(0, 1);
  BOOST_CHECK(tree.begin() != tree.end());
  auto it = tree.begin();
  BOOST_CHECK((*it).first == 0 && (*it).second == 1);
  (*it).second = 2;
  it = tree.begin();
  BOOST_CHECK((*it).first == 0 && (*it).second == 2);
}

BOOST_AUTO_TEST_CASE(begin_iter_in_tree_with_many_elem)
{
  shirokov::BSTree< int, int, std::less< int > > tree;
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
  shirokov::BSTree< int, int, std::less< int > > t1;
  t1.insert(10, 1);
  t1.insert(5, 2);
  t1.insert(15, 3);
  shirokov::BSTree< int, int, std::less< int > > t2(std::move(t1));
  BOOST_TEST(t2.at(10) == 1);
  BOOST_TEST(t2.at(5) == 2);
  BOOST_TEST(t2.at(15) == 3);
}

BOOST_AUTO_TEST_CASE(get_elem)
{
  shirokov::BSTree< int, int, std::less< int > > t1;
  t1.insert(10, 1);
  t1.insert(5, 2);
  t1.insert(15, 3);
  BOOST_TEST(t1[10] == 1);
  BOOST_TEST(t1[5] == 2);
  BOOST_TEST(t1[15] == 3);
}

BOOST_AUTO_TEST_CASE(move_assignment_operator)
{
  shirokov::BSTree< int, int, std::less< int > > t1;
  t1.insert(10, 1);
  t1.insert(5, 2);
  t1.insert(15, 3);
  shirokov::BSTree< int, int, std::less< int > > t2;
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
  shirokov::BSTree< int, int, std::less< int > > tree;
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

BOOST_AUTO_TEST_CASE(copy_constructor_empty_tree)
{
  shirokov::BSTree< int, int, std::less< int > > t1;
  BOOST_REQUIRE(t1.empty());
  shirokov::BSTree< int, int, std::less< int > > t2(t1);
  BOOST_TEST(t2.empty());
}

BOOST_AUTO_TEST_CASE(copy_constructor_tree_has_many_elem)
{
  shirokov::BSTree< int, int, std::less< int > > t1;
  t1.insert(10, 1);
  t1.insert(5, 2);
  t1.insert(15, 3);
  t1.insert(11, 4);
  t1.insert(6, 5);
  t1.insert(16, 6);
  shirokov::BSTree< int, int, std::less< int > > t2(t1);
  BOOST_TEST(!t2.empty());
  BOOST_TEST(t2.at(10) == 1);
  BOOST_TEST(t2.at(5) == 2);
  BOOST_TEST(t2.at(15) == 3);
  BOOST_TEST(t2.at(11) == 4);
  BOOST_TEST(t2.at(6) == 5);
  BOOST_TEST(t2.at(16) == 6);
}

BOOST_AUTO_TEST_CASE(copy_assignment_operator_test)
{
  shirokov::BSTree< int, int, std::less< int > > t1;
  t1.insert(10, 1);
  t1.insert(5, 2);
  t1.insert(15, 3);
  shirokov::BSTree< int, int, std::less< int > > t2;
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
