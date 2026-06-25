#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "HashTableImpl.hpp"
#include "Graph.hpp"
#include "ListImpl.hpp"
#include <limits>

using pair_t = std::pair<std::string, vishnevskiy::List<int>*>;
using stringHash_t = size_t(*)(const std::string&);
using stringEq_t = bool(*)(const std::string&, const std::string&);
using vHt = size_t(*)(const vishnevskiy::vertex&);
using vEqt = bool(*)(const vishnevskiy::vertex&, const vishnevskiy::vertex&);
using graph_t = vishnevskiy::HashTable<std::string, vishnevskiy::graph, stringHash_t, stringEq_t>;

bool stringEq(const std::string& a, const std::string& b)
{
  return a == b;
}

size_t stringHasher(const std::string& s)
{
  boost::hash2::siphash_64 hash;
  hash.update(s.data(), s.size());
  return hash.result();
}

void sortStrings(std::string* arr, size_t size)
{
  for (size_t i = 0; i < size - 1; ++i)
  {
    for (size_t j = 0; j < size - i - 1; ++j)
    {
      if (arr[j] > arr[j + 1])
      {
        std::string temp = arr[j];
        arr[j] = arr[j + 1];
        arr[j + 1] = temp;
      }
    }
  }
}

void sortPair(std::pair<std::string, vishnevskiy::List<int>*>* data, size_t size)
{
  for (size_t i = 0; i < size - 1; ++i)
  {
    for (size_t j = 0; j < size - i - 1; ++j)
    {
      if (data[j].first > data[j + 1].first)
      {
        std::pair<std::string, vishnevskiy::List<int>*> temp = data[j];
        data[j] = data[j + 1];
        data[j + 1] = temp;
      }
    }
  }
}

void printWeights(vishnevskiy::List<int>* weights, std::ostream& o)
{
  size_t cnt = 0;
  vishnevskiy::LIter<int> countIt(weights);
  while (countIt.hasNext())
  {
    cnt++;
    ++countIt;
  }
  cnt++;
  int* arr = new int[cnt];
  vishnevskiy::LIter<int> fillIt(weights);
  for (size_t i = 0; i < cnt; ++i)
  {
    arr[i] = fillIt.value();
    if (fillIt.hasNext()) ++fillIt;
  }
  for (size_t i = 0; i < cnt - 1; ++i)
  {
    for (size_t j = 0; j < cnt - i - 1; ++j)
    {
      if (arr[j] > arr[j + 1])
      {
        int temp = arr[j];
        arr[j] = arr[j + 1];
        arr[j + 1] = temp;
      }
    }
  }
  for (size_t i = 0; i < cnt; ++i)
  {
    o << " " << arr[i];
  }
  delete[] arr;
}

bool hasEl(std::string* arr, size_t size, std::string el)
{
  for (size_t i = 0; i < size; ++i)
  {
    if (arr[i] == el)
    {
      return true;
    }
  }
  return false;
}

bool hasElList(vishnevskiy::List<std::string>* arr, std::string el)
{
  vishnevskiy::LIter<std::string> it(arr);
  while (it.hasNext())
  {
    if (it.value() == el)
    {
      return true;
    }
    ++it;
  }
  if (it.value() == el)
  {
    return true;
  }
  return false;
}

