
#include "queue.hpp"
#include <boost/test/unit_test.hpp>
#include <stdexcept>

namespace saldaev
{

  BOOST_AUTO_TEST_SUITE(QueueTests)

  BOOST_AUTO_TEST_CASE(default_constructor_and_empty)
  {
    Queue< int > q;
    BOOST_TEST(q.empty());
  }

  BOOST_AUTO_TEST_CASE(push_and_front)
  {
    Queue< int > q;
    BOOST_TEST(q.empty());

    q.push(10);
    BOOST_TEST(!q.empty());
    BOOST_TEST(q.front() == 10);

    q.push(20);
    BOOST_TEST(q.front() == 10);
    BOOST_TEST(!q.empty());
  }

  BOOST_AUTO_TEST_CASE(pop)
  {
    Queue< int > q;
    q.push(1);
    q.push(2);
    q.push(3);

    q.pop();
    BOOST_TEST(q.front() == 2);
    q.pop();
    BOOST_TEST(q.front() == 3);
    q.pop();
    BOOST_TEST(q.empty());

    BOOST_CHECK_NO_THROW(q.pop());
    BOOST_TEST(q.empty());
  }

  BOOST_AUTO_TEST_CASE(clear)
  {
    Queue< int > q;
    q.push(1);
    q.push(2);
    q.push(3);
    q.clear();
    BOOST_TEST(q.empty());

    BOOST_CHECK_NO_THROW(q.clear());
    BOOST_TEST(q.empty());

    q.push(10);
    BOOST_TEST(q.front() == 10);
  }

  BOOST_AUTO_TEST_CASE(copy_constructor)
  {
    Queue< int > original;
    original.push(5);
    original.push(6);

    Queue< int > copy(original);
    BOOST_TEST(copy.front() == 5);
    copy.pop();
    BOOST_TEST(copy.front() == 6);
    copy.pop();
    BOOST_TEST(copy.empty());

    BOOST_TEST(!original.empty());
    BOOST_TEST(original.front() == 5);

    Queue< int > empty;
    Queue< int > copyEmpty(empty);
    BOOST_TEST(copyEmpty.empty());
  }

  BOOST_AUTO_TEST_CASE(copy_assignment)
  {
    Queue< int > a;
    a.push(10);
    a.push(20);

    Queue< int > b;
    b.push(30);

    b = a;
    BOOST_TEST(b.front() == 10);
    b.pop();
    BOOST_TEST(b.front() == 20);
    b.pop();
    BOOST_TEST(b.empty());

    BOOST_TEST(a.front() == 10);
    BOOST_TEST(!a.empty());

    b = b;
    BOOST_TEST(b.empty());

    Queue< int > empty;
    b = empty;
    BOOST_TEST(b.empty());
  }

  BOOST_AUTO_TEST_SUITE_END()

}
