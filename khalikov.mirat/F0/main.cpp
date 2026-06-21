#include <utility>
#include <functional>
#include <string>
#include "commands.hpp"
#include "RBTree.hpp"
#include "../common/hashtable.hpp"
#include "../common/hasher.hpp"

auto f = std::bind(std::less< std::string >(), std::bind(&pair_t::first, std::placeholders::_1),
                   std::bind(&pair_t::first, std::placeholders::_2));

using hash_t =
    khalikov::HashTable< std::string, cmd_t, khalikov::SipHash< std::string >, std::equal_to< std::string > >;

int main()
{
  Storage mxs(f);
  hash_t cmds;
  cmds["create"] = khalikov::create;
  cmds["show"] = khalikov::show;
  cmds["fill"] = khalikov::fill;
  cmds["add"] = khalikov::add;
  cmds["mul"] = khalikov::mul;
  cmds["sub"] = khalikov::sub;
  cmds["remove"] = khalikov::remove;
  cmds["list"] = khalikov::list;
  cmds["clear"] = khalikov::clear;
  cmds["basech"] = khalikov::basech;
  cmds["transpose"] = khalikov::transpose;
  cmds["eye"] = khalikov::eye;
  cmds["scale"] = khalikov::scale;
  cmds["det"] = khalikov::det;
  cmds["rotate-r"] = khalikov::rotateR;
  cmds["rotate-l"] = khalikov::rotateL;
  cmds["merge-v"] = khalikov::mergeV;
  cmds["merge-h"] = khalikov::mergeH;
  cmds["swap-r"] = khalikov::swapR;
  cmds["swap-c"] = khalikov::swapC;
  cmds["square"] = khalikov::square;
  cmds["split-v"] = khalikov::splitV;
  cmds["split-h"] = khalikov::splitH;
  cmds["reshape"] = khalikov::reshape;
  std::string cmd;
  while (std::cin >> std::ws >> cmd) {
    try {
      cmds.at(cmd)(std::cout, std::cin, mxs);
    } catch (...) {
      std::cout << "<INVALID COMMAND>\n";
      auto toignore = std::numeric_limits< std::streamsize >::max();
      std::cin.ignore(toignore, '\n');
    }
  }
  if (!std::cin.eof()) {
    std::cerr << "Bad input\n";
    return 1;
  }
}