void load(const std::string& filename, graph_t& graphtable)
{
  std::ifstream file(filename);
  if (!file.is_open())
  {
    throw std::runtime_error("Cannot open file");
  }

  std::string line;
  std::string graphName;
  size_t vertexCount = 0;

  while (std::getline(file, line))
  {
    if (line.empty() || line.find_first_not_of(" \t") == std::string::npos)
    {
      continue;
    }
    std::istringstream iss(line);

    if (vertexCount == 0)
    {
      if (!(iss >> graphName >> vertexCount))
      {
        throw std::runtime_error("Invalid file format");
      }
      vishnevskiy::graph newGraph(graphName, vertexCount);
      graphtable.add(graphName, newGraph);
    }
    else
    {
      std::string from, to;
      int weight;
      if (!(iss >> from >> to >> weight))
      {
        throw std::runtime_error("Invalid edge format");
      }
      vishnevskiy::graph& g = graphtable.at(graphName);
      vishnevskiy::vertex v{to, from};

      if (!g.points)
      {
        g.points = new vishnevskiy::List<std::string>();
        g.points->val = to;
        g.points->next = nullptr;
        g.pointCount = 1;
      }

      if (!hasElList(g.points, to))
      {
        vishnevskiy::LIter<std::string> it(g.points);
        g.pointCount++;
        it.end();
        it.insert(to);
      }
      if (!hasElList(g.points, from))
      {
        vishnevskiy::LIter<std::string> it(g.points);
        g.pointCount++;
        it.end();
        it.insert(from);
      }

      if (g.vertexes.has(v))
      {
        vishnevskiy::List<int>* weights = g.vertexes.at(v);
        vishnevskiy::LIter<int> it(weights);
        it.end();
        it.insert(weight);
      }
      else
      {
        vishnevskiy::List<int>* weights = new vishnevskiy::List<int>();
        weights->val = weight;
        weights->next = nullptr;
        g.vertexes.add(v, weights);
      }

      vertexCount--;
    }
  }
}

void graphs(std::ostream& o, std::istream&, graph_t& graphtable)
{
  if (graphtable.getSize() == 0)
  {
    o << "\n";
    return;
  }
  vishnevskiy::tableIt<std::string, vishnevskiy::graph, stringHash_t, stringEq_t> it(&graphtable);
  size_t cnt = 0;
  while (it.hasNext())
  {
    cnt++;
    it.next();
  }
  vishnevskiy::tableIt<std::string, vishnevskiy::graph, stringHash_t, stringEq_t> it2(&graphtable);
  std::string* st = new std::string[cnt];
  size_t c = 0;
  while (it2.hasNext())
  {
    st[c] = it2.key();
    c++;
    it2.next();
  }
  sortStrings(st, cnt);
  for (size_t i = 0; i < cnt; ++i)
  {
    o << st[i] << "\n";
  }
  delete[] st;
}

void vertexes(std::ostream& o, std::istream& i, graph_t& graphtable)
{
  std::string name;
  i >> name;

  if (graphtable.has(name))
  {
    vishnevskiy::graph& g = graphtable.at(name);
    if (g.pointCount != 0 && g.points)
    {
      std::string* vert = new std::string[g.pointCount];
      size_t currEl = 0;
      vishnevskiy::LIter<std::string> it(g.points);

      while (it.hasNext())
      {
        vert[currEl] = it.value();
        currEl++;
        ++it;
      }
      vert[currEl] = it.value();
      currEl++;
      sortStrings(vert, currEl);

      for (size_t i = 0; i < currEl; ++i)
      {
        o << vert[i] << "\n";
      }
      delete[] vert;
    }
    else
    {
      o << "\n";
    }
  }
  else
  {
    throw std::logic_error("Cannot find name!");
  }
}

void outbound(std::ostream& o, std::istream& i, graph_t& graphtable)
{
  std::string name, vertex;
  i >> name >> vertex;

  if (graphtable.has(name))
  {
    vishnevskiy::graph& g = graphtable.at(name);
    if (g.pointCount == 0 || !g.points)
    {
      throw std::logic_error("No vertexes!");
    }
    pair_t* pair = new pair_t[g.pointCount];
    size_t currEl = 0;
    bool f = false;
    vishnevskiy::tableIt<vishnevskiy::vertex, vishnevskiy::List<int>*, vHt, vEqt> it(&g.vertexes);
    while (it.hasNext())
    {
      if (it.key().in == vertex)
      {
        pair[currEl] = std::make_pair(it.key().out, it.val());
        currEl++;
        f = true;
      }
      it.next();
    }
    if (!f)
    {
      o << "\n";
      delete[] pair;
      return;
    }
    sortPair(pair, currEl);
    for (size_t i = 0; i < currEl; ++i)
    {
      o << pair[i].first;
      printWeights(pair[i].second, o);
      o << "\n";
    }
    delete[] pair;
  }
  else
  {
    throw std::logic_error("Cannot find name!");
  }
}

