#include <boost/test/unit_test.hpp>
#include <boost/test/data/test_case.hpp>
#include <boost/test/data/monomorphic.hpp>
#include <sstream>
#include <string>
#include "expression.hpp"

namespace bdata = boost::unit_test::data;

BOOST_AUTO_TEST_SUITE(PetrovExpressionTests)

BOOST_AUTO_TEST_CASE(test_tokenize_simple)
{
    std::string line = "1 + 2 * 3";
    petrov::Queue<std::string> tokens = petrov::tokenize(line);
    BOOST_REQUIRE_EQUAL(tokens.size(), 5);
    BOOST_CHECK_EQUAL(tokens.front(), "1"); tokens.pop();
    BOOST_CHECK_EQUAL(tokens.front(), "+"); tokens.pop();
    BOOST_CHECK_EQUAL(tokens.front(), "2"); tokens.pop();
    BOOST_CHECK_EQUAL(tokens.front(), "*"); tokens.pop();
    BOOST_CHECK_EQUAL(tokens.front(), "3");
}

BOOST_AUTO_TEST_CASE(test_tokenize_multiple_spaces)
{
    std::string line = "  (  1 + 2 )  ";
    petrov::Queue<std::string> tokens = petrov::tokenize(line);
    BOOST_REQUIRE_EQUAL(tokens.size(), 5);
    BOOST_CHECK_EQUAL(tokens.front(), "("); tokens.pop();
    BOOST_CHECK_EQUAL(tokens.front(), "1"); tokens.pop();
    BOOST_CHECK_EQUAL(tokens.front(), "+"); tokens.pop();
    BOOST_CHECK_EQUAL(tokens.front(), "2"); tokens.pop();
    BOOST_CHECK_EQUAL(tokens.front(), ")");
}

BOOST_AUTO_TEST_CASE(test_tokenize_empty)
{
    std::string line = "";
    petrov::Queue<std::string> tokens = petrov::tokenize(line);
    BOOST_CHECK(tokens.empty());
}

BOOST_AUTO_TEST_CASE(test_infix_to_postfix_simple)
{
    petrov::Queue<std::string> tokens;
    tokens.push("1");
    tokens.push("+");
    tokens.push("2");
    tokens.push("*");
    tokens.push("3");

    petrov::Queue<std::string> postfix = petrov::infixToPostfix(tokens);
    BOOST_REQUIRE_EQUAL(postfix.size(), 5);
    BOOST_CHECK_EQUAL(postfix.front(), "1"); postfix.pop();
    BOOST_CHECK_EQUAL(postfix.front(), "2"); postfix.pop();
    BOOST_CHECK_EQUAL(postfix.front(), "3"); postfix.pop();
    BOOST_CHECK_EQUAL(postfix.front(), "*"); postfix.pop();
    BOOST_CHECK_EQUAL(postfix.front(), "+");
}

BOOST_AUTO_TEST_CASE(test_infix_to_postfix_parentheses)
{
    petrov::Queue<std::string> tokens;
    tokens.push("(");
    tokens.push("1");
    tokens.push("+");
    tokens.push("2");
    tokens.push(")");
    tokens.push("*");
    tokens.push("3");

    petrov::Queue<std::string> postfix = petrov::infixToPostfix(tokens);
    BOOST_REQUIRE_EQUAL(postfix.size(), 5);
    BOOST_CHECK_EQUAL(postfix.front(), "1"); postfix.pop();
    BOOST_CHECK_EQUAL(postfix.front(), "2"); postfix.pop();
    BOOST_CHECK_EQUAL(postfix.front(), "+"); postfix.pop();
    BOOST_CHECK_EQUAL(postfix.front(), "3"); postfix.pop();
    BOOST_CHECK_EQUAL(postfix.front(), "*");
}

