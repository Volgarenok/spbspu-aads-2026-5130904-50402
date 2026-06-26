#include "command.hpp"

namespace muhamadiarov
{
  const double INF = 1e18;
  static Robot robot;

  enum class Goal
  {
    PATH,
    TIME,
    GOLD,
    VISITALL
  };

  Graph& getGraph(GraphTable& graphs, std::string name)
  {
    if (!graphs.has(name))
    {
      throw std::runtime_error("getGraph:Graph not found");
    }
    return graphs.get(name);
  }

  struct DFSState;
  struct DFSResult;
  void stateToResult(const DFSState& state, DFSResult& best);

  struct DijkstraResult
  {
    List< int > path;
    List< Edge > edges;
    double totalCost;
    bool found;

    DijkstraResult():
      totalCost(0),
      found(false)
    {}
  };

  DijkstraResult dijkstra(
    const Graph& graph,
    int start,
    int end,
    Goal goal
  )
  {
    DijkstraResult result;

    if (!graph.findVertex(start) || !graph.findVertex(end))
    {
      return result;
    }

    const List< int >& vertices = graph.getVertices();
    Vector< int > vertList;
    LCIter< int > it = vertices.cbegin();
    for (size_t i = 0; i < vertices.size(); ++i)
    {
      vertList.pushBack(*it);
      ++it;
    }

    Vector< double > dist;
    Vector< bool > visited;
    Vector< int > prev;
    Vector< Edge > prevEdge;

    for (size_t i = 0; i < vertList.size(); ++i)
    {
      dist.pushBack(INF);
      visited.pushBack(false);
      prev.pushBack(-1);
      prevEdge.pushBack(Edge());
    }

    int startIdx = -1;
    int endIdx = -1;
    for (size_t i = 0; i < vertList.size(); ++i)
    {
      if (vertList[i] == start)
      {
        startIdx = i;
      }
      if (vertList[i] == end)
      {
        endIdx = i;
      }
    }

    if (startIdx == -1 || endIdx == -1)
    {
      return result;
    }

    dist[startIdx] = 0;

    for (size_t count = 0; count < vertList.size(); ++count)
    {
      int current = -1;
      double minDist = INF;

      for (size_t i = 0; i < vertList.size(); ++i)
      {
        if (!visited[i] && dist[i] < minDist)
        {
          minDist = dist[i];
          current = i;
        }
      }

      if (current == -1 || minDist == INF)
      {
        break;
      }
      if (current == endIdx)
      {
        break;
      }

      visited[current] = true;
      int currentVertex = vertList[current];

      List< std::pair< int, Edge > > outgoing = graph.getOutBounds(currentVertex);
      LCIter< std::pair< int, Edge > > edgeIt = outgoing.cbegin();
      for (size_t i = 0; i < outgoing.size(); ++i)
      {
        int neighbor = edgeIt->first;
        const Edge& edge = edgeIt->second;

        int neighborIdx = -1;
        for (size_t j = 0; j < vertList.size(); ++j)
        {
          if (vertList[j] == neighbor)
          {
            neighborIdx = j;
            break;
          }
        }

        double energyCost = edge.getEnergy(robot.getSpeed());
        double dopCost = 0.0;
        bool enough = true;
        double simEnergy = robot.getCurrentEnergy();
        while (simEnergy < energyCost)
        {
          if (simEnergy >= robot.getMaxEnergy())
          {
            enough = false;
            break;
          }
          simEnergy += robot.getRecoveryRate();
          dopCost   += 1.0;
          if (simEnergy > robot.getMaxEnergy())
          {
            simEnergy = robot.getMaxEnergy();
          }
        }
        if (!enough)
        {
          ++edgeIt;
          continue;
        }

        double cost;
        if (goal == Goal::TIME)
        {
          cost = edge.getTime(robot.getSpeed()) + dopCost;
        }
        else if (goal == Goal::PATH)
        {
          cost = static_cast< double >(edge.distance_);
        }
        else
        {
          int value;
          if (graph.getPointValue(neighbor, value))
          {
            cost = static_cast< double >(value);
          }
        }

        if (dist[current] + cost < dist[neighborIdx])
        {
          dist[neighborIdx] = dist[current] + cost;
          prev[neighborIdx] = current;
          prevEdge[neighborIdx] = edge;
        }
        ++edgeIt;
      }
    }

    if (dist[endIdx] == INF)
    {
      return result;
    }

    Vector< int > pathReversed;
    Vector< Edge > edgesReversed;

    int currentIdx = endIdx;
    while (currentIdx != -1)
    {
      pathReversed.pushBack(vertList[currentIdx]);
      if (prev[currentIdx] != -1)
      {
        edgesReversed.pushBack(prevEdge[currentIdx]);
      }
      currentIdx = prev[currentIdx];
    }

    for (int i = static_cast< int >(pathReversed.size()) - 1; i >= 0; --i) {
      result.path.pushBack(pathReversed[i]);
    }

    for (int i = static_cast< int >(edgesReversed.size()) - 1; i >= 0; --i) {
      result.edges.pushBack(edgesReversed[i]);
    }

    result.totalCost = dist[endIdx];
    result.found = true;

    return result;
  }

