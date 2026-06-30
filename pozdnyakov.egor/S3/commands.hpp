#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <string>
#include <utility>
#include "graph.hpp"
#include "vector.hpp"

namespace pozdnyakov
{
  Graph *findGraph(Vector< std::pair< std::string, Graph > > &dict, const std::string &name);
  void printInvalid();
  void cmdGraphs(Vector< std::pair< std::string, Graph > > &dict, const Vector< std::string > &tokens);
  void cmdVertexes(Vector< std::pair< std::string, Graph > > &dict, const Vector< std::string > &tokens);
  void cmdOutbound(Vector< std::pair< std::string, Graph > > &dict, const Vector< std::string > &tokens);
  void cmdInbound(Vector< std::pair< std::string, Graph > > &dict, const Vector< std::string > &tokens);
  void cmdBind(Vector< std::pair< std::string, Graph > > &dict, const Vector< std::string > &tokens);
  void cmdCut(Vector< std::pair< std::string, Graph > > &dict, const Vector< std::string > &tokens);
  void cmdCreate(Vector< std::pair< std::string, Graph > > &dict, const Vector< std::string > &tokens);
  void cmdMerge(Vector< std::pair< std::string, Graph > > &dict, const Vector< std::string > &tokens);
  void cmdExtract(Vector< std::pair< std::string, Graph > > &dict, const Vector< std::string > &tokens);

  typedef void (*CommandFn)(Vector< std::pair< std::string, Graph > > &, const Vector< std::string > &);

  class CommandDispatcher
  {
  private:
    Vector< std::pair< std::string, CommandFn > > cmds_;

  public:
    void add(const std::string &name, CommandFn fn);

    void execute(const std::string &name, Vector< std::pair< std::string, Graph > > &dict,
                 const Vector< std::string > &tokens);
  };
}

#endif
