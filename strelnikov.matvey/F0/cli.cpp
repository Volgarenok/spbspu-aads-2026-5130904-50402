#include "cli.hpp"
#include <sstream>
#include <unordered_map>
#include <utility>

namespace strelnikov
{

  bool Parser::validateArgs(const std::string &cmdName, const std::vector< std::string > &args, std::string &errMsg)
  {
    static const std::unordered_map< std::string, std::pair< int, int > > argRules = {
        {"make-collection", {1, 1}},
        {"rm-collection", {1, 1}},
        {"add-file", {2, 2}},
        {"rm-file", {2, 2}},
        {"make-tag", {1, 1}},
        {"rm-tag", {1, 1}},
        {"give-tag", {2, 2}},
        {"withdraw-tag", {2, 2}},
        {"show", {1, 1}},
        {"sort", {2, 2}},
        {"check-files", {1, 2}},
        {"list-collections", {0, 0}},
        {"find-file-by-name", {1, 2}},
        {"find-file-by-prefix", {1, 2}},
        {"find-file-by-tag", {1, 2}},
        {"find-tag-by-name", {1, 1}},
        {"find-tag-by-prefix", {1, 1}},
        {"find-collection-by-name", {1, 1}},
        {"find-collection-by-prefix", {1, 1}},
        {"union-collections", {3, 3}},
        {"intersect-collections", {3, 3}},
        {"filter-by-tag", {3, 3}},
        {"undo", {0, 0}},
        {"dryUndo", {0, 0}},
        {"print-undo", {0, 0}},
        {"redo", {0, 0}},
        {"dryRedo", {0, 0}},
        {"print-redo", {0, 0}},
        {"history", {0, 0}},
        {"save", {1, 1}},
        {"load", {1, 1}},
    };

    auto it = argRules.find(cmdName);
    if (it == argRules.end()) {
      errMsg = "unknown command";
      return false;
    }

    const size_t actualArgs = args.size();
    if (actualArgs < static_cast< size_t >(it->second.first) || actualArgs > static_cast< size_t >(it->second.second)) {
      errMsg = "invalid number of arguments";
      return false;
    }

    return true;
  }

  std::vector< std::string > Parser::tokenize(const std::string &input)
  {
    std::vector< std::string > tokens;
    std::istringstream iss(input);
    std::string token;
    while (iss >> token) {
      tokens.push_back(token);
    }
    return tokens;
  }

  Command Parser::parse(const std::string &input)
  {
    Command cmd;
    std::vector< std::string > tokens = tokenize(input);

    if (tokens.empty()) {
      cmd.isValid = false;
      cmd.errorMessage = "empty input";
      return cmd;
    }

    cmd.name = tokens[0];
    cmd.args.assign(tokens.begin() + 1, tokens.end());
    cmd.isValid = validateArgs(cmd.name, cmd.args, cmd.errorMessage);

    return cmd;
  }
}