BOOST_AUTO_TEST_CASE(test_infix_to_postfix_mismatched_parentheses)
{
    petrov::Queue<std::string> tokens;
    tokens.push("(");
    tokens.push("1");
    tokens.push("+");
    tokens.push("2");

    BOOST_CHECK_THROW(petrov::infixToPostfix(tokens), std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(test_evaluate_simple_addition)
{
    petrov::Queue<std::string> postfix;
    postfix.push("1");
    postfix.push("2");
    postfix.push("+");

    long long result = petrov::evaluatePostfix(postfix);
    BOOST_CHECK_EQUAL(result, 3);
}

BOOST_AUTO_TEST_CASE(test_evaluate_complex)
{
    petrov::Queue<std::string> postfix;
    postfix.push("1");
    postfix.push("2");
    postfix.push("+");
    postfix.push("3");
    postfix.push("*");

    long long result = petrov::evaluatePostfix(postfix);
    BOOST_CHECK_EQUAL(result, 9);
}

BOOST_AUTO_TEST_CASE(test_evaluate_division)
{
    petrov::Queue<std::string> postfix;
    postfix.push("10");
    postfix.push("3");
    postfix.push("/");

    long long result = petrov::evaluatePostfix(postfix);
    BOOST_CHECK_EQUAL(result, 3);
}

BOOST_AUTO_TEST_CASE(test_evaluate_division_by_zero)
{
    petrov::Queue<std::string> postfix;
    postfix.push("5");
    postfix.push("0");
    postfix.push("/");

    BOOST_CHECK_THROW(petrov::evaluatePostfix(postfix), std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(test_evaluate_modulo_positive)
{
    petrov::Queue<std::string> postfix;
    postfix.push("7");
    postfix.push("3");
    postfix.push("%");

    long long result = petrov::evaluatePostfix(postfix);
    BOOST_CHECK_EQUAL(result, 1);
}

BOOST_AUTO_TEST_CASE(test_evaluate_modulo_negative)
{
    petrov::Queue<std::string> postfix;
    postfix.push("0");
    postfix.push("7");
    postfix.push("-");
    postfix.push("3");
    postfix.push("%");

    long long result = petrov::evaluatePostfix(postfix);
    BOOST_CHECK_EQUAL(result, 2);
}

BOOST_AUTO_TEST_CASE(test_evaluate_bitwise_or)
{
    petrov::Queue<std::string> postfix;
    postfix.push("5");   // 0101
    postfix.push("3");   // 0011
    postfix.push("|");   // 0111 = 7

    long long result = petrov::evaluatePostfix(postfix);
    BOOST_CHECK_EQUAL(result, 7);
}

BOOST_AUTO_TEST_CASE(test_evaluate_overflow_addition)
{
    petrov::Queue<std::string> postfix;
    postfix.push("9223372036854775807");
    postfix.push("1");
    postfix.push("+");

    BOOST_CHECK_THROW(petrov::evaluatePostfix(postfix), std::overflow_error);
}

BOOST_AUTO_TEST_CASE(test_evaluate_overflow_multiplication)
{
    petrov::Queue<std::string> postfix;
    postfix.push("9223372036854775807");
    postfix.push("2");
    postfix.push("*");

    BOOST_CHECK_THROW(petrov::evaluatePostfix(postfix), std::overflow_error);
}

BOOST_AUTO_TEST_CASE(test_evaluate_underflow_multiplication)
{
    petrov::Queue<std::string> postfix;
    postfix.push("-9223372036854775807");
    postfix.push("2");
    postfix.push("*");

    BOOST_CHECK_THROW(petrov::evaluatePostfix(postfix), std::overflow_error);
}

BOOST_AUTO_TEST_CASE(test_evaluate_invalid_expression)
{
    petrov::Queue<std::string> postfix;
    postfix.push("1");
    postfix.push("2");
    postfix.push("+");
    postfix.push("+");

    BOOST_CHECK_THROW(petrov::evaluatePostfix(postfix), std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(test_evaluate_empty)
{
    petrov::Queue<std::string> postfix;
    BOOST_CHECK_THROW(petrov::evaluatePostfix(postfix), std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(test_integration)
{
    std::string line = "( 1 + 2 ) * ( 3 + 4 )";
    petrov::Queue<std::string> tokens = petrov::tokenize(line);
    petrov::Queue<std::string> postfix = petrov::infixToPostfix(tokens);
    long long result = petrov::evaluatePostfix(postfix);
    BOOST_CHECK_EQUAL(result, 21);
}

BOOST_AUTO_TEST_SUITE_END()

