#include "pathfinding.hpp"

#include "array.hpp"
#include "hash-table.hpp"
#include "hashFunctions.hpp"
#include "location.hpp"

#include <limits>

namespace
{
template< class T >
void appendBack(matveev::List< T >& list, const T& value)
{
  matveev::LIter< T > prev = list.beforeBegin();
  matveev::LIter< T > it = list.begin();

  while (it != list.end())
  {
    ++prev;
    ++it;
  }

  list.insertAfter(prev, value);
}

bool solveRoute(const matveev::World& world, const std::string& from, const std::string& to,
  const matveev::Array< std::string >& targets, const matveev::Array< unsigned long long >& weights,
  unsigned long long required, matveev::List< std::string >& path_out, unsigned long long& cost_out)
{
  const unsigned long long infinity = std::numeric_limits< unsigned long long >::max();
  const std::size_t none = std::numeric_limits< std::size_t >::max();
  const std::size_t one = 1;

  if (required == 0)
  {
    return matveev::findShortestPath(world, from, to, path_out, cost_out);
  }

  std::size_t k = targets.size();

  unsigned long long available = 0;
  for (std::size_t i = 0; i < k; ++i)
  {
    available += weights[i];
  }

  if (available < required)
  {
    return false;
  }

  matveev::Array< unsigned long long > cost_s(k, infinity);
  matveev::Array< unsigned long long > cost_e(k, infinity);
  matveev::Array< unsigned long long > cost_t(k * k, infinity);

  for (std::size_t i = 0; i < k; ++i)
  {
    matveev::List< std::string > leg;
    unsigned long long c = 0;
    if (matveev::findShortestPath(world, from, targets[i], leg, c))
    {
      cost_s[i] = c;
    }

    matveev::List< std::string > leg_e;
    unsigned long long ce = 0;
    if (matveev::findShortestPath(world, targets[i], to, leg_e, ce))
    {
      cost_e[i] = ce;
    }

    for (std::size_t j = 0; j < k; ++j)
    {
      if (i == j)
      {
        cost_t[i * k + j] = 0;
        continue;
      }

      matveev::List< std::string > leg_t;
      unsigned long long ct = 0;
      if (matveev::findShortestPath(world, targets[i], targets[j], leg_t, ct))
      {
        cost_t[i * k + j] = ct;
      }
    }
  }

  std::size_t num_masks = one << k;
  matveev::Array< unsigned long long > mask_weight(num_masks, 0);
  for (std::size_t mask = 0; mask < num_masks; ++mask)
  {
    unsigned long long w = 0;
    for (std::size_t i = 0; i < k; ++i)
    {
      if ((mask & (one << i)) != 0)
      {
        w += weights[i];
      }
    }

    mask_weight[mask] = w;
  }

  matveev::Array< unsigned long long > dp(num_masks * k, infinity);
  matveev::Array< std::size_t > parent(num_masks * k, none);

  for (std::size_t i = 0; i < k; ++i)
  {
    if (cost_s[i] != infinity)
    {
      dp[(one << i) * k + i] = cost_s[i];
    }
  }

  for (std::size_t mask = 1; mask < num_masks; ++mask)
  {
    for (std::size_t i = 0; i < k; ++i)
    {
      if ((mask & (one << i)) == 0)
      {
        continue;
      }

      unsigned long long current = dp[mask * k + i];
      if (current == infinity)
      {
        continue;
      }

      for (std::size_t j = 0; j < k; ++j)
      {
        if ((mask & (one << j)) != 0)
        {
          continue;
        }

        unsigned long long edge = cost_t[i * k + j];
        if (edge == infinity)
        {
          continue;
        }

        std::size_t next_mask = mask | (one << j);
        unsigned long long candidate = current + edge;
        if (candidate < dp[next_mask * k + j])
        {
          dp[next_mask * k + j] = candidate;
          parent[next_mask * k + j] = i;
        }
      }
    }
  }

  unsigned long long best = infinity;
  std::size_t best_mask = 0;
  std::size_t best_end = none;

  for (std::size_t mask = 1; mask < num_masks; ++mask)
  {
    if (mask_weight[mask] < required)
    {
      continue;
    }

    for (std::size_t i = 0; i < k; ++i)
    {
      if (dp[mask * k + i] == infinity || cost_e[i] == infinity)
      {
        continue;
      }

      unsigned long long total = dp[mask * k + i] + cost_e[i];
      if (total < best)
      {
        best = total;
        best_mask = mask;
        best_end = i;
      }
    }
  }

  if (best_end == none)
  {
    return false;
  }

  cost_out = best;

  matveev::Array< std::size_t > order;
  std::size_t mask = best_mask;
  std::size_t node = best_end;
  while (true)
  {
    order.pushBack(node);
    std::size_t previous = parent[mask * k + node];
    if (previous == none)
    {
      break;
    }

    mask ^= (one << node);
    node = previous;
  }

  matveev::Array< std::string > sequence;
  sequence.pushBack(from);
  for (std::size_t s = order.size(); s-- > 0; )
  {
    sequence.pushBack(targets[order[s]]);
  }

  sequence.pushBack(to);

  for (std::size_t s = 0; s + 1 < sequence.size(); ++s)
  {
    matveev::List< std::string > leg;
    unsigned long long leg_cost = 0;
    matveev::findShortestPath(world, sequence[s], sequence[s + 1], leg, leg_cost);

    matveev::LIter< std::string > p = leg.begin();
    if (s != 0 && p != leg.end())
    {
      ++p;
    }

    for (; p != leg.end(); ++p)
    {
      appendBack(path_out, *p);
    }
  }

  return true;
}
}

