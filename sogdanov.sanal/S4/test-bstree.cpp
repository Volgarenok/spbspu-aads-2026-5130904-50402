#include <boost/test/unit_test.hpp>
#include <stdexcept>
#include "bstree.hpp"

using namespace sogdanov;

BOOST_AUTO_TEST_SUITE(BSTreeTests)

BOOST_AUTO_TEST_CASE(test_default_constructor)
{
    BSTree< int, int > tree;
    BOOST_CHECK(tree.empty());
    BOOST_CHECK(tree.begin() == tree.end());
}

BOOST_AUTO_TEST_CASE(test_push)
{
    BSTree< int, int > tree;
    tree.push(1, 10);
    BOOST_CHECK(!tree.empty());
    BOOST_CHECK_EQUAL(tree.get(1), 10);

    tree.push(2, 20);
    tree.push(3, 30);
    BOOST_CHECK_EQUAL(tree.get(2), 20);
    BOOST_CHECK_EQUAL(tree.get(3), 30);
}

BOOST_AUTO_TEST_CASE(test_get_not_found)
{
    BSTree< int, int > tree;
    tree.push(1, 10);
    BOOST_CHECK_THROW(tree.get(2), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(test_push_update_existing)
{
    BSTree< int, int > tree;
    tree.push(1, 10);
    tree.push(1, 100);

    BOOST_CHECK_EQUAL(tree.get(1), 100);
}

BOOST_AUTO_TEST_CASE(test_modify_value)
{
    BSTree< int, int > tree;
    tree.push(1, 10);
    BSTIterator<int, int> it = tree.find(1);
    BOOST_REQUIRE(it != tree.end());
    (*it).second = 100;
    
    BOOST_CHECK_EQUAL(tree.get(1), 100);
}

BOOST_AUTO_TEST_CASE(test_drop)
{
    BSTree< int, int > tree;
    tree.push(1, 10);
    tree.push(2, 20);
    tree.push(3, 30);

    int val = tree.drop(2);
    BOOST_CHECK_EQUAL(val, 20);
    BOOST_CHECK_EQUAL(tree.get(1), 10);
    BOOST_CHECK_EQUAL(tree.get(3), 30);
    BOOST_CHECK_THROW(tree.get(2), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(test_drop_root_with_two_children)
{
    BSTree< int, int > tree;
    tree.push(2, 20);
    tree.push(1, 10);
    tree.push(3, 30);

    tree.drop(2);
    BOOST_CHECK_EQUAL(tree.get(1), 10);
    BOOST_CHECK_EQUAL(tree.get(3), 30);
    BOOST_CHECK_THROW(tree.get(2), std::out_of_range);
    BOOST_CHECK_EQUAL(tree.height(), 2);
}

BOOST_AUTO_TEST_CASE(test_copy_constructor)
{
    BSTree< int, int > tree1;
    tree1.push(1, 10);
    tree1.push(2, 20);
    tree1.push(3, 30);

    BSTree< int, int > tree2(tree1);

    BOOST_CHECK_EQUAL(tree2.get(1), 10);
    BOOST_CHECK_EQUAL(tree2.get(2), 20);
    BOOST_CHECK_EQUAL(tree2.get(3), 30);

    BSTIterator<int, int> it = tree2.find(1);
    (*it).second = 100;
    
    BOOST_CHECK_EQUAL(tree1.get(1), 10);
    BOOST_CHECK_EQUAL(tree2.get(1), 100);
}

BOOST_AUTO_TEST_CASE(test_copy_assignment)
{
    BSTree< int, int > tree1;
    tree1.push(1, 10);

    BSTree< int, int > tree2;
    tree2.push(99, 990);

    tree2 = tree1;

    BOOST_CHECK_EQUAL(tree2.get(1), 10);
    BOOST_CHECK_THROW(tree2.get(99), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(test_iterator_traversal)
{
    BSTree< int, int > tree;
    tree.push(3, 30);
    tree.push(1, 10);
    tree.push(2, 20);
    tree.push(4, 40);

    BSTIterator<int, int> it = tree.begin();
    
    BOOST_REQUIRE(it != tree.end());
    BOOST_CHECK_EQUAL((*it).first, 1);
    
    ++it;
    BOOST_REQUIRE(it != tree.end());
    BOOST_CHECK_EQUAL((*it).first, 2);
    
    ++it;
    BOOST_REQUIRE(it != tree.end());
    BOOST_CHECK_EQUAL((*it).first, 3);
    
    ++it;
    BOOST_REQUIRE(it != tree.end());
    BOOST_CHECK_EQUAL((*it).first, 4);
    
    ++it;
    BOOST_CHECK(it == tree.end());
}

BOOST_AUTO_TEST_CASE(test_const_iterator)
{
    BSTree< int, int > tree;
    tree.push(1, 10);
    
    const BSTree< int, int >& ctree = tree;
    BSTConstIterator<int, int> it = ctree.begin();
    
    BOOST_CHECK(it != ctree.end());
    BOOST_CHECK_EQUAL((*it).first, 1);
}

BOOST_AUTO_TEST_CASE(test_iterator_modification)
{
    BSTree< int, int > tree;
    tree.push(1, 10);
    tree.push(2, 20);

    for (BSTIterator<int, int> it = tree.begin(); it != tree.end(); ++it)
    {
        if ((*it).first == 1)
        {
            (*it).second = 100;
        }
    }
    BOOST_CHECK_EQUAL(tree.get(1), 100);
}

BOOST_AUTO_TEST_CASE(test_height_empty)
{
    BSTree< int, int > tree;
    BOOST_CHECK_EQUAL(tree.height(), 0);
}

BOOST_AUTO_TEST_CASE(test_height_single)
{
    BSTree< int, int > tree;
    tree.push(1, 10);
    BOOST_CHECK_EQUAL(tree.height(), 1);
}

BOOST_AUTO_TEST_CASE(test_height_balanced)
{
    BSTree< int, int > tree;
    tree.push(2, 20);
    tree.push(1, 10);
    tree.push(3, 30);
    BOOST_CHECK_EQUAL(tree.height(), 2);
}

BOOST_AUTO_TEST_CASE(test_height_degenerate)
{
    BSTree< int, int > tree;
    tree.push(1, 10);
    tree.push(2, 20);
    tree.push(3, 30);
    tree.push(4, 40);
    BOOST_CHECK_EQUAL(tree.height(), 4);
}

BOOST_AUTO_TEST_CASE(test_rotate_left)
{
    BSTree< int, int > tree;
    tree.push(1, 10);
    tree.push(2, 20);
    tree.push(3, 30);

    BOOST_CHECK_EQUAL(tree.height(), 3);

    BSTConstIterator<int, int> it = tree.find(2);
    tree.rotateLeft(it);

    BOOST_CHECK_EQUAL(tree.height(), 2);
    BOOST_CHECK_EQUAL(tree.get(1), 10);
    BOOST_CHECK_EQUAL(tree.get(2), 20);
    BOOST_CHECK_EQUAL(tree.get(3), 30);
}

BOOST_AUTO_TEST_CASE(test_rotate_right)
{
    BSTree< int, int > tree;
    tree.push(3, 30);
    tree.push(2, 20);
    tree.push(1, 10);

    BOOST_CHECK_EQUAL(tree.height(), 3);

    BSTConstIterator<int, int> it = tree.find(2);
    tree.rotateRight(it);

    BOOST_CHECK_EQUAL(tree.height(), 2);
    BOOST_CHECK_EQUAL(tree.get(1), 10);
    BOOST_CHECK_EQUAL(tree.get(2), 20);
    BOOST_CHECK_EQUAL(tree.get(3), 30);
}

BOOST_AUTO_TEST_CASE(test_rotate_large_left)
{
    BSTree< int, int > tree;
    tree.push(1, 10);
    tree.push(3, 30);
    tree.push(2, 20);

    BOOST_CHECK_EQUAL(tree.height(), 3);

    BSTConstIterator<int, int> it = tree.find(2);
    tree.rotateLargeLeft(it);

    BOOST_CHECK_EQUAL(tree.height(), 2);
    BOOST_CHECK_EQUAL(tree.get(1), 10);
    BOOST_CHECK_EQUAL(tree.get(2), 20);
    BOOST_CHECK_EQUAL(tree.get(3), 30);
}

BOOST_AUTO_TEST_CASE(test_rotate_large_right)
{
    BSTree< int, int > tree;
    tree.push(3, 30);
    tree.push(1, 10);
    tree.push(2, 20);

    BOOST_CHECK_EQUAL(tree.height(), 3);

    BSTConstIterator<int, int> it = tree.find(2);
    tree.rotateLargeRight(it);

    BOOST_CHECK_EQUAL(tree.height(), 2);
    BOOST_CHECK_EQUAL(tree.get(1), 10);
    BOOST_CHECK_EQUAL(tree.get(2), 20);
    BOOST_CHECK_EQUAL(tree.get(3), 30);
}

BOOST_AUTO_TEST_CASE(test_iterator_equality)
{
    BSTree< int, int > tree;
    tree.push(1, 10);

    BSTIterator<int, int> it1 = tree.begin();
    BSTIterator<int, int> it2 = tree.begin();
    BSTIterator<int, int> it3 = tree.end();

    BOOST_CHECK(it1 == it2);
    BOOST_CHECK(it1 != it3);
    BOOST_CHECK(tree.begin() != tree.end());
}

BOOST_AUTO_TEST_SUITE_END()
