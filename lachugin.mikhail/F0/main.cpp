#include "commands.hpp"
#include "WordbookManager.hpp"
#include "CuckooHashTable.hpp"

#include <iostream>
#include <limits>
#include <string>
#include <functional>

int main()
{
  lachugin::WordbookManager manager;
  using Cmd = void(*)(std::istream&, std::ostream&, lachugin::WordbookManager&);
  using CommandTable = lachugin::CuckooHashTable< std::string, Cmd,lachugin::DefaultHash1< std::string >,
    lachugin::DefaultHash2< std::string >, std::equal_to< std::string > >;

  CommandTable commands;
  commands.add("make", lachugin::cmdMake);
  commands.add("delete", lachugin::cmdDelete);
  commands.add("addWord", lachugin::cmdAddWord);
  commands.add("remove", lachugin::cmdRemove);
  commands.add("addAnother", lachugin::cmdAddAnother);
  commands.add("addInterpretation", lachugin::cmdAddInterpretation);
  commands.add("removeInterpretation", lachugin::cmdRemoveInterpretation);
  commands.add("divWord", lachugin::cmdDivWord);
  commands.add("show", lachugin::cmdShow);
  commands.add("showTranslate", lachugin::cmdShowTranslate);
  commands.add("showStartsWith", lachugin::cmdShowStartsWith);
  commands.add("showPartsOfSpeech", lachugin::cmdShowPartsOfSpeech);
  commands.add("merge", lachugin::cmdMerge);

  std::string cmd;
  while (std::cin >> cmd)
  {
    try
    {
      commands.get(cmd)
      (std::cin, std::cout, manager);
    }
    catch (const std::exception&)
    {
      std::cout << "<INVALID COMMAND>\n";
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }

}
