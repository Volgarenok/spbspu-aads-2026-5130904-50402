#include <stdexcept>
#include <boost/test/unit_test.hpp>
#include "Graph.hpp"

namespace saldaev
{
  BOOST_AUTO_TEST_SUITE(GraphTests)

  using TestGraph = Graph< int, int >;

  void fill_sample_graph(TestGraph &g)
  {
    g.addVertex(100, "A");
    g.addVertex(200, "B");
    g.addVertex(300, "C");
    g.addEdge("A", "B", 10);
    g.addEdge("B", "C", 20);
  }

  BOOST_AUTO_TEST_CASE(constructor_and_empty)
  {
    TestGraph g;
    BOOST_TEST(g.countVertexes() == 0);
    BOOST_TEST(g.countEdges() == 0);
    BOOST_TEST(g.vertexIds().getSize() == 0);
    BOOST_TEST(g.edgesIds().getSize() == 0);
  }

  BOOST_AUTO_TEST_CASE(add_and_has_vertex)
  {
    TestGraph g;
    g.addVertex(42, "V1");
    BOOST_TEST(g.countVertexes() == 1);
    BOOST_TEST(g.hasVertex("V1"));
    BOOST_TEST(!g.hasVertex("V2"));

    BOOST_CHECK_THROW(g.addVertex(99, "V1"), std::invalid_argument);
    BOOST_TEST(g.countVertexes() == 1);
  }

  BOOST_AUTO_TEST_CASE(remove_vertex)
  {
    TestGraph g;
    fill_sample_graph(g);
    BOOST_TEST(g.countVertexes() == 3);
    BOOST_TEST(g.hasVertex("B"));

    g.removeVertex("B");
    BOOST_TEST(g.countVertexes() == 2);
    BOOST_TEST(!g.hasVertex("B"));
    BOOST_TEST(g.hasVertex("A"));
    BOOST_TEST(g.hasVertex("C"));
    BOOST_TEST(g.countEdges() == 0);
    BOOST_CHECK_THROW(g.hasEdge("A", "B"), std::logic_error);
    BOOST_CHECK_THROW(g.removeVertex("X"), std::out_of_range);
  }

  BOOST_AUTO_TEST_CASE(vertex_data)
  {
    TestGraph g;
    g.addVertex(123, "X");
    BOOST_TEST(g.getVertexData("X") == 123);
    g.setVertexData("X", 999);
    BOOST_TEST(g.getVertexData("X") == 999);
    BOOST_CHECK_THROW(g.getVertexData("Y"), std::exception);
    BOOST_CHECK_THROW(g.setVertexData("Y", 0), std::exception);
  }

  BOOST_AUTO_TEST_CASE(add_and_has_edge)
  {
    TestGraph g;
    g.addVertex(1, "A");
    g.addVertex(2, "B");
    g.addEdge("A", "B", 55);
    BOOST_TEST(g.countEdges() == 1);
    BOOST_TEST(g.hasEdge("A", "B"));
    BOOST_TEST(!g.hasEdge("B", "A"));

    BOOST_CHECK_THROW(g.addEdge("A", "B", 66), std::logic_error);
    BOOST_TEST(g.countEdges() == 1);
    BOOST_CHECK_THROW(g.addEdge("A", "C", 77), std::exception);
  }

  BOOST_AUTO_TEST_CASE(remove_edge)
  {
    TestGraph g;
    fill_sample_graph(g);
    BOOST_TEST(g.countEdges() == 2);
    BOOST_TEST(g.hasEdge("A", "B"));
    BOOST_TEST(g.hasEdge("B", "C"));

    g.removeEdge("A", "B");
    BOOST_TEST(g.countEdges() == 1);
    BOOST_TEST(!g.hasEdge("A", "B"));
    BOOST_TEST(g.hasEdge("B", "C"));

    BOOST_CHECK_THROW(g.removeEdge("A", "B"), std::invalid_argument);
    BOOST_CHECK_THROW(g.removeEdge("X", "Y"), std::logic_error);
  }

  BOOST_AUTO_TEST_CASE(edge_data)
  {
    TestGraph g;
    fill_sample_graph(g);
    BOOST_TEST(g.getEdgeData("A", "B") == 10);
    g.setEdgeData("A", "B", 999);
    BOOST_TEST(g.getEdgeData("A", "B") == 999);

    BOOST_CHECK_THROW(g.getEdgeData("B", "A"), std::invalid_argument);
    BOOST_CHECK_THROW(g.setEdgeData("X", "Y", 0), std::logic_error);
  }

  BOOST_AUTO_TEST_CASE(degrees)
  {
    TestGraph g;
    g.addVertex(0, "A");
    g.addVertex(0, "B");
    g.addVertex(0, "C");
    g.addEdge("A", "B", 1);
    g.addEdge("A", "C", 2);
    g.addEdge("B", "C", 3);

    BOOST_TEST(g.outdegree("A") == 2);
    BOOST_TEST(g.indegree("A") == 0);
    BOOST_TEST(g.degree("A") == 2);

    BOOST_TEST(g.outdegree("B") == 1);
    BOOST_TEST(g.indegree("B") == 1);
    BOOST_TEST(g.degree("B") == 2);

    BOOST_TEST(g.outdegree("C") == 0);
    BOOST_TEST(g.indegree("C") == 2);
    BOOST_TEST(g.degree("C") == 2);

    BOOST_CHECK_THROW(g.outdegree("X"), std::out_of_range);
    BOOST_CHECK_THROW(g.indegree("X"), std::out_of_range);
    BOOST_CHECK_THROW(g.degree("X"), std::out_of_range);
  }