  void printPathWithEdges(const List< int >& path, const List< Edge >& edges, std::ostream& out)
  {
    LCIter< int > vertexIt = path.cbegin();
    LCIter< Edge > edgeIt = edges.cbegin();
    for (size_t i = 0; i < edges.size(); ++i)
    {
      int from = *vertexIt;
      const Edge& edge = *edgeIt;
      char typeChar = roadTypeToChar(edge.type_);
      out << from << " " << edge.to_ << " " << typeChar << " " << edge.distance_ << '\n';
      ++edgeIt;
      ++vertexIt;
    }
  }

  Graph createPathGraph(const List< int >& path, const List< Edge >& edges)
  {
    Graph pathGraph;
    LCIter< int > vertexIt = path.cbegin();
    for (size_t i = 0; i < path.size(); ++i)
    {
      pathGraph.addVertex(*vertexIt);
      ++vertexIt;
    }

    LCIter< Edge > edgeIt = edges.cbegin();
    LCIter< int > fromIt = path.cbegin();
    for (size_t i = 0; i < edges.size(); ++i)
    {
      char typeChar = roadTypeToChar((*edgeIt).type_);
      pathGraph.addConnection(*fromIt, (*edgeIt).to_, typeChar, (*edgeIt).distance_);
      ++fromIt;
      ++edgeIt;
    }
    return pathGraph;
  }

  bool checkCond(size_t value, size_t target, const std::string& op)
  {
    if (op == "-eq")
    {
      return value == target;
    }
    else if (op == "-ne")
    {
      return value != target;
    }
    else if (op == "-gt")
    {
      return value > target;
    }
    else if (op == "-lt")
    {
      return value < target;
    }
    else if (op == "-ge")
    {
      return value >= target;
    }
    else if (op == "-le")
    {
      return value <= target;
    }
    throw std::invalid_argument("checkCondition: incorrect operators");
  }

  std::string operators[] = {"-eq", "-ne", "-gt", "-lt", "-ge", "-le"};

  bool isTrueOp(const std::string& op)
  {
    for (size_t i = 0; i < 6; ++i)
    {
      if (op == operators[i])
      {
        return true;
      }
    }
    return false;
  }

  struct DFSState
  {
    int current;
    double time;
    int gold;
    int visitedCount;
    List< int > path;
    List< Edge > edges;
    RobinTable< int, bool > visited;
    std::string op;
    size_t target;

    DFSState(int v, std::string oprt = "", size_t count = 0):
      current(v),
      time(0),
      gold(0),
      visitedCount(1),
      op(oprt),
      target(count)
    {
      path.pushBack(v);
      visited.add(v, true);
    }
    DFSState(const DFSState& other) = default;
  };

  struct DFSResult
  {
    List< int > path;
    List< Edge > edges;
    double totalTime;
    int totalGold;
    bool found;

    DFSResult():
      totalTime(0),
      totalGold(0),
      found(false)
    {}
  };

  void stateToResult(const DFSState& state, DFSResult& best)
  {
    best.path = state.path;
    best.edges = state.edges;
    best.totalTime = state.time;
    best.totalGold = state.gold;
    best.found = true;
  }

