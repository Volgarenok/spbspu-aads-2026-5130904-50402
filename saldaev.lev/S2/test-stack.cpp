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

  BOOST_AUTO_TEST_CASE(copy_constructor)
  {
    Stack< int > original;
    original.push(5);
    original.push(6);

    Stack< int > copy(original);
    BOOST_TEST(copy.top() == 6);
    copy.pop();
    BOOST_TEST(copy.top() == 5);
    copy.pop();
    BOOST_TEST(copy.empty());

    BOOST_TEST(!original.empty());
    BOOST_TEST(original.top() == 6);

    Stack< int > empty;
    Stack< int > copyEmpty(empty);
    BOOST_TEST(copyEmpty.empty());
  }

  BOOST_AUTO_TEST_CASE(copy_assignment)
  {
    Stack< int > a;
    a.push(10);
    a.push(20);

    Stack< int > b;
    b.push(30);

    b = a;
    BOOST_TEST(b.top() == 20);
    b.pop();
    BOOST_TEST(b.top() == 10);
    b.pop();
    BOOST_TEST(b.empty());

    BOOST_TEST(a.top() == 20);
    BOOST_TEST(!a.empty());

    b = b;
    BOOST_TEST(b.empty());

    Stack< int > empty;
    b = empty;
    BOOST_TEST(b.empty());
  }

  BOOST_AUTO_TEST_CASE(move_constructor)
  {
    Stack< int > src;
    src.push(100);
    src.push(200);

    Stack< int > dst(std::move(src));
    BOOST_TEST(dst.top() == 200);
    dst.pop();
    BOOST_TEST(dst.top() == 100);
    dst.pop();
    BOOST_TEST(dst.empty());

    BOOST_TEST(src.empty());

    Stack< int > empty;
    Stack< int > movedEmpty(std::move(empty));
    BOOST_TEST(movedEmpty.empty());
    BOOST_TEST(empty.empty());
  }

  BOOST_AUTO_TEST_CASE(move_assignment)
  {
    Stack< int > a;
    a.push(1);
    a.push(2);

    Stack< int > b;
    b.push(3);

    b = std::move(a);
    BOOST_TEST(b.top() == 2);
    b.pop();
    BOOST_TEST(b.top() == 1);
    b.pop();
    BOOST_TEST(b.empty());

    BOOST_TEST(a.empty());

    b = std::move(b);
    BOOST_TEST(b.empty());

    Stack< int > empty;
    Stack< int > target;
    target.push(42);
    target = std::move(empty);
    BOOST_TEST(target.empty());
    BOOST_TEST(empty.empty());
  }

  BOOST_AUTO_TEST_SUITE_END()

}