bool matveev::findShortestPath(const World& world, const std::string& from, const std::string& to,
  List< std::string >& path_out, unsigned long long& cost_out)
{
  const unsigned long long infinity = std::numeric_limits< unsigned long long >::max();
  const std::size_t none = std::numeric_limits< std::size_t >::max();

  HashTable< std::string, std::size_t, StringHash, StringEqual > index_of(16);
  Array< std::string > names;

  std::size_t count = 0;
  for (LCIter< std::string > it = world.order().begin(); it != world.order().end(); ++it)
  {
    index_of.add(*it, count);
    names.pushBack(*it);
    ++count;
  }

  std::size_t from_index = index_of.at(from);
  std::size_t to_index = index_of.at(to);

  Array< unsigned long long > dist(count, infinity);
  Array< bool > visited(count, false);
  Array< std::size_t > previous(count, none);
  dist[from_index] = 0;

  for (std::size_t step = 0; step < count; ++step)
  {
    std::size_t current = none;
    unsigned long long best = infinity;

    for (std::size_t i = 0; i < count; ++i)
    {
      if (!visited[i] && dist[i] < best)
      {
        best = dist[i];
        current = i;
      }
    }

    if (current == none)
    {
      break;
    }

    visited[current] = true;

    const Location& loc = world.location(names[current]);
    for (LCIter< Connection > c = loc.connections.begin(); c != loc.connections.end(); ++c)
    {
      std::size_t neighbour = index_of.at(c->to);
      if (!visited[neighbour] && dist[current] + c->cost < dist[neighbour])
      {
        dist[neighbour] = dist[current] + c->cost;
        previous[neighbour] = current;
      }
    }
  }

  if (dist[to_index] == infinity)
  {
    return false;
  }

  Array< std::size_t > reverse_path;
  std::size_t cursor = to_index;
  reverse_path.pushBack(cursor);

  while (cursor != from_index)
  {
    cursor = previous[cursor];
    reverse_path.pushBack(cursor);
  }

  for (std::size_t k = reverse_path.size(); k-- > 0; )
  {
    appendBack(path_out, names[reverse_path[k]]);
  }

  cost_out = dist[to_index];
  return true;
}

bool matveev::findItemRoute(const World& world, const std::string& from, const std::string& to,
  const List< std::string >& items, List< std::string >& path_out, unsigned long long& cost_out)
{
  HashTable< std::string, char, StringHash, StringEqual > seen(16);
  Array< std::string > targets;

  for (LCIter< std::string > it = items.begin(); it != items.end(); ++it)
  {
    std::string location;
    if (!world.findItem(*it, location))
    {
      return false;
    }

    if (!seen.has(location))
    {
      seen.add(location, 0);
      targets.pushBack(location);
    }
  }

  Array< unsigned long long > weights(targets.size(), 1);
  return solveRoute(world, from, to, targets, weights, targets.size(), path_out, cost_out);
}

bool matveev::findTypeRoute(const World& world, const std::string& from, const std::string& to,
  std::size_t count, const std::string& type, List< std::string >& path_out, unsigned long long& cost_out)
{
  Array< std::string > targets;
  Array< unsigned long long > weights;

  for (LCIter< std::string > it = world.order().begin(); it != world.order().end(); ++it)
  {
    const Location& loc = world.location(*it);
    unsigned long long here = 0;
    for (LCIter< Item > item = loc.items.begin(); item != loc.items.end(); ++item)
    {
      if (item->type == type)
      {
        ++here;
      }
    }

    if (here > 0)
    {
      targets.pushBack(*it);
      weights.pushBack(here);
    }
  }

  return solveRoute(world, from, to, targets, weights, count, path_out, cost_out);
}
