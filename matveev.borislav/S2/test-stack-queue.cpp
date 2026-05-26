#define BOOST_TEST_MODULE stack_queue_tests
#include <boost/test/included/unit_test.hpp>

#include "stack.hpp"
#include "queue.hpp"
#include "../common/list.hpp"

#include <stdexcept>
#include <string>
#include <utility>

BOOST_AUTO_TEST_CASE(stack_push_top_drop_int)
{
  matveev::Stack< int > s;

  BOOST_TEST(s.empty());
  BOOST_TEST(s.size() == 0);

  s.push(10);
  s.push(20);
  s.push(30);

  BOOST_TEST(!s.empty());
  BOOST_TEST(s.size() == 3);

  BOOST_CHECK_EQUAL(s.top(), 30);
  s.drop();

  BOOST_CHECK_EQUAL(s.top(), 20);
  s.drop();

  BOOST_CHECK_EQUAL(s.top(), 10);
  s.drop();

  BOOST_TEST(s.empty());
  BOOST_TEST(s.size() == 0);
}

BOOST_AUTO_TEST_CASE(stack_push_top_drop_string)
{
  matveev::Stack< std::string > s;

  s.push("a");
  s.push("b");

  BOOST_CHECK_EQUAL(s.top(), "b");
  s.drop();

  BOOST_CHECK_EQUAL(s.top(), "a");
  s.drop();

  BOOST_TEST(s.empty());
}

