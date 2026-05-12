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

BOOST_AUTO_TEST_SUITE(CommandTests)

BOOST_AUTO_TEST_CASE(test_create_and_remove_item)
{
  Engine engine;
  std::string out = captureOutput(engine, "create-item key INVENTORY \"Rusty key\"");
  BOOST_CHECK(out.find("<ITEM CREATED: key>") != std::string::npos);

  out = captureOutput(engine, "remove-item key");
  BOOST_CHECK(out.find("<ITEM REMOVED: key>") != std::string::npos);

  out = captureOutput(engine, "remove-item key");
  BOOST_CHECK(out.find("<INVALID COMMAND>") != std::string::npos);
}

BOOST_AUTO_TEST_CASE(test_create_scene_and_link)
{
  Engine engine;
  captureOutput(engine, "create-game quest1 \"Test\"");
  captureOutput(engine, "load-project quest1");

  std::string out = captureOutput(engine, "create-scene tavern \"You are in a tavern\"");
  BOOST_CHECK(out.find("<SCENE CREATED: tavern>") != std::string::npos);

  out = captureOutput(engine, "create-scene forest \"You are in a forest\"");
  BOOST_CHECK(out.find("<SCENE CREATED: forest>") != std::string::npos);

  out = captureOutput(engine, "link-scene tavern forest \"Go to forest\"");
  BOOST_CHECK(out.find("<LINK CREATED: tavern -> forest>") != std::string::npos);

  out = captureOutput(engine, "remove-scene tavern");
  BOOST_CHECK(out.find("<SCENE REMOVED: tavern>") != std::string::npos);
}

BOOST_AUTO_TEST_CASE(test_validation_pass)
{
  Engine engine;
  captureOutput(engine, "create-game quest3 \"Test\"");
  captureOutput(engine, "load-project quest3");
  captureOutput(engine, "create-scene a \"A\"");
  captureOutput(engine, "create-scene b \"B\"");
  captureOutput(engine, "link-scene a b \"Go\"");

  std::string out = captureOutput(engine, "validate");
  BOOST_CHECK(out.find("<STATUS: PASSED>") != std::string::npos);
}

BOOST_AUTO_TEST_CASE(test_validation_fail_unreachable)
{
  Engine engine;
  captureOutput(engine, "create-game quest4 \"Test\"");
  captureOutput(engine, "load-project quest4");
  captureOutput(engine, "create-scene a \"A\"");
  captureOutput(engine, "create-scene b \"B\"");

  std::string out = captureOutput(engine, "validate");
  BOOST_CHECK(out.find("<VALIDATION FAILED>") != std::string::npos);
  BOOST_CHECK(out.find("<ERROR: Scene \"b\" is unreachable>") != std::string::npos);
}

BOOST_AUTO_TEST_CASE(test_faq)
{
  Engine engine;
  std::string out = captureOutput(engine, "faq");
  BOOST_CHECK(out.find("<HELP: ") != std::string::npos);

  out = captureOutput(engine, "faq extra");
  BOOST_CHECK(out.find("<INVALID COMMAND>") != std::string::npos);
}

BOOST_AUTO_TEST_SUITE_END()
