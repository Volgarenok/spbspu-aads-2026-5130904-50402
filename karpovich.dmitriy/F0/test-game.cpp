#include <boost/test/tools/old/interface.hpp>
#include <boost/test/unit_test.hpp>
#include "engine.hpp"
using namespace karpovich;

namespace
{
  std::string runCommand(Engine &engine, const std::string &cmd)
  {
    std::ostringstream oss;
    std::streambuf *old_buf = std::cout.rdbuf(oss.rdbuf());
    try {
      engine.processCommand(cmd);
    } catch (...) {
      std::cout.rdbuf(old_buf);
      throw;
    }
    std::cout.rdbuf(old_buf);
    return oss.str();
  }
}
BOOST_AUTO_TEST_SUITE(GameplayTests)

BOOST_AUTO_TEST_CASE(test_mode_switching_and_start_restriction)
{
  Engine engine;

  std::string out = runCommand(engine, "mode editor");
  BOOST_CHECK(out.find("<MODE: EDITOR>") != std::string::npos);

  out = runCommand(engine, "start");
  BOOST_CHECK(out.find("<INVALID MODE>") != std::string::npos);

  out = runCommand(engine, "mode game");
  BOOST_CHECK(out.find("<MODE: GAME>") != std::string::npos);

  out = runCommand(engine, "faq");
  BOOST_CHECK(out.find("<HELP: ") != std::string::npos);
}

BOOST_AUTO_TEST_CASE(test_create_and_load_project)
{
  Engine engine;
  std::string out = runCommand(engine, "create-game quest_alpha \"Alpha Quest\"");
  BOOST_CHECK(out.find("<PROJECT CREATED: quest_alpha.dat>") != std::string::npos);
  out = runCommand(engine, "load-project quest_alpha");
  BOOST_CHECK(out.find("<PROJECT LOADED: quest_alpha>") != std::string::npos);
  BOOST_CHECK(out.find("<SCENES: 0, ITEMS: 0>") != std::string::npos);
  std::remove("quest_alpha.dat");
}

BOOST_AUTO_TEST_CASE(test_load_missing_project)
{
  Engine engine;
  std::string out = runCommand(engine, "load-project non_existent_proj");
  BOOST_CHECK(out.find("<ERROR: FILE NOT FOUND>") != std::string::npos);
}

BOOST_AUTO_TEST_CASE(test_duplicate_project_creation)
{
  Engine engine;
  runCommand(engine, "create-game quest_beta \"Beta Quest\"");
  std::string out = runCommand(engine, "create-game quest_beta \"Another Quest\"");
  BOOST_CHECK(out.find("<INVALID COMMAND>") != std::string::npos);

  std::remove("quest_beta.dat");
}

BOOST_AUTO_TEST_CASE(test_scene_management_and_validation)
{
  Engine engine;
  runCommand(engine, "create-game quest_gamma \"Gamma Quest\"");
  runCommand(engine, "load-project quest_gamma");

  std::string out = runCommand(engine, "create-scene hall \"Main Hall\"");
  BOOST_CHECK(out.find("<SCENE CREATED: hall>") != std::string::npos);

  out = runCommand(engine, "create-scene garden \"Secret Garden\"");
  BOOST_CHECK(out.find("<SCENE CREATED: garden>") != std::string::npos);

  out = runCommand(engine, "link-scene hall garden \"Go to garden\"");
  BOOST_CHECK(out.find("<LINK CREATED: hall -> garden>") != std::string::npos);

  out = runCommand(engine, "validate");
  BOOST_CHECK(out.find("<STATUS: PASSED>") != std::string::npos);

  out = runCommand(engine, "unlink-scene hall 0");
  BOOST_CHECK(out.find("<LINK REMOVED: index 0 from hall>") != std::string::npos);

  out = runCommand(engine, "validate");
  BOOST_CHECK(out.find("<VALIDATION FAILED>") != std::string::npos);
  BOOST_CHECK(out.find("<ERROR: Scene \"garden\" is unreachable>") != std::string::npos);

  out = runCommand(engine, "remove-scene garden");
  BOOST_CHECK(out.find("<SCENE REMOVED: garden>") != std::string::npos);

  std::remove("quest_gamma.dat");
}

BOOST_AUTO_TEST_CASE(test_gameplay_errors)
{
  Engine engine;
  std::string out = runCommand(engine, "start");
  BOOST_CHECK(out.find("<INVALID MODE>") != std::string::npos);

  out = runCommand(engine, "show-inv");
  BOOST_CHECK(out.find("<NOT IN GAME>") != std::string::npos);

  out = runCommand(engine, "save-game any_slot");
  BOOST_CHECK(out.find("<ERROR: CANNOT SAVE>") != std::string::npos);

  out = runCommand(engine, "load-game missing_slot");
  BOOST_CHECK(out.find("<ERROR: SAVE FILE NOT FOUND>") != std::string::npos);
}

BOOST_AUTO_TEST_CASE(test_gameplay_loop)
{
  Engine engine;
  std::string out = runCommand(engine, "create-game loop_quest \"Loop Quest\"");
  BOOST_CHECK(out.find("<PROJECT CREATED: loop_quest.dat>") != std::string::npos);

  out = runCommand(engine, "load-project loop_quest");
  BOOST_CHECK(out.find("<PROJECT LOADED: loop_quest>") != std::string::npos);

  runCommand(engine, "create-scene start \"Start area\"");
  runCommand(engine, "create-scene next \"Next area\"");
  runCommand(engine, "link-scene start next \"Go forward\"");
  runCommand(engine, "add-object start crate");
  runCommand(engine, "set-interact start crate give:key");
  runCommand(engine, "add-object start vault");
  runCommand(engine, "set-interact start vault give:gem has:key");

  runCommand(engine, "mode game");
  out = runCommand(engine, "start");
  BOOST_CHECK(out.find("<GAME STARTED>") != std::string::npos);

  out = runCommand(engine, "show-inv");
  BOOST_CHECK(out.find("<INVENTORY EMPTY>") != std::string::npos);

  out = runCommand(engine, "interact vault");
  BOOST_CHECK(out.find("<NOTHING HAPPENS>") != std::string::npos);

  out = runCommand(engine, "interact crate");
  BOOST_CHECK(out.find("<ITEM ADDED: key>") != std::string::npos);

  out = runCommand(engine, "interact vault");
  BOOST_CHECK(out.find("<ITEM ADDED: gem>") != std::string::npos);

  out = runCommand(engine, "show-inv");
  BOOST_CHECK(out.find("<INVENTORY: ") != std::string::npos);
  BOOST_CHECK(out.find("key x1") != std::string::npos);
  BOOST_CHECK(out.find("gem x1") != std::string::npos);

  out = runCommand(engine, "choice 0");
  BOOST_CHECK(out.find("<MOVED TO: next>") != std::string::npos);

  out = runCommand(engine, "choice 5");
  BOOST_CHECK(out.find("<INVALID CHOICE>") != std::string::npos);

  out = runCommand(engine, "save-game save_slot");
  BOOST_CHECK(out.find("<GAME SAVED: save_slot>") != std::string::npos);

  out = runCommand(engine, "load-game save_slot");
  BOOST_CHECK(out.find("<GAME LOADED: save_slot>") != std::string::npos);

  std::remove("loop_quest.dat");
  std::remove("save_slot.save");
}

BOOST_AUTO_TEST_SUITE_END()