void inbound(std::ostream& o, std::istream& i, graph_t& graphtable)
{
  std::string name, vertex;
  i >> name >> vertex;

  if (graphtable.has(name))
  {
    vishnevskiy::graph& g = graphtable.at(name);
    if (g.pointCount == 0 || !g.points)
    {
      throw std::logic_error("No vertexes!");
    }
    pair_t* pair = new pair_t[g.pointCount];
    size_t currEl = 0;
    bool f = false;
    vishnevskiy::tableIt<vishnevskiy::vertex, vishnevskiy::List<int>*, vHt, vEqt> it(&g.vertexes);
    while (it.hasNext())
    {
      if (it.key().out == vertex)
      {
        pair[currEl] = std::make_pair(it.key().in, it.val());
        currEl++;
        f = true;
      }
      it.next();
    }
    if (!f)
    {
      o << "\n";
      delete[] pair;
      return;
    }
    sortPair(pair, currEl);
    for (size_t i = 0; i < currEl; ++i)
    {
      o << pair[i].first;
      printWeights(pair[i].second, o);
      o << "\n";
    }
    delete[] pair;
  }
  else
  {
    throw std::logic_error("Cannot find name!");
  }
}

void bind(std::ostream&, std::istream& i, graph_t& graphtable)
{
  std::string name, from, to;
  int weight;
  i >> name >> from >> to >> weight;

  if (graphtable.has(name))
  {
    vishnevskiy::graph& g = graphtable.at(name);
    vishnevskiy::vertex v{to, from};

    if (!g.points)
    {
      g.points = new vishnevskiy::List<std::string>();
      g.points->val = to;
      g.points->next = nullptr;
      g.pointCount = 1;
    }
    else if (!hasElList(g.points, to))
    {
      vishnevskiy::LIter<std::string> it(g.points);
      it.end();
      it.insert(to);
      g.pointCount++;
    }

    if (!hasElList(g.points, from))
    {
      vishnevskiy::LIter<std::string> it(g.points);
      it.end();
      it.insert(from);
      g.pointCount++;
    }

    if (g.vertexes.has(v))
    {
      vishnevskiy::List<int>* weights = g.vertexes.at(v);
      vishnevskiy::LIter<int> it(weights);
      it.end();
      it.insert(weight);
    }
    else
    {
      vishnevskiy::List<int>* w = new vishnevskiy::List<int>();
      w->val = weight;
      w->next = nullptr;
      g.vertexes.add(v, w);
    }
  }
  else
  {
    throw std::logic_error("Cannot find name!");
  }
}

void cut(std::ostream&, std::istream& i, graph_t& graphtable)
{
  std::string name, from, to;
  int weight;
  i >> name >> from >> to >> weight;

  if (!graphtable.has(name))
  {
    throw std::logic_error("Cannot find name!");
  }

  vishnevskiy::graph& g = graphtable.at(name);
  vishnevskiy::vertex v{to, from};

  if (!g.vertexes.has(v))
  {
    throw std::logic_error("Cannot find vertex!");
  }

  vishnevskiy::List<int>* wlist = g.vertexes.at(v);
  vishnevskiy::LIter<int> prev(wlist);
  vishnevskiy::LIter<int> curr(wlist);
  bool f = false;
  while (curr.hasNext() && !f)
  {
    if (curr.value() == weight)
    {
      f = true;
    }
    prev = curr;
    ++curr;
  }

  if (curr.value() == weight)
  {
    f = true;
  }

  if (!f)
  {
    throw std::logic_error("Cannot find weight!");
  }

  if (curr.curr == wlist)
  {
    vishnevskiy::List<int>* next = wlist->next;
    if (next)
    {
      wlist->val = next->val;
      wlist->next = next->next;
      delete next;
    }
    else
    {
      delete wlist;
      g.vertexes.drop(v);
    }
  }
  else
  {
    prev.curr->next = curr.curr->next;
    delete curr.curr;
  }
}

