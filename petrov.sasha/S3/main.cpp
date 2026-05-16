#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include <limits>
#include <set>
#include "Graph.hpp"
#include "HashTable.hpp"
#include "SHA1Hash.hpp"

int main(int argc, char* argv[])
{
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <filename>\n";
    return 1;
  }

  std::ifstream file(argv[1]);
  if (!file) {
    std::cerr << "Error opening file\n";
    return 1;
  }

  petrov::HashTable< std::string, petrov::Graph, petrov::SHA1Hasher< std::string >, petrov::DefaultEqual< std::string > > graphs(100);

  std::string line;
  while (std::getline(file, line)) {
    if (line.empty()) {
      continue;
    }

    std::istringstream iss(line);
    std::string graphName;
    size_t edgeCount = 0;
    iss >> graphName >> edgeCount;

    petrov::Graph graph;

    for (size_t i = 0; i < edgeCount; ++i) {
      std::string from;
      std::string to;
      unsigned int weight = 0;

      if (!(file >> from >> to >> weight)) {
        std::cerr << "Error reading edge data\n";
        return 1;
      }

      graph.addEdge(from, to, weight);
    }

    file.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    graphs.add(graphName, graph);
  }

  file.close();

  while (std::getline(std::cin, line)) {
    if (line.empty()) {
      continue;
    }

    std::istringstream iss(line);
    std::string command;
    iss >> command;

    try {
      if (command == "graphs") {
        std::vector< std::string > names;
        for (auto it = graphs.begin(); it != graphs.end(); ++it) {
          auto pair = *it;
          names.push_back(pair.first);
        }
        if (names.empty()) {
          std::cout << "\n";
        } else {
          std::sort(names.begin(), names.end());
          for (const auto& name : names) {
            std::cout << name << "\n";
          }
        }
      } else if (command == "vertexes") {
        std::string graphName;
        iss >> graphName;

        if (!graphs.has(graphName)) {
          std::cout << "<INVALID COMMAND>\n";
          continue;
        }

        petrov::Graph& graph = graphs.get(graphName);
        std::vector< std::string > vertices = graph.getVertices();
        
        if (vertices.empty()) {
          std::cout << "\n";
        } else {
          std::sort(vertices.begin(), vertices.end());
          for (const auto& vertex : vertices) {
            std::cout << vertex << "\n";
          }
        }
      } else if (command == "outbound") {
        std::string graphName;
        std::string vertex;
        iss >> graphName >> vertex;

        if (!graphs.has(graphName)) {
          std::cout << "<INVALID COMMAND>\n";
          continue;
        }

        petrov::Graph& graph = graphs.get(graphName);
        std::vector< std::string > vertices = graph.getVertices();

        if (std::find(vertices.begin(), vertices.end(), vertex) == vertices.end()) {
          std::cout << "<INVALID COMMAND>\n";
          continue;
        }

        auto outbound = graph.getOutbound(vertex);
        if (outbound.empty()) {
          std::cout << "\n";
        } else {
          for (const auto& pair : outbound) {
            std::cout << pair.first;
            for (unsigned int weight : pair.second) {
              std::cout << " " << weight;
            }
            std::cout << "\n";
          }
        }
      } else if (command == "inbound") {
        std::string graphName;
        std::string vertex;
        iss >> graphName >> vertex;

        if (!graphs.has(graphName)) {
          std::cout << "<INVALID COMMAND>\n";
          continue;
        }

        petrov::Graph& graph = graphs.get(graphName);
        std::vector< std::string > vertices = graph.getVertices();

        if (std::find(vertices.begin(), vertices.end(), vertex) == vertices.end()) {
          std::cout << "<INVALID COMMAND>\n";
          continue;
        }

        auto inbound = graph.getInbound(vertex);
        if (inbound.empty()) {
          std::cout << "\n";
        } else {
          for (const auto& pair : inbound) {
            std::cout << pair.first;
            for (unsigned int weight : pair.second) {
              std::cout << " " << weight;
            }
            std::cout << "\n";
          }
        }
      } else if (command == "bind") {
        std::string graphName;
        std::string vertexA;
        std::string vertexB;
        unsigned int weight = 0;
        iss >> graphName >> vertexA >> vertexB >> weight;

        if (!graphs.has(graphName)) {
          std::cout << "<INVALID COMMAND>\n";
          continue;
        }

        petrov::Graph& graph = graphs.get(graphName);
        graph.addEdge(vertexA, vertexB, weight);
      } else if (command == "cut") {
        std::string graphName;
        std::string vertexA;
        std::string vertexB;
        unsigned int weight = 0;
        iss >> graphName >> vertexA >> vertexB >> weight;

        if (!graphs.has(graphName)) {
          std::cout << "<INVALID COMMAND>\n";
          continue;
        }

        try {
          petrov::Graph& graph = graphs.get(graphName);
          graph.removeEdge(vertexA, vertexB, weight);
        } catch (...) {
          std::cout << "<INVALID COMMAND>\n";
        }
      } else if (command == "create") {
        std::string graphName;
        size_t vertexCount = 0;
        iss >> graphName >> vertexCount;

        if (graphs.has(graphName)) {
          std::cout << "<INVALID COMMAND>\n";
          continue;
        }

        petrov::Graph newGraph;
        
        for (size_t i = 0; i < vertexCount; ++i) {
          std::string vertex;
          iss >> vertex;
          newGraph.addEdge(vertex, vertex, 0);
          newGraph.removeEdge(vertex, vertex, 0);
        }

        graphs.add(graphName, newGraph);
      } else if (command == "merge") {
        std::string newGraphName;
        std::string graph1Name;
        std::string graph2Name;
        iss >> newGraphName >> graph1Name >> graph2Name;

        if (graphs.has(newGraphName) || !graphs.has(graph1Name) || !graphs.has(graph2Name)) {
          std::cout << "<INVALID COMMAND>\n";
          continue;
        }

        petrov::Graph newGraph;
        petrov::Graph& g1 = graphs.get(graph1Name);
        petrov::Graph& g2 = graphs.get(graph2Name);

        std::vector< std::string > vertices1 = g1.getVertices();
        for (const auto& vertex : vertices1) {
          auto outbound = g1.getOutbound(vertex);
          for (const auto& pair : outbound) {
            for (unsigned int weight : pair.second) {
              newGraph.addEdge(vertex, pair.first, weight);
            }
          }
        }

        std::vector< std::string > vertices2 = g2.getVertices();
        for (const auto& vertex : vertices2) {
          auto outbound = g2.getOutbound(vertex);
          for (const auto& pair : outbound) {
            for (unsigned int weight : pair.second) {
              newGraph.addEdge(vertex, pair.first, weight);
            }
          }
        }

        graphs.add(newGraphName, newGraph);
      } else if (command == "extract") {
        std::string newGraphName;
        std::string oldGraphName;
        size_t count = 0;
        iss >> newGraphName >> oldGraphName >> count;

        std::vector< std::string > vertices;
        for (size_t i = 0; i < count; ++i) {
          std::string vertex;
          iss >> vertex;
          vertices.push_back(vertex);
        }

        if (graphs.has(newGraphName) || !graphs.has(oldGraphName)) {
          std::cout << "<INVALID COMMAND>\n";
          continue;
        }

        petrov::Graph& oldGraph = graphs.get(oldGraphName);
        std::vector< std::string > allVertices = oldGraph.getVertices();

        bool valid = true;
        for (const auto& vertex : vertices) {
          if (std::find(allVertices.begin(), allVertices.end(), vertex) == allVertices.end()) {
            valid = false;
            break;
          }
        }

        if (!valid) {
          std::cout << "<INVALID COMMAND>\n";
          continue;
        }

        petrov::Graph newGraph;
        for (const auto& from : vertices) {
          auto outbound = oldGraph.getOutbound(from);
          for (const auto& pair : outbound) {
            if (std::find(vertices.begin(), vertices.end(), pair.first) != vertices.end()) {
              for (unsigned int weight : pair.second) {
                newGraph.addEdge(from, pair.first, weight);
              }
            }
          }
        }

        graphs.add(newGraphName, newGraph);
      } else {
        std::cout << "<INVALID COMMAND>\n";
      }
    } catch (...) {
      std::cout << "<INVALID COMMAND>\n";
    }
  }

  return 0;
}

