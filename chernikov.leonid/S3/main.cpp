#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <cstdlib>
#include "hash_table.hpp"
#include "graph.hpp"

using namespace chernikov;

List< std::string > split(const std::string &line)
{
  List< std::string > tokens;
  std::string current;

  for (size_t i = 0; i < line.length(); ++i)
  {
    if (line[i] == ' ')
    {
      if (!current.empty())
      {
        tokens.push_back(current);
        current.clear();
      }
    } else
    {
      current += line[i];
    }
  }
  if (!current.empty())
  {
    tokens.push_back(current);
  }

  return tokens;
}

int string_to_int(const std::string &s)
{
  int result = 0;
  int sign = 1;
  size_t i = 0;

  if (s[0] == '-')
  {
    sign = -1;
    i = 1;
  }

  for (; i < s.length(); ++i)
  {
    result = result * 10 + (s[i] - '0');
  }

  return result * sign;
}

void cmd_graphs(const HashTable< std::string, Graph > &graphs)
{
  if (graphs.empty())
  {
    std::cout << "\n";
    return;
  }

  List< std::string > names;
  for (auto it = graphs.begin(); it != graphs.end(); ++it)
  {
    names.push_back((*it).first);
  }

  for (auto it1 = names.begin(); it1 != names.end(); ++it1)
  {
    for (auto it2 = it1; it2 != names.end(); ++it2)
    {
      if (*it2 < *it1)
      {
        std::swap(*it1, *it2);
      }
    }
  }

  for (auto it = names.cbegin(); it != names.cend(); ++it)
  {
    std::cout << *it << "\n";
  }
}

void cmd_vertexes(const HashTable< std::string, Graph > &graphs, const std::string &graph_name)
{
  if (!graphs.has(graph_name))
  {
    std::cout << "<INVALID COMMAND>\n";
    return;
  }

  Graph::VertexList vertices = graphs.get(graph_name).getVertices();
  if (vertices.empty())
  {
    std::cout << "\n";
    return;
  }

  for (auto it = vertices.cbegin(); it != vertices.cend(); ++it)
  {
    std::cout << *it << "\n";
  }
}

void cmd_outbound(const HashTable< std::string, Graph > &graphs, const std::string &graph_name,
                  const std::string &vertex)
{
  if (!graphs.has(graph_name) || !graphs.get(graph_name).hasVertex(vertex))
  {
    std::cout << "<INVALID COMMAND>\n";
    return;
  }

  auto outbound = graphs.get(graph_name).getOutbound(vertex);
  if (outbound.empty())
  {
    std::cout << "\n";
    return;
  }

  for (auto it = outbound.cbegin(); it != outbound.cend(); ++it)
  {
    std::cout << (*it).vertex;
    for (auto wit = (*it).weights.cbegin(); wit != (*it).weights.cend(); ++wit)
    {
      std::cout << " " << *wit;
    }
    std::cout << "\n";
  }
}

void cmd_inbound(const HashTable< std::string, Graph > &graphs, const std::string &graph_name,
                 const std::string &vertex)
{
  if (!graphs.has(graph_name) || !graphs.get(graph_name).hasVertex(vertex))
  {
    std::cout << "<INVALID COMMAND>\n";
    return;
  }

  auto inbound = graphs.get(graph_name).getInbound(vertex);
  if (inbound.empty())
  {
    std::cout << "\n";
    return;
  }

  for (auto it = inbound.cbegin(); it != inbound.cend(); ++it)
  {
    std::cout << (*it).vertex;
    for (auto wit = (*it).weights.cbegin(); wit != (*it).weights.cend(); ++wit)
    {
      std::cout << " " << *wit;
    }
    std::cout << "\n";
  }
}

void cmd_bind(HashTable< std::string, Graph > &graphs, const std::string &graph_name, const std::string &from,
              const std::string &to, int weight)
{
  if (!graphs.has(graph_name))
  {
    std::cout << "<INVALID COMMAND>\n";
    return;
  }

  graphs.get(graph_name).addEdge(from, to, weight);
}

void cmd_cut(HashTable< std::string, Graph > &graphs, const std::string &graph_name, const std::string &from,
             const std::string &to, int weight)
{
  if (!graphs.has(graph_name))
  {
    std::cout << "<INVALID COMMAND>\n";
    return;
  }

  if (!graphs.get(graph_name).hasVertex(from) || !graphs.get(graph_name).hasVertex(to))
  {
    std::cout << "<INVALID COMMAND>\n";
    return;
  }

  if (!graphs.get(graph_name).removeEdge(from, to, weight))
  {
    std::cout << "<INVALID COMMAND>\n";
  }
}

void cmd_create(HashTable< std::string, Graph > &graphs, const List< std::string > &tokens)
{
  if (tokens.size() < 2)
  {
    std::cout << "<INVALID COMMAND>\n";
    return;
  }

  auto it = tokens.cbegin();
  ++it;
  const std::string &name = *it;

  if (graphs.has(name))
  {
    std::cout << "<INVALID COMMAND>\n";
    return;
  }

  Graph new_graph(name);

  if (tokens.size() >= 3)
  {
    ++it;
    int count = string_to_int(*it);

    if (count < 0)
    {
      std::cout << "<INVALID COMMAND>\n";
      return;
    }

    ++it;
    for (int i = 0; i < count && it != tokens.cend(); ++i, ++it)
    {
      new_graph.addVertex(*it);
    }
  }

  graphs.add(name, new_graph);
}

