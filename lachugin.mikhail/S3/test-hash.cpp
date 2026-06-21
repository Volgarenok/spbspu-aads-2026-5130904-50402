#define BOOST_TEST_MODULE S3Tests
#include <boost/test/included/unit_test.hpp>

#include "HashTable.hpp"
#include "Graph.hpp"
#include "GraphStorage.hpp"

using namespace lachugin;

BOOST_AUTO_TEST_SUITE(HashTableTests)

BOOST_AUTO_TEST_CASE(test_rehash)
{
  HashTable< int, int, std::hash< int >, std::equal_to< int > > table(2, 1, 1);

  for (int i = 0; i < 20; ++i)
  {
    table.add(i, i * 10);
  }

  BOOST_CHECK_EQUAL(table.size(), 20);

  for (int i = 0; i < 20; ++i)
  {
    BOOST_CHECK(table.has(i));
    BOOST_CHECK_EQUAL(table.get(i), i * 10);
  }
}

BOOST_AUTO_TEST_CASE(test_bind)
{
  Graph g;

  g.bind("a", "b", 10);

  BOOST_CHECK(g.hasVertex("a"));
  BOOST_CHECK(g.hasVertex("b"));
  BOOST_CHECK(g.hasEdge("a", "b"));
}

BOOST_AUTO_TEST_CASE(test_cut)
{
  Graph g;

  g.bind("a", "b", 10);

  g.cut("a", "b", 10);

  BOOST_CHECK(!g.hasEdge("a", "b"));
}

BOOST_AUTO_TEST_CASE(test_add_graph)
{
  GraphStorage storage;

  Graph g;

  storage.addGraph("gr1", g);

  BOOST_CHECK(storage.hasGraph("gr1"));
}



BOOST_AUTO_TEST_SUITE_END()