void create(std::ostream&, std::istream& i, graph_t& graphtable)
{
  std::string name;
  i >> name;

  if (graphtable.has(name))
  {
    throw std::logic_error("Graph exists!");
  }


  size_t pointCount = 0;
  if (i >> pointCount)
  {
    vishnevskiy::graph newGraph(name, pointCount);
    newGraph.pointCount = 0;


    if (pointCount > 0)
    {
      newGraph.points = new vishnevskiy::List<std::string>();
      vishnevskiy::LIter<std::string> it(newGraph.points);


      for (size_t j = 0; j < pointCount; ++j)
      {
        std::string pointName;
        i >> pointName;
        if (j == 0)
        {
          newGraph.points->val = pointName;
          newGraph.points->next = nullptr;
          newGraph.pointCount = 1;
        }
        else
        {
          it.insert(pointName);
          newGraph.pointCount++;
        }
      }
    }
    graphtable.add(name, newGraph);
  }
  else
  {
    i.clear();
    graphtable.add(name, vishnevskiy::graph(name));
  }
}

void merge(std::ostream&, std::istream& i, graph_t& graphtable)
{
  std::string name1, name2, newname;
  i >> newname >> name1 >> name2;

  if (!graphtable.has(name1) || !graphtable.has(name2))
  {
    throw std::logic_error("Cannot find name!");
  }

  if (graphtable.has(newname))
  {
    throw std::logic_error("Graph exists!");
  }

  vishnevskiy::graph& g1 = graphtable.at(name1);
  vishnevskiy::graph& g2 = graphtable.at(name2);
  size_t totalVertexes = g1.vertexCount + g2.vertexCount;
  vishnevskiy::graph newGraph(newname, totalVertexes);

  if (g1.points)
  {
    newGraph.points = new vishnevskiy::List<std::string>();
    newGraph.points->val = g1.points->val;
    newGraph.points->next = nullptr;
    newGraph.pointCount = 1;

    vishnevskiy::List<std::string>* h = g1.points->next;
    vishnevskiy::List<std::string>* e = newGraph.points;

    while (h)
    {
      e->next = new vishnevskiy::List<std::string>();
      e = e->next;
      e->val = h->val;
      e->next = nullptr;
      h = h->next;
      newGraph.pointCount++;
    }
  }

  if (g2.points)
  {
    vishnevskiy::LIter<std::string> it2(g2.points);
    while (it2.hasNext())
    {
      std::string point = it2.value();

      if (!hasElList(newGraph.points, point))
      {
        if (!newGraph.points)
        {
          newGraph.points = new vishnevskiy::List<std::string>();
          newGraph.points->val = point;
          newGraph.points->next = nullptr;
          newGraph.pointCount = 1;
        }
        else
        {
          vishnevskiy::LIter<std::string> e(newGraph.points);
          e.end();
          e.insert(point);
          newGraph.pointCount++;
        }
      }
      ++it2;
    }

    std::string point = it2.value();
    if (!hasElList(newGraph.points, point))
    {
      vishnevskiy::LIter<std::string> e(newGraph.points);
      e.end();
      e.insert(point);
      newGraph.pointCount++;
    }
  }

  vishnevskiy::tableIt<vishnevskiy::vertex, vishnevskiy::List<int>*, vHt, vEqt> it1(&g1.vertexes);
  while (it1.hasNext())
  {
    vishnevskiy::List<int>* ls = new vishnevskiy::List<int>(*it1.val());
    newGraph.vertexes.add(it1.key(), ls);
    it1.next();
  }

  vishnevskiy::tableIt<vishnevskiy::vertex, vishnevskiy::List<int>*, vHt, vEqt> it2(&g2.vertexes);
  while (it2.hasNext())
  {
    if (newGraph.vertexes.has(it2.key()))
    {
      vishnevskiy::List<int>* ls = newGraph.vertexes.at(it2.key());
      vishnevskiy::LIter<int> it(ls);
      it.end();
      vishnevskiy::List<int>* ls2 = it2.val();
      vishnevskiy::LIter<int> wit(ls2);
      while (wit.hasNext())
      {
        it.insert(wit.value());
        ++wit;
      }
      it.insert(wit.value());
    }
    else
    {
      vishnevskiy::List<int>* toAdd = new vishnevskiy::List<int>(*it2.val());
      newGraph.vertexes.add(it2.key(), toAdd);
    }
    it2.next();
  }
  graphtable.add(newname, newGraph);
}

