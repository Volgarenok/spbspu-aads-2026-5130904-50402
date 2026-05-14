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

BOOST_AUTO_TEST_SUITE_END()