  void dfs(
    const Graph& graph,
    DFSState& state,
    DFSResult& best,
    Goal mode,
    int end,
    double timeLimit,
    int target
  )
  {
    if (state.current == end && mode == Goal::GOLD)
    {
      if (isTrueOp(state.op) && checkCond(state.visitedCount, state.target, state.op))
      {
        stateToResult(state, best);
      }
      else if (state.gold > best.totalGold)
      {
        stateToResult(state, best);
      }
      return;
    }

    if (mode == Goal::GOLD && state.time >= timeLimit && !isTrueOp(state.op))
    {
      return;
    }

    if (mode == Goal::VISITALL && state.visitedCount == target - 1)
    {
      if (state.time < best.totalTime || !best.found)
      {
        stateToResult(state, best);
      }
      return;
    }

    List< std::pair< int, Edge > > outgoing = graph.getOutBounds(state.current);
    LCIter< std::pair< int, Edge > > edgeIt = outgoing.cbegin();

    for (size_t i = 0; i < outgoing.size(); ++i)
    {
      int neighbor = edgeIt->first;
      const Edge& edge = edgeIt->second;

      if (state.visited.has(neighbor) && state.visited.get(neighbor))
      {
        ++edgeIt;
        continue;
      }

      double energyCost = edge.getEnergy(robot.getSpeed());
      double dopCost = 0.0;
      bool enough = true;
      double simEnergy = robot.getCurrentEnergy();
      while (simEnergy < energyCost)
      {
        if (simEnergy >= robot.getMaxEnergy())
        {
          enough = false;
          break;
        }
        simEnergy += robot.getRecoveryRate();
        dopCost   += 1.0;
        if (simEnergy > robot.getMaxEnergy())
        {
          simEnergy = robot.getMaxEnergy();
        }
      }
      if (!enough)
      {
        ++edgeIt;
        continue;
      }

      double newTime = state.time + edge.getTime(robot.getSpeed()) + dopCost;
      if (mode == Goal::GOLD && newTime > timeLimit && !isTrueOp(state.op))
      {
        ++edgeIt;
        continue;
      }

      DFSState next = state;
      next.current = neighbor;
      next.time = newTime;
      next.visitedCount++;
      next.visited.add(neighbor, true);
      next.path.pushBack(neighbor);
      next.edges.pushBack(edge);

      int value = 0;
      if (graph.getPointValue(neighbor, value))
      {
        next.gold += value;
      }

      dfs(graph, next, best, mode, end, timeLimit, target);
      ++edgeIt;
    }
  }

  void dfsSpeed(
    const Graph& graph,
    DFSState& state,
    DFSResult& best,
    int end
  )
  {
    if (state.current == end)
    {
      bool condOk = checkCond(state.visitedCount, state.target, state.op);
      if (condOk && (!best.found || state.time < best.totalTime))
      {
        stateToResult(state, best);
      }
      return;
    }

    List< std::pair< int, Edge > > outgoing = graph.getOutBounds(state.current);
    LCIter< std::pair< int, Edge > > edgeIt = outgoing.cbegin();
    for (size_t i = 0; i < outgoing.size(); ++i)
    {
      int neighbor = edgeIt->first;
      const Edge& edge = edgeIt->second;
      if (state.visited.has(neighbor) && state.visited.get(neighbor))
      {
        ++edgeIt;
        continue;
      }

      double energyCost = edge.getEnergy(robot.getSpeed());
      double dopCost = 0.0;
      bool enough = true;
      double simEnergy = robot.getCurrentEnergy();
      while (simEnergy < energyCost)
      {
        if (simEnergy >= robot.getMaxEnergy())
        {
          enough = false;
          break;
        }
        simEnergy += robot.getRecoveryRate();
        dopCost   += 1.0;
        if (simEnergy > robot.getMaxEnergy())
        {
          simEnergy = robot.getMaxEnergy();
        }
      }
      if (!enough)
      {
        ++edgeIt;
        continue;
      }


      DFSState next = state;
      next.current = neighbor;
      next.time = state.time + edge.getTime(robot.getSpeed()) + dopCost;
      next.visitedCount++;
      next.visited.add(neighbor, true);
      next.path.pushBack(neighbor);
      next.edges.pushBack(edge);

      dfsSpeed(graph, next, best, end);
      ++edgeIt;
    }
  }
}

void muh::makeRobot(std::istream& in, GraphTable&)
{
  double maxEnergy, recoveryRate, speed;
  if (!(in >> maxEnergy >> recoveryRate >> speed))
  {
    throw std::runtime_error("makeRobot: error input");
  }
  robot = Robot(maxEnergy, recoveryRate, speed);
}

