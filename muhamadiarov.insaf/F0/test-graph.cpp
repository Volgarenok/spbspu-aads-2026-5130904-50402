#include <boost/test/unit_test.hpp>
#include "graph.hpp"
#include "edge.hpp"

using namespace muhamadiarov;

BOOST_AUTO_TEST_SUITE(GraphTest)

BOOST_AUTO_TEST_CASE(TestGraphConstructor)
{
  Graph g1;
  BOOST_CHECK_EQUAL(g1.getVertices().size(), 0);

  Graph g2(32, 16);
  BOOST_CHECK_EQUAL(g2.getVertices().size(), 0);
}

BOOST_AUTO_TEST_CASE(TestAddVertex)
{
  Graph g;

  g.addVertex(1);
  BOOST_CHECK(g.findVertex(1));
  BOOST_CHECK_EQUAL(g.getVertices().size(), 1);

  g.addVertex(2);
  g.addVertex(1);
  BOOST_CHECK(g.findVertex(1));
  BOOST_CHECK(g.findVertex(2));
  BOOST_CHECK_EQUAL(g.getVertices().size(), 2);

  BOOST_CHECK(!g.findVertex(10));
}

BOOST_AUTO_TEST_CASE(TestRemoveVertex)
{
  Graph g;
  g.addVertex(1);
  g.addVertex(2);
  g.addVertex(3);

  g.removeVertex(2);
  BOOST_CHECK(!g.findVertex(2));
  BOOST_CHECK_EQUAL(g.getVertices().size(), 2);

  g.removeVertex(10);
  BOOST_CHECK_EQUAL(g.getVertices().size(), 2);

  g.removeVertex(1);
  g.removeVertex(3);
  BOOST_CHECK_EQUAL(g.getVertices().size(), 0);
}

BOOST_AUTO_TEST_CASE(TestAddConnection)
{
  Graph g;
  g.addVertex(1);
  g.addVertex(2);

  g.addConnection(1, 2, 'a', 100);
  BOOST_CHECK(g.hasConnection(1, 2));
  BOOST_CHECK(!g.hasConnection(2, 1));

  Edge expectedEdge(2, RoadType::ASPHALT, 100);
  BOOST_CHECK(g.findConnection(1, expectedEdge));
  BOOST_CHECK(g.findConnection(1, 2, 'a', 100));

  BOOST_CHECK(!g.findConnection(1, 2, 'g', 200));

  g.addConnection(3, 4, 'g', 50);
  BOOST_CHECK(g.findVertex(3));
  BOOST_CHECK(g.findVertex(4));
  BOOST_CHECK(g.hasConnection(3, 4));
}

BOOST_AUTO_TEST_CASE(TestRemoveConnection)
{
  Graph g;
  g.addConnection(1, 2, 'a', 100);
  g.addConnection(1, 2, 'g', 150);
  g.addConnection(1, 2, 's', 200);

  g.removeConnection(1, 2, 'g', 150);
  BOOST_CHECK(g.hasConnection(1, 2));
  BOOST_CHECK(g.findConnection(1, 2, 'a', 100));
  BOOST_CHECK(!g.findConnection(1, 2, 'g', 150));
  BOOST_CHECK(g.findConnection(1, 2, 's', 200));

  g.removeAllConnection(1, 2);
  BOOST_CHECK(!g.hasConnection(1, 2));

  g.removeConnection(1, 2, 'a', 100);
  BOOST_CHECK(!g.hasConnection(1, 2));
}

BOOST_AUTO_TEST_CASE(testOutboundAndInbound)
{
  Graph g;

  g.addConnection(1, 2, 'a', 100);
  g.addConnection(1, 3, 'g', 200);
  g.addConnection(2, 1, 's', 50);
  g.addConnection(3, 2, 'a', 150);
  g.addConnection(4, 1, 'g', 75);

  List< std::pair< int, Edge > > out1 = g.getOutBounds(1);
  BOOST_CHECK_EQUAL(out1.size(), 2);

  bool foundTo2 = false, foundTo3 = false;
  LCIter< std::pair< int, Edge > > it = out1.cbegin();
  for (size_t i = 0; i < out1.size(); ++i)
  {
    if (it->first == 2 && it->second.distance_ == 100)
    {
      foundTo2 = true;
    }
    else if (it->first == 3 && it->second.distance_ == 200)
    {
      foundTo3 = true;
    }
    ++it;
  }
  BOOST_CHECK(foundTo2);
  BOOST_CHECK(foundTo3);

  List< std::pair< int, Edge > > in1 = g.getIncoming(1);
  BOOST_CHECK_EQUAL(in1.size(), 2);

  bool foundFrom2 = false, foundFrom4 = false;
  it = in1.cbegin();
  for (size_t i = 0; i < in1.size(); ++i)
  {
    if (it->first == 2 && it->second.distance_ == 50)
    {
      foundFrom2 = true;
    }
    if (it->first == 4 && it->second.distance_ == 75)
    {
      foundFrom4 = true;
    }
    ++it;
  }
  BOOST_CHECK(foundFrom2);
  BOOST_CHECK(foundFrom4);
}

BOOST_AUTO_TEST_CASE(TestPointValues)
{
  Graph g;
  g.addVertex(1);
  g.addVertex(2);
  g.addVertex(3);

  g.setPointValue(1, 100);
  g.setPointValue(2, 50);
  g.setPointValue(3, 200);

  int value;
  BOOST_CHECK(g.getPointValue(1, value));
  BOOST_CHECK_EQUAL(value, 100);
  BOOST_CHECK(g.getPointValue(2, value));
  BOOST_CHECK_EQUAL(value, 50);
  BOOST_CHECK(g.getPointValue(3, value));
  BOOST_CHECK_EQUAL(value, 200);

  BOOST_CHECK(!g.getPointValue(10, value));

  g.setPointValue(1, 150);
  BOOST_CHECK(g.getPointValue(1, value));
  BOOST_CHECK_EQUAL(value, 150);
}

BOOST_AUTO_TEST_CASE(TestClear)
{
  Graph g;
  g.addConnection(1, 2, 'a', 100);
  g.addConnection(2, 3, 'g', 150);
  g.addConnection(3, 1, 's', 200);
  g.setPointValue(1, 100);
  g.setPointValue(2, 50);
  g.setPointValue(3, 200);

  BOOST_CHECK_EQUAL(g.getVertices().size(), 3);
  BOOST_CHECK(g.hasConnection(1, 2));

  g.clear();

  BOOST_CHECK_EQUAL(g.getVertices().size(), 0);
  BOOST_CHECK(!g.hasConnection(1, 2));

  int value;
  BOOST_CHECK(!g.getPointValue(1, value));
}

BOOST_AUTO_TEST_SUITE_END()
