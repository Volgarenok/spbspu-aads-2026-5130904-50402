#ifndef MATVEEV_COMMANDS_HPP
#define MATVEEV_COMMANDS_HPP

#include "graphCollection.hpp"

#include <iosfwd>
#include <string>

namespace matveev
{
constexpr const char* INVALID_COMMAND = "<INVALID COMMAND>";

constexpr const char* GRAPHS_COMMAND = "graphs";
constexpr const char* VERTEXES_COMMAND = "vertexes";
constexpr const char* OUTBOUND_COMMAND = "outbound";
constexpr const char* INBOUND_COMMAND = "inbound";
constexpr const char* BIND_COMMAND = "bind";
constexpr const char* CUT_COMMAND = "cut";
constexpr const char* CREATE_COMMAND = "create";
constexpr const char* MERGE_COMMAND = "merge";
constexpr const char* EXTRACT_COMMAND = "extract";

void printGraphs(std::ostream& out, const GraphCollection& graphs);
void printVertexes(std::ostream& out, const Graph& graph);
void printOutbound(std::ostream& out, const Graph& graph, const std::string& vertex);
}

#endif