void muh::setRobot(std::istream& in, GraphTable&)
{
  std::string param;
  double value;
  if (!(in >> param >> value))
  {
    throw std::runtime_error("setRobot: error input");
  }

  if (param == "-maxEnergy")
  {
    robot.setMaxEnergy(value);
  }
  else if (param == "-recoveryRate")
  {
    robot.setRecoveryRate(value);
  }
  else if (param == "-speed")
  {
    robot.setSpeed(value);
  }
  else
  {
    throw std::invalid_argument("setRobot:invalid argument <t1>");
  }
}

void muh::addVertex(std::istream& in, GraphTable& graphs)
{
  std::string graphName;
  int vertex, value;

  if (!(in >> graphName >> vertex >> value))
  {
    throw std::runtime_error("addVertex: error input");
  }

  Graph& g = getGraph(graphs, graphName);
  g.addVertex(vertex);
  g.setPointValue(vertex, value);
}

void muh::rmVertex(std::istream& in, GraphTable& graphs)
{
  std::string graphName;
  int vertex;

  if (!(in >> graphName >> vertex))
  {
    throw std::runtime_error("rmVertex: error input");
  }

  Graph& g = getGraph(graphs, graphName);
  g.removeVertex(vertex);
}

void muh::setVertex(std::istream& in, GraphTable& graphs)
{
  std::string graphName;
  int vertex;
  size_t value;

  if (!(in >> graphName >> vertex >> value))
  {
    throw std::runtime_error("rmVertex: error input");
  }

  Graph& g = getGraph(graphs, graphName);
  g.setPointValue(vertex, value);
}

void muh::addEdge(std::istream& in, GraphTable& graphs)
{
  std::string graphName;
  int from, to;
  char type;
  size_t distance;

  if (!(in >> graphName >> from >> to >> type >> distance))
  {
    throw std::runtime_error("addEdge: error input");
  }

  Graph& g = getGraph(graphs, graphName);
  g.addConnection(from, to, type, distance);
}

void muh::rmEdge(std::istream& in, GraphTable& graphs)
{
  std::string graphName;
  int from, to;
  char type;
  size_t distance;

  if (!(in >> graphName >> from >> to >> type >> distance))
  {
    throw std::runtime_error("rmEdge: error input");
  }

  Graph& g = getGraph(graphs, graphName);
  g.removeConnection(from, to, type, distance);
}

void muh::setEdge(std::istream& in, GraphTable& graphs)
{
  std::string mode;
  std::string graphName;
  int from, to;
  char oldType;
  size_t oldDistance;

  if (!(in >> mode >> graphName >> from >> to >> oldType >> oldDistance))
  {
    throw std::runtime_error("setEdge: error input");
  }

  Graph& g = getGraph(graphs, graphName);
  if (!g.findConnection(from, to, oldType, oldDistance))
  {
    throw std::invalid_argument("setEdge: not found this connection");
  }

  g.removeConnection(from, to, oldType, oldDistance);
  if (mode == "-type")
  {
    char newType;
    if (!(in >> newType))
    {
      throw std::runtime_error("setEdge: error input");
    }
    g.addConnection(from, to, newType, oldDistance);
  }
  else if (mode == "-distance")
  {
    size_t newDistance;
    if (!(in >> newDistance))
    {
      throw std::runtime_error("setEdge: error input");
    }
    g.addConnection(from, to, oldType, newDistance);
  }
  else if (mode == "-all")
  {
    char newType;
    size_t newDistance;
    if (!(in >> newType >>newDistance))
    {
      throw std::runtime_error("setEdge: error input");
    }
    g.addConnection(from, to, newType, newDistance);
  }
  else
  {
    throw std::invalid_argument("setEdge: incorrect type");
  }
}

muh::Graph muh::findPath(std::istream& in, std::ostream& out, GraphTable& graphs)
{
  std::string graphName;
  int start, end;
  if (!(in >> graphName >> start >> end))
  {
    throw std::runtime_error("findPath: error input");
  }

  Graph& g = getGraph(graphs, graphName);
  if (!g.findVertex(start) || !g.findVertex(end))
  {
    throw std::invalid_argument("findPath: not found those vertexes");
  }

  DijkstraResult result = dijkstra(g, start, end, Goal::PATH);
  robot.resetEnergy();

  if (!result.found)
  {
    out << "<NO FOUND>\n";
    return Graph();
  }
  printPathWithEdges(result.path, result.edges, out);
  return createPathGraph(result.path, result.edges);
}