void extract(std::ostream&, std::istream& i, graph_t& graphtable)
{
  std::string newname, oldname;
  size_t vertexCount;
  i >> newname >> oldname >> vertexCount;

  if (graphtable.has(newname) || !graphtable.has(oldname))
  {
    i.clear();
    throw std::logic_error("Incorrect name!");
  }

  vishnevskiy::graph& old = graphtable.at(oldname);
  vishnevskiy::graph newGraph(newname, 0);

  std::string* vertexNames = new std::string[vertexCount];
  for (size_t j = 0; j < vertexCount; ++j)
  {
    i >> vertexNames[j];
    if (!newGraph.points)
    {
      newGraph.points = new vishnevskiy::List<std::string>();
      newGraph.points->val = vertexNames[j];
      newGraph.points->next = nullptr;
      newGraph.pointCount = 1;
    }
    else if (!hasElList(newGraph.points, vertexNames[j]))
    {
      vishnevskiy::LIter<std::string> it(newGraph.points);
      it.end();
      it.insert(vertexNames[j]);
      newGraph.pointCount++;
    }
  }

  vishnevskiy::tableIt<vishnevskiy::vertex, vishnevskiy::List<int>*, vHt, vEqt> it(&old.vertexes);
  while (it.hasNext())
  {
    std::string from = it.key().in;
    std::string to = it.key().out;

    bool fromF = false;
    bool toF = false;
    for (size_t j = 0; j < vertexCount; ++j)
    {
      if (vertexNames[j] == from)
      {
        fromF = true;
      }
      if (vertexNames[j] == to)
      {
        toF = true;
      }
    }

    if (fromF && toF)
    {
      vishnevskiy::List<int>* newList = new vishnevskiy::List<int>(*it.val());
      newGraph.vertexes.add(it.key(), newList);
      newGraph.vertexCount++;
    }
    it.next();
  }

  delete[] vertexNames;
  graphtable.add(newname, newGraph);
}

int main(int argc, char* argv[])
{
  if (argc != 2)
  {
    return 1;
  }

  std::string filename = argv[1];

  using cm_t = void(*)(std::ostream&, std::istream&, graph_t&);
  using table_t = vishnevskiy::HashTable<std::string, cm_t, stringHash_t, stringEq_t>;
  graph_t graphtable(64, stringHasher, stringEq);

  try
  {
    load(filename, graphtable);
  }
  catch (const std::runtime_error&)
  {
    std::cerr << "Bad file!\n";
    return 1;
  }

  table_t cmds(9, stringHasher, stringEq);

  cmds.add("graphs", graphs);
  cmds.add("vertexes", vertexes);
  cmds.add("outbound", outbound);
  cmds.add("inbound", inbound);
  cmds.add("bind", bind);
  cmds.add("cut", cut);
  cmds.add("create", create);
  cmds.add("merge", merge);
  cmds.add("extract", extract);

  std::string cmd;
  while (std::cin >> cmd)
  {
    try
    {
      cmds.at(cmd)(std::cout, std::cin, graphtable);
    }
    catch (const std::runtime_error& e)
    {
      std::cout << "<INVALID COMMAND>\n";
      auto toignore = std::numeric_limits<std::streamsize>::max();
      std::cin.ignore(toignore, '\n');
    }
    catch (const std::out_of_range&)
    {
      std::cout << "<INVALID COMMAND>\n";
      auto toignore = std::numeric_limits<std::streamsize>::max();
      std::cin.ignore(toignore, '\n');
    }
    catch (const std::logic_error& e)
    {
      std::cout << "<INVALID COMMAND>\n";
      auto toignore = std::numeric_limits<std::streamsize>::max();
      std::cin.ignore(toignore, '\n');
    }
  }
  if (!std::cin.eof())
  {
    std::cerr << "Bad input!\n";
    return 1;
  }
  return 0;
}
