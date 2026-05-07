#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <string>
#include "../common/Vector.hpp"
#include "../common/hashTable.hpp"

namespace karpovich
{
  class Engine
  {
  public:
    Engine();
    void processCommand(const std::string &line);

  private:
    using cmd_handler_t = void (Engine::*)(const Vector< std::string > &);
    HashTable< std::string, cmd_handler_t > command_table_;
    void initCommandTable();

    void cmdCreateItem(const Vector< std::string > &args);
    void cmdRemoveItem(const Vector< std::string > &args);
    void cmdAddAssetDb(const Vector< std::string > &args);
    void cmdRemoveAssetDb(const Vector< std::string > &args);
    void cmdShowDb(const Vector< std::string > &args);

    void cmdCreateGame(const Vector< std::string > &args);
    void cmdLoadProject(const Vector< std::string > &args);
    void cmdSaveProject(const Vector< std::string > &args);
    void cmdValidate(const Vector< std::string > &args);

    void cmdCreateScene(const Vector< std::string > &args);
    void cmdRemoveScene(const Vector< std::string > &args);
    void cmdLinkScene(const Vector< std::string > &args);
    void cmdUnlinkScene(const Vector< std::string > &args);
    void cmdAddObject(const Vector< std::string > &args);
    void cmdRemoveObject(const Vector< std::string > &args);
    void cmdSetInteract(const Vector< std::string > &args);

    void cmdStart(const Vector< std::string > &args);
    void cmdLook(const Vector< std::string > &args);
    void cmdInteract(const Vector< std::string > &args);
    void cmdChoice(const Vector< std::string > &args);
    void cmdShowInv(const Vector< std::string > &args);
    void cmdSaveGame(const Vector< std::string > &args);
    void cmdLoadGame(const Vector< std::string > &args);
    void cmdMode(const Vector< std::string > &args);
    void cmdFaq(const Vector< std::string > &args);
  };
}

#endif