BOOST_AUTO_TEST_CASE(stack_empty_throws)
{
  matveev::Stack< int > s;

  BOOST_CHECK_THROW(s.top(), std::runtime_error);
  BOOST_CHECK_THROW(s.drop(), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(queue_push_front_drop_int)
{
  matveev::Queue< int > q;

  BOOST_TEST(q.empty());
  BOOST_TEST(q.size() == 0);

  q.push(1);
  q.push(2);
  q.push(3);

  BOOST_TEST(!q.empty());
  BOOST_TEST(q.size() == 3);

  BOOST_CHECK_EQUAL(q.front(), 1);
  q.drop();

  BOOST_CHECK_EQUAL(q.front(), 2);
  q.drop();

  BOOST_CHECK_EQUAL(q.front(), 3);
  q.drop();

  BOOST_TEST(q.empty());
  BOOST_TEST(q.size() == 0);
}

BOOST_AUTO_TEST_CASE(queue_push_front_drop_string)
{
  matveev::Queue< std::string > q;

  q.push("one");
  q.push("two");

  BOOST_CHECK_EQUAL(q.front(), "one");
  q.drop();

  BOOST_CHECK_EQUAL(q.front(), "two");
  q.drop();

  BOOST_TEST(q.empty());
}

BOOST_AUTO_TEST_CASE(queue_empty_throws)
{
  matveev::Queue< int > q;

  BOOST_CHECK_THROW(q.front(), std::runtime_error);
  BOOST_CHECK_THROW(q.drop(), std::runtime_error);
}

struct ComplexValue
{
  ComplexValue():
    first(0),
    second()
  {}

  ComplexValue(int first_value, const std::string& second_value):
    first(first_value),
    second(second_value)
  {}

  int first;
  std::string second;
};

BOOST_AUTO_TEST_CASE(stack_emplace_complex_value)
{
  matveev::Stack< ComplexValue > stack;

  ComplexValue& value = stack.emplace(10, "ten");

  BOOST_TEST(stack.size() == 1);
  BOOST_TEST(value.first == 10);
  BOOST_TEST(value.second == "ten");
  BOOST_TEST(stack.top().first == 10);
  BOOST_TEST(stack.top().second == "ten");
}

BOOST_AUTO_TEST_CASE(queue_emplace_complex_value)
{
  matveev::Queue< ComplexValue > queue;

  ComplexValue& first = queue.emplace(1, "one");
  ComplexValue& second = queue.emplace(2, "two");

  BOOST_TEST(queue.size() == 2);
  BOOST_TEST(first.first == 1);
  BOOST_TEST(first.second == "one");
  BOOST_TEST(second.first == 2);
  BOOST_TEST(second.second == "two");

  BOOST_TEST(queue.front().first == 1);
  BOOST_TEST(queue.front().second == "one");
  queue.drop();

  BOOST_TEST(queue.front().first == 2);
  BOOST_TEST(queue.front().second == "two");
}

BOOST_AUTO_TEST_CASE(stack_emplace_pair)
{
  matveev::Stack< std::pair< int, std::string > > stack;

  std::pair< int, std::string >& value = stack.emplace(10, "ten");

  BOOST_TEST(stack.size() == 1);
  BOOST_TEST(value.first == 10);
  BOOST_TEST(value.second == "ten");
  BOOST_TEST(stack.top().first == 10);
  BOOST_TEST(stack.top().second == "ten");
}

BOOST_AUTO_TEST_CASE(queue_emplace_pair)
{
  matveev::Queue< std::pair< int, std::string > > queue;

  std::pair< int, std::string >& first = queue.emplace(1, "one");
  std::pair< int, std::string >& second = queue.emplace(2, "two");

  BOOST_TEST(queue.size() == 2);
  BOOST_TEST(first.first == 1);
  BOOST_TEST(first.second == "one");
  BOOST_TEST(second.first == 2);
  BOOST_TEST(second.second == "two");

  BOOST_TEST(queue.front().first == 1);
  BOOST_TEST(queue.front().second == "one");

  queue.drop();

  BOOST_TEST(queue.front().first == 2);
  BOOST_TEST(queue.front().second == "two");
}

BOOST_AUTO_TEST_CASE(list_emplace_after_pair)
{
  matveev::List< std::pair< int, std::string > > list;

  matveev::LIter< std::pair< int, std::string > > first = list.emplaceAfter(list.beforeBegin(), 1, "one");
  matveev::LIter< std::pair< int, std::string > > second = list.emplaceAfter(first, 2, "two");

  BOOST_TEST(first->first == 1);
  BOOST_TEST(first->second == "one");
  BOOST_TEST(second->first == 2);
  BOOST_TEST(second->second == "two");

  matveev::LIter< std::pair< int, std::string > > it = list.begin();

  BOOST_TEST(it->first == 1);
  BOOST_TEST(it->second == "one");

  ++it;

  BOOST_TEST(it->first == 2);
  BOOST_TEST(it->second == "two");
}

struct MoveOnlyValue
{
  MoveOnlyValue():
    value(0)
  {}

  explicit MoveOnlyValue(int rhs):
    value(rhs)
  {}

  MoveOnlyValue(const MoveOnlyValue&) = delete;
  MoveOnlyValue& operator=(const MoveOnlyValue&) = delete;

  MoveOnlyValue(MoveOnlyValue&& other) noexcept:
    value(other.value)
  {
    other.value = 0;
  }

  MoveOnlyValue& operator=(MoveOnlyValue&& other) noexcept
  {
    if (this != &other)
    {
      value = other.value;
      other.value = 0;
    }

    return *this;
  }

  int value;
};

BOOST_AUTO_TEST_CASE(stack_emplace_move_only_value)
{
  matveev::Stack< MoveOnlyValue > stack;

  MoveOnlyValue& value = stack.emplace(42);

  BOOST_TEST(stack.size() == 1);
  BOOST_TEST(value.value == 42);
  BOOST_TEST(stack.top().value == 42);
}

BOOST_AUTO_TEST_CASE(queue_emplace_move_only_value)
{
  matveev::Queue< MoveOnlyValue > queue;

  MoveOnlyValue& first = queue.emplace(10);
  MoveOnlyValue& second = queue.emplace(20);

  BOOST_TEST(queue.size() == 2);
  BOOST_TEST(first.value == 10);
  BOOST_TEST(second.value == 20);

  BOOST_TEST(queue.front().value == 10);
  queue.drop();

  BOOST_TEST(queue.front().value == 20);
}
