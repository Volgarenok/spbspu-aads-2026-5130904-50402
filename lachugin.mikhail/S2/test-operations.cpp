#define BOOST_TEST_MODULE S2
#include <boost/test/included/unit_test.hpp>
#include <string>
#include "operations.hpp"

BOOST_AUTO_TEST_SUITE(LachuginOperationsSafeTests)

BOOST_AUTO_TEST_CASE(test_sum_basic)
{
    long long a = 2, b = 3;
    BOOST_CHECK_EQUAL(lachugin::sum(a, b), 5);
}

BOOST_AUTO_TEST_CASE(test_subtraction_basic)
{
    long long a = 5, b = 3;
    BOOST_CHECK_EQUAL(lachugin::subtraction(a, b), 2);
}

BOOST_AUTO_TEST_CASE(test_multiply_basic)
{
    long long a = 4, b = 5;
    BOOST_CHECK_EQUAL(lachugin::multiply(a, b), 20);
}

BOOST_AUTO_TEST_CASE(test_multiply_negative)
{
    long long a = -4, b = 5;
    BOOST_CHECK_EQUAL(lachugin::multiply(a, b), -20);
}

BOOST_AUTO_TEST_CASE(test_divide_basic)
{
    long long a = 10, b = 2;
    BOOST_CHECK_EQUAL(lachugin::divide(a, b), 5);
}

BOOST_AUTO_TEST_CASE(test_mod_basic)
{
    long long a = 7, b = 3;
    BOOST_CHECK_EQUAL(lachugin::mod(a, b), 1);
}

BOOST_AUTO_TEST_CASE(test_mod_negative_result)
{
    long long a = -7, b = 3;
    BOOST_CHECK_EQUAL(lachugin::mod(a, b), 2);
}

BOOST_AUTO_TEST_CASE(test_reversal_basic)
{
    std::string s = "123";
    lachugin::reversal(s);
    BOOST_CHECK_EQUAL(s, "321");
}

BOOST_AUTO_TEST_CASE(test_isPriority_basic)
{
    BOOST_CHECK(lachugin::isPriority("*", "+"));
    BOOST_CHECK(lachugin::isPriority("+", "+"));
    BOOST_CHECK(!lachugin::isPriority("+", "*"));
}

BOOST_AUTO_TEST_CASE(test_operations_add)
{
    BOOST_CHECK_EQUAL(lachugin::operations("+", 2, 3), 5);
}

BOOST_AUTO_TEST_CASE(test_operations_sub)
{
    BOOST_CHECK_EQUAL(lachugin::operations("-", 5, 3), 2);
}

BOOST_AUTO_TEST_CASE(test_operations_mul)
{
    BOOST_CHECK_EQUAL(lachugin::operations("*", 4, 3), 12);
}

BOOST_AUTO_TEST_CASE(test_operations_div)
{
    BOOST_CHECK_EQUAL(lachugin::operations("/", 8, 2), 4);
}

BOOST_AUTO_TEST_CASE(test_operations_mod)
{
    BOOST_CHECK_EQUAL(lachugin::operations("%", 7, 3), 1);
}

BOOST_AUTO_TEST_SUITE_END()
