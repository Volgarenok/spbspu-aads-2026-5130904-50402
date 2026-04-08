#include <boost/test/unit_test.hpp>
#include "queque.hpp"

using namespace muhamadiarov;

BOOST_AUTO_TEST_SUITE(quequeTest)
BOOST_AUTO_TEST_CASE(size)
{
  Queque< int > queque;
  BOOST_CHECK(queque.size() == 0);
  queque.push(7);
  BOOST_CHECK(queque.size() == 1);
}
BOOST_AUTO_TEST_CASE(push)
{
  Queque< int > queque;
  queque.push(5);
  queque.push(6);
  BOOST_CHECK(queque.size() == 2);
  BOOST_CHECK(queque.top() == 5)
}
BOOST_AUTO_TEST_CASE(pop)
{
  Queque< int > queque;
  queque.push(8);
  queque.push(7)
  queque.pop();
  BOOST_CHECK(queque.top() == 7);
}
BOOST_AUTO_TEST_CASE(top)
{
  Queque< int > queque;
  queque.push(8);
  queque.push(9);
  BOOST_CHECK(queque.top() == 8);
}
BOOST_AUTO_TEST_CASE(empty)
{
  Queque< int > queque;
  BOOST_CHECK(queque.empty() == true);
  queque.push(10);
  BOOST_CHECK(queque.empty() == false);
}
BOOST_AUTO_TEST_SUITE_END
