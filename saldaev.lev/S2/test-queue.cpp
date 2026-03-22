
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

  BOOST_AUTO_TEST_SUITE_END()

}
