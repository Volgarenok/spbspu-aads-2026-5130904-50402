#include <sstream>
#include <string>
#include <boost/test/unit_test.hpp>
#include "engine.hpp"

using namespace karpovich;

namespace
{
  std::string captureOutput(Engine &engine, const std::string &cmd)
  {
    std::ostringstream oss;
    std::streambuf *old_buf = std::cout.rdbuf(oss.rdbuf());
    engine.processCommand(cmd);
    std::cout.rdbuf(old_buf);
    return oss.str();
  }
}

BOOST_AUTO_TEST_SUITE(GameplayTests)

BOOST_AUTO_TEST_CASE(test_game_flow)
{
  Engine engine;
  captureOutput(engine, "mode game");
  captureOutput(engine, "create-game quest2 \"Test\"");
  captureOutput(engine, "load-project quest2");
  captureOutput(engine, "create-scene start \"Start room\"");
  captureOutput(engine, "create-scene end \"End room\"");
  captureOutput(engine, "link-scene start end \"Proceed\"");
  captureOutput(engine, "create-item key INVENTORY \"Key\"");
  captureOutput(engine, "add-object start chest");
  captureOutput(engine, "set-interact start chest give:gold has:key");

  std::string out = captureOutput(engine, "start");
  BOOST_CHECK(out.find("<GAME STARTED>") != std::string::npos);

  out = captureOutput(engine, "interact chest");
  BOOST_CHECK(out.find("<NOTHING HAPPENS>") != std::string::npos);

  out = captureOutput(engine, "choice 0");
  BOOST_CHECK(out.find("<MOVED TO: end>") != std::string::npos);
}

BOOST_AUTO_TEST_CASE(test_invalid_mode)
{
  Engine engine;
  std::string out = captureOutput(engine, "start");
  BOOST_CHECK(out.find("<INVALID MODE>") != std::string::npos);

  out = captureOutput(engine, "mode editor");
  BOOST_CHECK(out.find("<MODE: EDITOR>") != std::string::npos);
}

BOOST_AUTO_TEST_SUITE_END()