  BOOST_AUTO_TEST_CASE(edge_lists)
  {
    TestGraph g;
    g.addVertex(0, "A");
    g.addVertex(0, "B");
    g.addVertex(0, "C");
    g.addEdge("A", "B", 1);
    g.addEdge("A", "C", 2);
    g.addEdge("B", "C", 3);

    Vector< std::string > outA = g.outgoingEdges("A");
    Vector< std::string > inC = g.incomingEdges("C");

    BOOST_TEST(outA.getSize() == 2);
    bool outA_ok = (outA[0] == "B" && outA[1] == "C") || (outA[0] == "C" && outA[1] == "B");
    BOOST_TEST(outA_ok);

    BOOST_TEST(inC.getSize() == 2);
    bool inC_ok = (inC[0] == "A" && inC[1] == "B") || (inC[0] == "B" && inC[1] == "A");
    BOOST_TEST(inC_ok);

    BOOST_CHECK_THROW(g.outgoingEdges("X"), std::out_of_range);
    BOOST_CHECK_THROW(g.incomingEdges("X"), std::out_of_range);
  }

  BOOST_AUTO_TEST_CASE(vertex_ids_and_edges_ids)
  {
    TestGraph g;
    fill_sample_graph(g);
    Vector< std::string > vids = g.vertexIds();
    BOOST_TEST(vids.getSize() == 3);
    bool hasA = false, hasB = false, hasC = false;
    for (size_t i = 0; i < vids.getSize(); ++i) {
      if (vids[i] == "A") {
        hasA = true;
      }
      if (vids[i] == "B") {
        hasB = true;
      }
      if (vids[i] == "C") {
        hasC = true;
      }
    }
    BOOST_TEST(hasA);
    BOOST_TEST(hasB);
    BOOST_TEST(hasC);

    Vector< std::pair< std::string, std::string > > eids = g.edgesIds();
    BOOST_TEST(eids.getSize() == 2);
    bool hasAB = false, hasBC = false;
    for (size_t i = 0; i < eids.getSize(); ++i) {
      if (eids[i].first == "A" && eids[i].second == "B") {
        hasAB = true;
      }
      if (eids[i].first == "B" && eids[i].second == "C") {
        hasBC = true;
      }
    }
    BOOST_TEST(hasAB);
    BOOST_TEST(hasBC);
  }

  BOOST_AUTO_TEST_CASE(clear_edges)
  {
    TestGraph g;
    fill_sample_graph(g);
    BOOST_TEST(g.countEdges() == 2);
    g.clearEdges();
    BOOST_TEST(g.countEdges() == 0);
    BOOST_TEST(!g.hasEdge("A", "B"));
    BOOST_TEST(!g.hasEdge("B", "C"));
    BOOST_TEST(g.countVertexes() == 3);
    BOOST_TEST(g.hasVertex("A"));
    g.clearEdges();
    BOOST_TEST(g.countEdges() == 0);
  }

  BOOST_AUTO_TEST_CASE(clear_all)
  {
    TestGraph g;
    fill_sample_graph(g);
    g.clear();
    BOOST_TEST(g.countVertexes() == 0);
    BOOST_TEST(g.countEdges() == 0);
    BOOST_TEST(!g.hasVertex("A"));
    g.clear();
    BOOST_TEST(g.countVertexes() == 0);
  }

  BOOST_AUTO_TEST_CASE(complex_operations)
  {
    TestGraph g;
    for (char c = 'A'; c <= 'D'; ++c) {
      g.addVertex(static_cast< int >(c), std::string(1, c));
    }
    g.addEdge("A", "B", 1);
    g.addEdge("B", "C", 2);
    g.addEdge("C", "D", 3);
    g.addEdge("D", "A", 4);

    BOOST_TEST(g.countVertexes() == 4);
    BOOST_TEST(g.countEdges() == 4);
    BOOST_TEST(g.outdegree("A") == 1);
    BOOST_TEST(g.indegree("A") == 1);
    BOOST_TEST(g.degree("A") == 2);

    g.removeVertex("C");
    BOOST_TEST(g.countVertexes() == 3);
    BOOST_TEST(g.countEdges() == 2);
    BOOST_TEST(g.hasEdge("A", "B"));
    BOOST_TEST(g.hasEdge("D", "A"));
    BOOST_CHECK_THROW(g.hasEdge("B", "C"), std::logic_error);
    BOOST_CHECK_THROW(g.hasEdge("C", "D"), std::logic_error);

    BOOST_CHECK_THROW(g.removeVertex("Z"), std::out_of_range);
    g.addVertex(300, "C");
    g.setVertexData("C", 999);
    BOOST_TEST(g.getVertexData("C") == 999);
    g.addEdge("B", "C", 77);
    BOOST_TEST(g.hasEdge("B", "C"));
    BOOST_TEST(g.getEdgeData("B", "C") == 77);
  }

  BOOST_AUTO_TEST_SUITE_END()
}
