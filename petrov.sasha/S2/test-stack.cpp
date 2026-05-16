#include <boost/test/unit_test.hpp>
#include "stack.hpp"

BOOST_AUTO_TEST_SUITE(PetrovStackTests)

BOOST_AUTO_TEST_CASE(test_default_constructor)
{
    petrov::Stack<int> s;
    BOOST_CHECK(s.empty());
    BOOST_CHECK_EQUAL(s.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_push_top)
{
    petrov::Stack<int> s;
    s.push(10);
    BOOST_CHECK(!s.empty());
    BOOST_CHECK_EQUAL(s.size(), 1);
    BOOST_CHECK_EQUAL(s.top(), 10);

    s.push(20);
    BOOST_CHECK_EQUAL(s.size(), 2);
    BOOST_CHECK_EQUAL(s.top(), 20);
}

BOOST_AUTO_TEST_CASE(test_pop)
{
    petrov::Stack<int> s;
    s.push(1);
    s.push(2);
    s.push(3);

    s.pop();
    BOOST_CHECK_EQUAL(s.size(), 2);
    BOOST_CHECK_EQUAL(s.top(), 2);

    s.pop();
    BOOST_CHECK_EQUAL(s.size(), 1);
    BOOST_CHECK_EQUAL(s.top(), 1);

    s.pop();
    BOOST_CHECK(s.empty());
}

BOOST_AUTO_TEST_CASE(test_pop_empty_throws)
{
    petrov::Stack<int> s;
    BOOST_CHECK_THROW(s.pop(), std::logic_error);
    BOOST_CHECK_THROW(s.top(), std::logic_error);
}

BOOST_AUTO_TEST_CASE(test_multiple_operations)
{
    petrov::Stack<int> s;
    for (int i = 0; i < 100; ++i) {
        s.push(i);
        BOOST_CHECK_EQUAL(s.top(), i);
    }
    BOOST_CHECK_EQUAL(s.size(), 100);
    for (int i = 99; i >= 0; --i) {
        BOOST_CHECK_EQUAL(s.top(), i);
        s.pop();
    }
    BOOST_CHECK(s.empty());
}

BOOST_AUTO_TEST_SUITE_END()

