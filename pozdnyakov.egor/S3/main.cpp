#include <algorithm>
#include <fstream>
#include <iostream>
#include <new>
#include <stdexcept>
#include <string>
#include "commands.hpp"
#include "graph.hpp"
#include "stringUtils.hpp"
#include "vector.hpp"

using namespace pozdnyakov;

int main(int argc, char *argv[])
{
  try {
    if (argc != 2) {
      std::cerr << "Missing file argument\n";
      return 1;
    }

    Vector< std::pair< std::string, Graph > > graphsDict;

    std::ifstream infile(argv[1]);
    if (!infile.is_open()) {
      std::cerr << "Failed to open file: " << argv[1] << "\n";
      return 1;
    }

    std::string line;
    while (std::getline(infile, line)) {
      Vector< std::string > tokens = utils::splitBySpace(line);
      if (tokens.empty()) {
        continue;
      }

      if (tokens.size() == 2 && utils::isUnsignedNumber(tokens[1])) {
        std::string graphName = tokens[0];
        unsigned int edgesCount = std::stoul(tokens[1]);

        Graph *currentGraph = findGraph(graphsDict, graphName);
        if (!currentGraph) {
          graphsDict.pushBack({graphName, Graph()});
          currentGraph = &graphsDict[graphsDict.size() - 1].second;
        }

        for (unsigned int i = 0; i < edgesCount; ++i) {
          if (!std::getline(infile, line)) {
            std::cerr << "Unexpected end of file\n";
            return 1;
          }
          Vector< std::string > edgeTokens = utils::splitBySpace(line);
          if (edgeTokens.size() == 3 && utils::isUnsignedNumber(edgeTokens[2])) {
            currentGraph->addEdge(edgeTokens[0], edgeTokens[1], std::stoul(edgeTokens[2]));
          } else {
            std::cerr << "Invalid format in file: " << line << "\n";
            return 1;
          }
        }
      } else {
        std::cerr << "Invalid format in file: " << line << "\n";
        return 1;
      }
    }
    infile.close();

    CommandDispatcher cmds;
    cmds.add("graphs", pozdnyakov::cmdGraphs);
    cmds.add("vertexes", pozdnyakov::cmdVertexes);
    cmds.add("outbound", pozdnyakov::cmdOutbound);
    cmds.add("inbound", pozdnyakov::cmdInbound);
    cmds.add("bind", pozdnyakov::cmdBind);
    cmds.add("cut", pozdnyakov::cmdCut);
    cmds.add("create", pozdnyakov::cmdCreate);
    cmds.add("merge", pozdnyakov::cmdMerge);
    cmds.add("extract", pozdnyakov::cmdExtract);

    std::string commandLine;
    while (std::getline(std::cin, commandLine)) {
      Vector< std::string > tokens = utils::splitBySpace(commandLine);
      if (tokens.empty()) {
        continue;
      }

      cmds.execute(tokens[0], graphsDict, tokens);
    }
  } catch (const std::bad_alloc &e) {
    std::cerr << "Memory allocation error: " << e.what() << "\n";
    return 2;
  } catch (const std::exception &e) {
    std::cerr << "Standard exception caught: " << e.what() << "\n";
    return 2;
  } catch (...) {
    std::cerr << "Unknown error\n";
    return 2;
  }

  return 0;
}
