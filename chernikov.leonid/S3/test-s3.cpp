#include <boost/test/unit_test.hpp>
#include "hash_table.hpp"
#include "graph.hpp"
#include <string>
#include <fstream>
#include <cstdio>
#include <stdexcept>

namespace chernikov {

  BOOST_AUTO_TEST_SUITE(HashTableTests)

  BOOST_AUTO_TEST_CASE(HashTableEmptyInitially)
  {
    HashTable< std::string, int > ht;
    BOOST_CHECK(ht.empty());
    BOOST_CHECK_EQUAL(ht.size(), 0);
  }

  BOOST_AUTO_TEST_CASE(HashTableAddAndHas)
  {
    HashTable< std::string, int > ht;
    ht.add("key1", 100);
    BOOST_CHECK(!ht.empty());
    BOOST_CHECK_EQUAL(ht.size(), 1);
    BOOST_CHECK(ht.has("key1"));
    BOOST_CHECK(!ht.has("key2"));
  }

  BOOST_AUTO_TEST_CASE(HashTableAddUpdate)
  {
    HashTable< std::string, int > ht;
    ht.add("key1", 100);
    ht.add("key1", 200);
    BOOST_CHECK_EQUAL(ht.size(), 1);
    BOOST_CHECK_EQUAL(ht.get("key1"), 200);
  }

  BOOST_AUTO_TEST_CASE(HashTableDrop)
  {
    HashTable< std::string, int > ht;
    ht.add("key1", 100);
    ht.add("key2", 200);

    int val = ht.drop("key1");
    BOOST_CHECK_EQUAL(val, 100);
    BOOST_CHECK(!ht.has("key1"));
    BOOST_CHECK_EQUAL(ht.size(), 1);
  }

  BOOST_AUTO_TEST_SUITE_END()

  BOOST_AUTO_TEST_SUITE(GraphTests)

  BOOST_AUTO_TEST_CASE(GraphCreation)
  {
    Graph g("test");
    BOOST_CHECK_EQUAL(g.getName(), "test");
    BOOST_CHECK_EQUAL(g.edgeCount(), 0);
  }

  BOOST_AUTO_TEST_CASE(GraphDefaultConstructor)
  {
    Graph g;
    BOOST_CHECK_EQUAL(g.getName(), "");
    BOOST_CHECK_EQUAL(g.edgeCount(), 0);
  }

  BOOST_AUTO_TEST_CASE(GraphSetName)
  {
    Graph g;
    g.setName("new_name");
    BOOST_CHECK_EQUAL(g.getName(), "new_name");
  }

  BOOST_AUTO_TEST_CASE(GraphAddEdge)
  {
    Graph g("test");
    g.addEdge("A", "B", 5);
    BOOST_CHECK(g.hasVertex("A"));
    BOOST_CHECK(g.hasVertex("B"));
    BOOST_CHECK(!g.hasVertex("C"));
    BOOST_CHECK_EQUAL(g.edgeCount(), 1);
  }

  BOOST_AUTO_TEST_CASE(GraphAddMultipleEdges)
  {
    Graph g("test");
    g.addEdge("A", "B", 1);
    g.addEdge("A", "B", 2);
    g.addEdge("A", "C", 3);
    BOOST_CHECK(g.hasVertex("A"));
    BOOST_CHECK(g.hasVertex("B"));
    BOOST_CHECK(g.hasVertex("C"));
  }

  BOOST_AUTO_TEST_CASE(GraphWithZeroEdges)
  {
    Graph g("test");
    auto vertices = g.getVertices();
    BOOST_CHECK(vertices.empty());
  }

  BOOST_AUTO_TEST_CASE(GraphExtractSubgraph)
  {
    Graph g("test");
    g.addEdge("A", "B", 1);
    g.addEdge("B", "C", 2);
    g.addEdge("A", "C", 3);

    Graph::VertexList sub_vertices;
    sub_vertices.push_back("A");
    sub_vertices.push_back("C");

    Graph sub = g.extractSubgraph(sub_vertices);
    BOOST_CHECK(sub.hasVertex("A"));
    BOOST_CHECK(sub.hasVertex("C"));
    BOOST_CHECK(!sub.hasVertex("B"));
  }

  BOOST_AUTO_TEST_CASE(GraphMerge)
  {
    Graph g1("g1");
    g1.addEdge("A", "B", 1);

    Graph g2("g2");
    g2.addEdge("B", "C", 2);

    Graph merged = Graph::merge(g1, g2, "merged");
    BOOST_CHECK_EQUAL(merged.getName(), "merged");
    BOOST_CHECK(merged.hasVertex("A"));
    BOOST_CHECK(merged.hasVertex("B"));
    BOOST_CHECK(merged.hasVertex("C"));
  }

  BOOST_AUTO_TEST_CASE(GraphRemoveEdge)
  {
    Graph g("test");
    g.addEdge("A", "B", 1);
    g.addEdge("A", "B", 2);

    bool removed = g.removeEdge("A", "B", 1);
    BOOST_CHECK(removed);

    auto outbound = g.getOutbound("A");
    BOOST_CHECK(!outbound.empty());
  }

  BOOST_AUTO_TEST_CASE(GraphRemoveNonExistentEdge)
  {
    Graph g("test");
    g.addEdge("A", "B", 1);

    bool removed = g.removeEdge("A", "B", 999);
    BOOST_CHECK(!removed);
  }

  BOOST_AUTO_TEST_CASE(GraphInbound)
  {
    Graph g("test");
    g.addEdge("A", "B", 1);
    g.addEdge("C", "B", 2);

    auto inbound = g.getInbound("B");
    BOOST_CHECK(!inbound.empty());
  }

  BOOST_AUTO_TEST_CASE(GraphOutbound)
  {
    Graph g("test");
    g.addEdge("A", "B", 1);
    g.addEdge("A", "C", 2);

    auto outbound = g.getOutbound("A");
    BOOST_CHECK(!outbound.empty());
  }

  BOOST_AUTO_TEST_CASE(GraphAddVertex)
  {
    Graph g("test");
    g.addVertex("X");
    BOOST_CHECK(g.hasVertex("X"));
    BOOST_CHECK_EQUAL(g.edgeCount(), 0);
  }

  BOOST_AUTO_TEST_SUITE_END()

}
