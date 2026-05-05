#define BOOST_TEST_MODULE stack_queue_tests
#include <boost/test/included/unit_test.hpp>

#include "../S2/stack.hpp"
#include "../S2/queue.hpp"
#include "../S2/parser.hpp"

#include <string>

BOOST_AUTO_TEST_CASE(stack_basic_operations)
{
  matveev::Stack<int> s;

  BOOST_CHECK(s.empty());
  BOOST_CHECK_EQUAL(s.size(), 0);

  s.push(10);
  s.push(20);
  s.push(30);

  BOOST_CHECK(!s.empty());
  BOOST_CHECK_EQUAL(s.size(), 3);

  BOOST_CHECK_EQUAL(s.top(), 30);

  BOOST_CHECK_EQUAL(s.drop(), 30);
  BOOST_CHECK_EQUAL(s.drop(), 20);
  BOOST_CHECK_EQUAL(s.drop(), 10);

  BOOST_CHECK(s.empty());
}

BOOST_AUTO_TEST_CASE(stack_string)
{
  matveev::Stack<std::string> s;

  s.push("a");
  s.push("b");

  BOOST_CHECK_EQUAL(s.top(), "b");
  BOOST_CHECK_EQUAL(s.drop(), "b");
  BOOST_CHECK_EQUAL(s.drop(), "a");
}

BOOST_AUTO_TEST_CASE(stack_exceptions)
{
  matveev::Stack<int> s;

  BOOST_CHECK_THROW(s.drop(), std::runtime_error);
  BOOST_CHECK_THROW(s.top(), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(queue_basic_operations)
{
  matveev::Queue<int> q;

  BOOST_CHECK(q.empty());
  BOOST_CHECK_EQUAL(q.size(), 0);

  q.push(1);
  q.push(2);
  q.push(3);

  BOOST_CHECK(!q.empty());
  BOOST_CHECK_EQUAL(q.size(), 3);

  BOOST_CHECK_EQUAL(q.front(), 1);

  BOOST_CHECK_EQUAL(q.drop(), 1);
  BOOST_CHECK_EQUAL(q.drop(), 2);
  BOOST_CHECK_EQUAL(q.drop(), 3);

  BOOST_CHECK(q.empty());
}

BOOST_AUTO_TEST_CASE(queue_string)
{
  matveev::Queue<std::string> q;

  q.push("one");
  q.push("two");

  BOOST_CHECK_EQUAL(q.front(), "one");
  BOOST_CHECK_EQUAL(q.drop(), "one");
  BOOST_CHECK_EQUAL(q.drop(), "two");
}

BOOST_AUTO_TEST_CASE(queue_exceptions)
{
  matveev::Queue<int> q;

  BOOST_CHECK_THROW(q.drop(), std::runtime_error);
  BOOST_CHECK_THROW(q.front(), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(parser_simple_expression)
{
  matveev::Queue<std::string> q;

  q.push("2");
  q.push("+");
  q.push("3");

  auto postfix = matveev::toPostfix(q);
  long result = matveev::evaluatePostfix(postfix);

  BOOST_CHECK_EQUAL(result, 5);
}

BOOST_AUTO_TEST_CASE(parser_precedence)
{
  matveev::Queue<std::string> q;

  q.push("2");
  q.push("+");
  q.push("3");
  q.push("*");
  q.push("4");

  auto postfix = matveev::toPostfix(q);
  long result = matveev::evaluatePostfix(postfix);

  BOOST_CHECK_EQUAL(result, 14);
}

BOOST_AUTO_TEST_CASE(parser_parentheses)
{
  matveev::Queue<std::string> q;

  q.push("(");
  q.push("2");
  q.push("+");
  q.push("3");
  q.push(")");
  q.push("*");
  q.push("4");

  auto postfix = matveev::toPostfix(q);
  long result = matveev::evaluatePostfix(postfix);

  BOOST_CHECK_EQUAL(result, 20);
}

BOOST_AUTO_TEST_CASE(parser_gcd)
{
  matveev::Queue<std::string> q;

  q.push("12");
  q.push("gcd");
  q.push("18");

  auto postfix = matveev::toPostfix(q);
  long result = matveev::evaluatePostfix(postfix);

  BOOST_CHECK_EQUAL(result, 6);
}

BOOST_AUTO_TEST_CASE(parser_complex)
{
  matveev::Queue<std::string> q;

  q.push("(");
  q.push("10");
  q.push("+");
  q.push("5");
  q.push(")");
  q.push("gcd");
  q.push("5");

  auto postfix = matveev::toPostfix(q);
  long result = matveev::evaluatePostfix(postfix);

  BOOST_CHECK_EQUAL(result, 5);
}

BOOST_AUTO_TEST_CASE(parser_invalid_expression)
{
  matveev::Queue<std::string> q;

  q.push("+");
  q.push("2");

  BOOST_CHECK_THROW(
    matveev::evaluatePostfix(matveev::toPostfix(q)),
    std::runtime_error
  );
}
