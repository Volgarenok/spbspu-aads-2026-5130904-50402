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

  BOOST_AUTO_TEST_SUITE_END()

}