muh::Graph muh::findFastes(std::istream& in, std::ostream& out, GraphTable& graphs)
{
  std::string graphName;
  int start, end;
  if (!(in >> graphName >> start >> end))
  {
    throw std::runtime_error("findFastes: error input");
  }

  Graph& g = getGraph(graphs, graphName);
  if (!g.findVertex(start) || !g.findVertex(end))
  {
    throw std::invalid_argument("findFastes: not found those vertexes");
  }

  DijkstraResult result = dijkstra(g, start, end, Goal::TIME);
  robot.resetEnergy();

  if (!result.found)
  {
    out << "<NO FOUND>\n";
    return Graph();
  }
  printPathWithEdges(result.path, result.edges, out);
  return createPathGraph(result.path, result.edges);
}

muh::Graph muh::findGold(std::istream& in, std::ostream& out, GraphTable& graphs)
{
  std::string graphName;
  int start, end;
  double timeLimit;
  if (!(in >> graphName >> start >> end >> timeLimit))
  {
    throw std::runtime_error("findGold: error input");
  }

  Graph& g = getGraph(graphs, graphName);

  if (!g.findVertex(start) || !g.findVertex(end))
  {
    throw std::invalid_argument("findGold: not found those vertexes");
  }

  DFSState initialState(start);
  DFSResult result;
  dfs(g, initialState, result, Goal::GOLD, end, timeLimit, 0);
  robot.resetEnergy();
  if (!result.found)
  {
    out << "<NO FOUND>\n";
    return Graph();
  }
  out << "Gold: " << result.totalGold << '\n';
  printPathWithEdges(result.path, result.edges, out);
  return createPathGraph(result.path, result.edges);
}

muh::Graph muh::findVisitall(std::istream& in, std::ostream& out, GraphTable& graphs)
{
  std::string graphName;
  int start;
  if (!(in >> graphName >> start))
  {
    throw std::runtime_error("findVisitall: error input");
  }

  Graph& g = getGraph(graphs, graphName);
  if (!g.findVertex(start))
  {
    throw std::invalid_argument("findVisitall: not found those vertexes");
  }
  if (g.getVertices().size() <= 1)
  {
    throw std::logic_error("findVisitall: isn`t landscape");
  }

  int targetCount = g.getVertices().size();
  DFSState initialState(start);
  DFSResult result;
  dfs(g, initialState, result, Goal::VISITALL, -1, 0.0, targetCount);
  robot.resetEnergy();
  if (!result.found)
  {
    out << "<NO FOUND>\n";
    return Graph();
  }
  out << "Time: " << result.totalTime << '\n';
  printPathWithEdges(result.path, result.edges, out);
  return createPathGraph(result.path, result.edges);
}

muh::Graph muh::findCertain(std::istream& in, std::ostream& out, GraphTable& graphs)
{
  std::string graphName, mode, op;
  int start, end;
  size_t count;
  if (!(in >> mode >> graphName >> start >> end >> op >> count))
  {
    throw std::runtime_error("findGold: error input");
  }

  Graph& g = getGraph(graphs, graphName);

  if (!g.findVertex(start) || !g.findVertex(end) || !isTrueOp(op))
  {
    throw std::invalid_argument("findGold: invalid arguments");
  }

  List< int > path;
  List< Edge > edges;
  bool found = true;
  if (mode == "-gold")
  {
    DFSState initialState(start, op, count);
    DFSResult result;
    dfs(g, initialState, result, Goal::GOLD, end, INF, 0);
    if (result.found)
    {
      path = result.path;
      edges = result.edges;
      found = result.found;
      out << "Gold: " << result.totalGold << '\n';
    }
  }
  else if (mode == "-speed")
  {
    DFSResult speedResult;
    DFSState  speedState(start, op, count);
    dfsSpeed(g, speedState, speedResult, end);
    if (speedResult.found)
    {
      path  = speedResult.path;
      edges = speedResult.edges;
      found = true;
      out << "Time: " << speedResult.totalTime << '\n';
    }
  }
  else
  {
    throw std::invalid_argument("findGold: incorrect mode");
  }
  robot.resetEnergy();
  if (!found)
  {
    out << "<NO FOUND>\n";
    return Graph();
  }
  printPathWithEdges(path, edges, out);
  return createPathGraph(path, edges);
}
