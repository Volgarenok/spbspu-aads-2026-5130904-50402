#include <boost/test/unit_test.hpp>
#include "node.hpp"

namespace chernikov {

  BOOST_AUTO_TEST_SUITE(NodeTests)

  BOOST_AUTO_TEST_CASE(constructor_with_one_param)
  {
    Node< int > node(42);

    BOOST_TEST(node.data == 42);
    BOOST_TEST(node.next == nullptr);
  }

  BOOST_AUTO_TEST_CASE(constructor_with_two_params)
  {
    Node< int > next_node(100);
    Node< int > node(42, &next_node);

    BOOST_TEST(node.data == 42);
    BOOST_TEST(node.next == &next_node);
  }

  BOOST_AUTO_TEST_SUITE_END()

}
