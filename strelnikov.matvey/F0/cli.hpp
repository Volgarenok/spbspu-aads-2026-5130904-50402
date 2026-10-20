#ifndef CLI_HPP
#define CLI_HPP
#include <string>
#include <vector>

namespace strelnikov
{
  struct Command
  {
    std::string name;
    std::vector< std::string > args;
    bool isValid = false;
    std::string errorMessage;
  };

  class Parser
  {
  public:
    Command parse(const std::string &);

  private:
    std::vector< std::string > tokenize(const std::string &);
    bool validateArgs(const std::string &, const std::vector< std::string > &, std::string &);
  };
}
#endif
