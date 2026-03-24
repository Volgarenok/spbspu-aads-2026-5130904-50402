#include "queue.hpp"
#include "stack.hpp"
#include <boost/test/unit_test.hpp>
#include <stdexcept>

namespace saldaev
{

  BOOST_AUTO_TEST_SUITE(StackTests)

  BOOST_AUTO_TEST_CASE(default_constructor_and_empty)
  {
    Stack< int > q;
    BOOST_TEST(q.empty());
  }

  BOOST_AUTO_TEST_CASE(push_and_front)
  {
    Stack< int > q;
    BOOST_TEST(q.empty());

    q.push(10);
    BOOST_TEST(!q.empty());
    BOOST_TEST(q.top() == 10);

    q.push(20);
    BOOST_TEST(q.top() == 20);
    BOOST_TEST(!q.empty());
  }

  BOOST_AUTO_TEST_CASE(pop)
  {
    Stack< int > q;
    q.push(1);
    q.push(2);
    q.push(3);

    q.pop();
    BOOST_TEST(q.top() == 2);
    q.pop();
    BOOST_TEST(q.top() == 1);
    q.pop();
    BOOST_TEST(q.empty());

    BOOST_CHECK_NO_THROW(q.pop());
    BOOST_TEST(q.empty());
  }

  BOOST_AUTO_TEST_CASE(clear)
  {
    Stack< int > q;
    q.push(1);
    q.push(2);
    q.push(3);
    q.clear();
    BOOST_TEST(q.empty());

    BOOST_CHECK_NO_THROW(q.clear());
    BOOST_TEST(q.empty());

    q.push(10);
    BOOST_TEST(q.top() == 10);
  }

  BOOST_AUTO_TEST_SUITE_END()

}