void cmd_merge(HashTable< std::string, Graph > &graphs, const List< std::string > &tokens)
{
  if (tokens.size() < 4)
  {
    std::cout << "<INVALID COMMAND>\n";
    return;
  }

  auto it = tokens.cbegin();
  ++it;
  const std::string &new_name = *it;
  ++it;
  const std::string &g1_name = *it;
  ++it;
  const std::string &g2_name = *it;

  if (!graphs.has(g1_name) || !graphs.has(g2_name) || graphs.has(new_name))
  {
    std::cout << "<INVALID COMMAND>\n";
    return;
  }

  Graph merged = Graph::merge(graphs.get(g1_name), graphs.get(g2_name), new_name);
  graphs.add(new_name, merged);
}

void cmd_extract(HashTable< std::string, Graph > &graphs, const List< std::string > &tokens)
{
  if (tokens.size() < 4)
  {
    std::cout << "<INVALID COMMAND>\n";
    return;
  }

  auto it = tokens.cbegin();
  ++it;
  const std::string &new_name = *it;
  ++it;
  const std::string &old_name = *it;
  ++it;
  int count = string_to_int(*it);

  if (!graphs.has(old_name) || graphs.has(new_name))
  {
    std::cout << "<INVALID COMMAND>\n";
    return;
  }

  Graph::VertexList vertices;
  ++it;
  int i = 0;
  while (i < count && it != tokens.cend())
  {
    if (!graphs.get(old_name).hasVertex(*it))
    {
      std::cout << "<INVALID COMMAND>\n";
      return;
    }
    vertices.push_back(*it);
    ++it;
    ++i;
  }

  if (i < count)
  {
    std::cout << "<INVALID COMMAND>\n";
    return;
  }

  Graph extracted = graphs.get(old_name).extractSubgraph(vertices);
  extracted.setName(new_name);
  graphs.add(new_name, extracted);
}

int main(int argc, char *argv[])
{
  if (argc < 2)
  {
    std::cerr << "Error: No input file specified\n";
    std::cerr << "Usage: " << argv[0] << " <filename>\n";
    return 1;
  }

  std::ifstream file(argv[1]);
  if (!file.is_open())
  {
    std::cerr << "Error: Cannot open file '" << argv[1] << "'\n";
    return 1;
  }

  HashTable< std::string, Graph > graphs;

  std::string line;
  while (std::getline(file, line))
  {
    if (line.empty())
      continue;

    List< std::string > tokens = split(line);
    if (tokens.size() < 2)
      continue;

    auto it = tokens.cbegin();
    std::string graph_name = *it;
    ++it;
    int edge_count = string_to_int(*it);

    Graph graph(graph_name);

    for (int i = 0; i < edge_count; ++i)
    {
      if (!std::getline(file, line) || line.empty())
        break;

      tokens = split(line);
      if (tokens.size() < 3)
        continue;

      auto it2 = tokens.cbegin();
      std::string from = *it2;
      ++it2;
      std::string to = *it2;
      ++it2;
      int weight = string_to_int(*it2);

      graph.addEdge(from, to, weight);
    }

    try
    {
      graphs.add(graph_name, graph);
    } catch (const std::overflow_error &)
    {
      graphs.rehash(graphs.bucket_count() * 2);
      graphs.add(graph_name, graph);
    }
  }

  file.close();

  while (std::getline(std::cin, line))
  {
    if (line.empty())
      continue;

    List< std::string > tokens = split(line);
    if (tokens.empty())
      continue;

    auto it = tokens.cbegin();
    const std::string &cmd = *it;

    if (cmd == "graphs")
    {
      cmd_graphs(graphs);
    } else if (cmd == "vertexes" && tokens.size() >= 2)
    {
      ++it;
      cmd_vertexes(graphs, *it);
    } else if (cmd == "outbound" && tokens.size() >= 3)
    {
      ++it;
      std::string gname = *it;
      ++it;
      cmd_outbound(graphs, gname, *it);
    } else if (cmd == "inbound" && tokens.size() >= 3)
    {
      ++it;
      std::string gname = *it;
      ++it;
      cmd_inbound(graphs, gname, *it);
    } else if (cmd == "bind" && tokens.size() >= 5)
    {
      ++it;
      std::string gname = *it;
      ++it;
      std::string from = *it;
      ++it;
      std::string to = *it;
      ++it;
      int weight = string_to_int(*it);
      cmd_bind(graphs, gname, from, to, weight);
    } else if (cmd == "cut" && tokens.size() >= 5)
    {
      ++it;
      std::string gname = *it;
      ++it;
      std::string from = *it;
      ++it;
      std::string to = *it;
      ++it;
      int weight = string_to_int(*it);
      cmd_cut(graphs, gname, from, to, weight);
    } else if (cmd == "create")
    {
      cmd_create(graphs, tokens);
    } else if (cmd == "merge")
    {
      cmd_merge(graphs, tokens);
    } else if (cmd == "extract")
    {
      cmd_extract(graphs, tokens);
    } else
    {
      std::cout << "<INVALID COMMAND>\n";
    }
  }

  return 0;
}
