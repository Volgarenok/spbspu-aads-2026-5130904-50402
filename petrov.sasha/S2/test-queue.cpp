#include <boost/test/unit_test.hpp>
#include "queue.hpp"

BOOST_AUTO_TEST_SUITE(PetrovQueueTests)

BOOST_AUTO_TEST_CASE(test_default_constructor)
{
    petrov::Queue<int> q;
    BOOST_CHECK(q.empty());
    BOOST_CHECK_EQUAL(q.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_push_front_back)
{
    petrov::Queue<int> q;
    q.push(10);
    BOOST_CHECK(!q.empty());
    BOOST_CHECK_EQUAL(q.size(), 1);
    BOOST_CHECK_EQUAL(q.front(), 10);

    q.push(20);
    BOOST_CHECK_EQUAL(q.size(), 2);
    BOOST_CHECK_EQUAL(q.front(), 10); // front still 10
}

BOOST_AUTO_TEST_CASE(test_pop)
{
    petrov::Queue<int> q;
    q.push(1);
    q.push(2);
    q.push(3);

    q.pop();
    BOOST_CHECK_EQUAL(q.size(), 2);
    BOOST_CHECK_EQUAL(q.front(), 2);

    q.pop();
    BOOST_CHECK_EQUAL(q.size(), 1);
    BOOST_CHECK_EQUAL(q.front(), 3);

    q.pop();
    BOOST_CHECK(q.empty());
}

BOOST_AUTO_TEST_CASE(test_pop_empty_throws)
{
    petrov::Queue<int> q;
    BOOST_CHECK_THROW(q.pop(), std::logic_error);
    BOOST_CHECK_THROW(q.front(), std::logic_error);
}

BOOST_AUTO_TEST_CASE(test_multiple_operations)
{
    petrov::Queue<int> q;
    for (int i = 0; i < 100; ++i) {
        q.push(i);
        BOOST_CHECK_EQUAL(q.front(), 0);
    }
    BOOST_CHECK_EQUAL(q.size(), 100);
    for (int i = 0; i < 100; ++i) {
        BOOST_CHECK_EQUAL(q.front(), i);
        q.pop();
    }
    BOOST_CHECK(q.empty());
}

BOOST_AUTO_TEST_SUITE_END()