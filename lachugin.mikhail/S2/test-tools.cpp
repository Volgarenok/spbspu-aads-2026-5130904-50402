#define BOOST_TEST_MODULE S2
#include <boost/test/included/unit_test.hpp>
#include <sstream>
#include <string>
#include "tools.hpp"
#include "queue.hpp"
#include "stack.hpp"

BOOST_AUTO_TEST_SUITE(LachuginToolsSafeTests)

BOOST_AUTO_TEST_CASE(test_reedInput_basic)
{
    std::istringstream input("10 20 30");
    lachugin::Queue< std::string > q;

    lachugin::reedInput(input, q);

    BOOST_REQUIRE_EQUAL(q.size(), 3);

    BOOST_CHECK_EQUAL(q.front(), "10"); q.pop();
    BOOST_CHECK_EQUAL(q.front(), "20"); q.pop();
    BOOST_CHECK_EQUAL(q.front(), "30");
}

BOOST_AUTO_TEST_CASE(test_isOperator_basic)
{
    BOOST_CHECK(lachugin::isOperator("+"));
    BOOST_CHECK(lachugin::isOperator("-"));
    BOOST_CHECK(lachugin::isOperator("*"));
    BOOST_CHECK(lachugin::isOperator("/"));
    BOOST_CHECK(lachugin::isOperator("%"));

    BOOST_CHECK(!lachugin::isOperator("123"));
    BOOST_CHECK(!lachugin::isOperator("("));
}

BOOST_AUTO_TEST_CASE(test_infix_to_postfix_simple)
{
    std::istringstream input("2 + 3 * 4");

    lachugin::Queue< std::string > q;
    lachugin::reedInput(input, q);

    lachugin::Queue< std::string > postfix = lachugin::infixToPostfix(q);

    BOOST_REQUIRE_EQUAL(postfix.size(), 5);

    BOOST_CHECK_EQUAL(postfix.front(), "2"); postfix.pop();
    BOOST_CHECK_EQUAL(postfix.front(), "3"); postfix.pop();
    BOOST_CHECK_EQUAL(postfix.front(), "4"); postfix.pop();
    BOOST_CHECK_EQUAL(postfix.front(), "*"); postfix.pop();
    BOOST_CHECK_EQUAL(postfix.front(), "+");
}

BOOST_AUTO_TEST_CASE(test_infix_to_postfix_parentheses_safe)
{
    std::istringstream input("( 2 + 3 )");

    lachugin::Queue< std::string > q;
    lachugin::reedInput(input, q);

    lachugin::Queue< std::string > postfix = lachugin::infixToPostfix(q);

    BOOST_REQUIRE_EQUAL(postfix.size(), 3);

    BOOST_CHECK_EQUAL(postfix.front(), "2"); postfix.pop();
    BOOST_CHECK_EQUAL(postfix.front(), "3"); postfix.pop();
    BOOST_CHECK_EQUAL(postfix.front(), "+");
}

BOOST_AUTO_TEST_CASE(test_counting_addition)
{
    lachugin::Queue< std::string > postfix;
    postfix.push("2");
    postfix.push("3");
    postfix.push("+");

    lachugin::Stack< long long > values;
    lachugin::counting(postfix, values);

    BOOST_REQUIRE(!values.empty());
    BOOST_CHECK_EQUAL(values.top(), 5);
}

BOOST_AUTO_TEST_CASE(test_counting_multiple_operations)
{
    lachugin::Queue< std::string > postfix;
    postfix.push("2");
    postfix.push("3");
    postfix.push("+");
    postfix.push("4");
    postfix.push("*");

    lachugin::Stack< long long > values;
    lachugin::counting(postfix, values);

    BOOST_REQUIRE(!values.empty());
    BOOST_CHECK_EQUAL(values.top(), 20);
}

BOOST_AUTO_TEST_CASE(test_counting_division_safe)
{
    lachugin::Queue< std::string > postfix;
    postfix.push("8");
    postfix.push("2");
    postfix.push("/");

    lachugin::Stack< long long > values;
    lachugin::counting(postfix, values);

    BOOST_CHECK_EQUAL(values.top(), 4);
}

BOOST_AUTO_TEST_CASE(test_full_pipeline_with_reversal)
{
    std::istringstream input("# 12 + 3");

    lachugin::Queue< std::string > q;
    lachugin::reedInput(input, q);

    lachugin::Queue< std::string > postfix = lachugin::infixToPostfix(q);

    lachugin::Stack< long long > values;
    lachugin::counting(postfix, values);

    BOOST_CHECK_EQUAL(values.top(), 24);
}

BOOST_AUTO_TEST_SUITE_END()
