#include <boost/test/unit_test.hpp>
#include <sstream>
#include "commands.hpp"

using namespace afanasev;

BOOST_AUTO_TEST_SUITE(test_commands)

std::string runCmd(NoteSet& ns, const std::string& cmdLine) {
  std::istringstream in(cmdLine);
  std::ostringstream out;
  std::string cmd;
  in >> cmd;
  if (cmd == "cr") cmdCr(in, out, ns);
  else if (cmd == "str") cmdStr(in, out, ns);
  else if (cmd == "del") cmdDel(in, out, ns);
  else if (cmd == "delk") cmdDelk(in, out, ns);
  else if (cmd == "see") cmdSee(in, out, ns);
  else if (cmd == "link") cmdLink(in, out, ns);
  else if (cmd == "deltagnote") cmdDeltagNote(in, out, ns);
  else if (cmd == "seetag") cmdSeetag(in, out, ns);
  else if (cmd == "seetagand") cmdSeetagAnd(in, out, ns);
  else if (cmd == "seetagor") cmdSeetagOr(in, out, ns);
  else if (cmd == "tagrp") cmdTagRp(in, out, ns);
  else if (cmd == "tagaddnew") cmdTagAddNew(in, out, ns);
  else if (cmd == "deltag") cmdDelTag(in, out, ns);
  else if (cmd == "tag") cmdTag(in, out, ns);
  else if (cmd == "tagdel") cmdTagDel(in, out, ns);
  else if (cmd == "getliked") cmdGetLiked(in, out, ns);
  else if (cmd == "addlinktag") cmdAddLinkTag(in, out, ns);
  else if (cmd == "dellinktag") cmdDelLinkTag(in, out, ns);
  return out.str();
}

BOOST_AUTO_TEST_CASE(test_cr_and_see) {
  NoteSet ns(16);
  BOOST_CHECK_EQUAL(runCmd(ns, "cr \"note1\""), "created \"note1\"\n");
  BOOST_CHECK_EQUAL(runCmd(ns, "see \"note1\""),
    "name: \"note1\"\ntags:\n");
  BOOST_CHECK_THROW(runCmd(ns, "cr \"note1\""), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(test_str) {
  NoteSet ns(16);
  runCmd(ns, "cr \"note\"");
  BOOST_CHECK_EQUAL(runCmd(ns, "str \"note\" \"hello world\""),
    "added line to \"note\"\n");
  std::string see = runCmd(ns, "see \"note\"");
  BOOST_TEST(see.find("1: hello world") != std::string::npos);
}

BOOST_AUTO_TEST_CASE(test_del) {
  NoteSet ns(16);
  runCmd(ns, "cr \"A\"");
  runCmd(ns, "cr \"B\"");
  runCmd(ns, "link \"A\" \"B\"");
  BOOST_CHECK_EQUAL(runCmd(ns, "del \"A\""), "\"A\" deleted\n");
  BOOST_CHECK_NO_THROW(runCmd(ns, "see \"B\""));
}

BOOST_AUTO_TEST_CASE(test_delk_depth0) {
  NoteSet ns(16);
  runCmd(ns, "cr \"A\"");
  runCmd(ns, "cr \"B\"");
  runCmd(ns, "link \"A\" \"B\"");
  BOOST_CHECK_EQUAL(runCmd(ns, "delk \"A\" 0"),
    "\"A\" subtree up to depth 0 deleted\n");
  BOOST_CHECK_THROW(runCmd(ns, "see \"A\""), std::runtime_error);
  BOOST_CHECK_THROW(runCmd(ns, "see \"B\""), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(test_delk_depth1) {
  NoteSet ns(16);
  runCmd(ns, "cr \"A\"");
  runCmd(ns, "cr \"B\"");
  runCmd(ns, "cr \"C\"");
  runCmd(ns, "link \"A\" \"B\"");
  runCmd(ns, "link \"B\" \"C\"");
  BOOST_CHECK_EQUAL(runCmd(ns, "delk \"A\" 1"),
    "\"A\" subtree up to depth 1 deleted\n");
  BOOST_CHECK_NO_THROW(runCmd(ns, "see \"A\""));
  BOOST_CHECK_THROW(runCmd(ns, "see \"B\""), std::runtime_error);
  BOOST_CHECK_THROW(runCmd(ns, "see \"C\""), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(test_link) {
  NoteSet ns(16);
  runCmd(ns, "cr \"parent\"");
  runCmd(ns, "cr \"child\"");
  BOOST_CHECK_EQUAL(runCmd(ns, "link \"parent\" \"child\""),
    "\"parent\" linked \"child\"\n");
  std::string liked = runCmd(ns, "getliked \"parent\" 1");
  BOOST_TEST(liked.find("\"child\"") != std::string::npos);
}

BOOST_AUTO_TEST_CASE(test_tag_and_seetag) {
  NoteSet ns(16);
  runCmd(ns, "cr \"note\"");
  BOOST_CHECK_EQUAL(runCmd(ns, "tag \"note\" \"important\""),
    "tag: important added to \"note\"\n");
  std::string see = runCmd(ns, "see \"note\"");
  BOOST_TEST(see.find("tags: important") != std::string::npos);
  std::string seetag = runCmd(ns, "seetag \"important\"");
  BOOST_TEST(seetag.find("\"note\"") != std::string::npos);
}

BOOST_AUTO_TEST_CASE(test_tagdel) {
  NoteSet ns(16);
  runCmd(ns, "cr \"note\"");
  runCmd(ns, "tag \"note\" \"temp\"");
  BOOST_CHECK_EQUAL(runCmd(ns, "tagdel \"note\" \"temp\""),
    "tag removed from \"note\"\n");
  std::string see = runCmd(ns, "see \"note\"");
  BOOST_TEST(see.find("temp") == std::string::npos);
}

BOOST_AUTO_TEST_CASE(test_deltag) {
  NoteSet ns(16);
  runCmd(ns, "cr \"A\"");
  runCmd(ns, "cr \"B\"");
  runCmd(ns, "tag \"A\" \"common\"");
  runCmd(ns, "tag \"B\" \"common\"");
  BOOST_CHECK_EQUAL(runCmd(ns, "deltag \"common\""),
    "\"common\" deleted\n");
  BOOST_CHECK_EQUAL(runCmd(ns, "seetag \"common\""), "");
}

BOOST_AUTO_TEST_CASE(test_tagrp) {
  NoteSet ns(16);
  runCmd(ns, "cr \"X\"");
  runCmd(ns, "tag \"X\" \"old\"");
  BOOST_CHECK_EQUAL(runCmd(ns, "tagrp \"old\" \"new\""),
    "\"old\" replaced to \"new\" in 1 notes\n");
  std::string see = runCmd(ns, "see \"X\"");
  BOOST_TEST(see.find("new") != std::string::npos);
  BOOST_TEST(see.find("old") == std::string::npos);
}

BOOST_AUTO_TEST_CASE(test_tagaddnew) {
  NoteSet ns(16);
  runCmd(ns, "cr \"X\"");
  runCmd(ns, "tag \"X\" \"salad\"");
  BOOST_CHECK_EQUAL(runCmd(ns, "tagaddnew \"salad\" \"snack\""),
    "added \"snack\" to notes with \"salad\" (1 notes)\n");
  std::string see = runCmd(ns, "see \"X\"");
  BOOST_TEST(see.find("salad") != std::string::npos);
  BOOST_TEST(see.find("snack") != std::string::npos);
}

BOOST_AUTO_TEST_CASE(test_seetagand) {
  NoteSet ns(16);
  runCmd(ns, "cr \"A\"");
  runCmd(ns, "cr \"B\"");
  runCmd(ns, "tag \"A\" \"x\"");
  runCmd(ns, "tag \"A\" \"y\"");
  runCmd(ns, "tag \"B\" \"x\"");
  std::string out = runCmd(ns, "seetagand 2 \"x\" \"y\"");
  BOOST_TEST(out.find("\"A\"") != std::string::npos);
  BOOST_TEST(out.find("\"B\"") == std::string::npos);
}

BOOST_AUTO_TEST_CASE(test_seetagor) {
  NoteSet ns(16);
  runCmd(ns, "cr \"A\"");
  runCmd(ns, "cr \"B\"");
  runCmd(ns, "tag \"A\" \"x\"");
  runCmd(ns, "tag \"B\" \"y\"");
  std::string out = runCmd(ns, "seetagor 2 \"x\" \"y\"");
  BOOST_TEST(out.find("\"A\"") != std::string::npos);
  BOOST_TEST(out.find("\"B\"") != std::string::npos);
}

BOOST_AUTO_TEST_CASE(test_getliked) {
  NoteSet ns(16);
  runCmd(ns, "cr \"root\"");
  runCmd(ns, "cr \"child1\"");
  runCmd(ns, "cr \"child2\"");
  runCmd(ns, "link \"root\" \"child1\"");
  runCmd(ns, "link \"root\" \"child2\"");
  std::string out = runCmd(ns, "getliked \"root\" 1");
  BOOST_TEST(out.find("\"child1\"") != std::string::npos);
  BOOST_TEST(out.find("\"child2\"") != std::string::npos);
  BOOST_TEST(out.find("\"root\"") == std::string::npos);
  out = runCmd(ns, "getliked \"root\" 0");
  BOOST_TEST(out == "\"root\"\n");
}

BOOST_AUTO_TEST_CASE(test_deltagnote) {
  NoteSet ns(16);
  runCmd(ns, "cr \"A\"");
  runCmd(ns, "cr \"B\"");
  runCmd(ns, "tag \"A\" \"remove_me\"");
  runCmd(ns, "tag \"B\" \"keep\"");
  std::string out = runCmd(ns, "deltagnote \"remove_me\"");
  BOOST_TEST(out.find("deleted 1 notes") != std::string::npos);
  BOOST_CHECK_THROW(runCmd(ns, "see \"A\""), std::runtime_error);
  BOOST_CHECK_NO_THROW(runCmd(ns, "see \"B\""));
}

BOOST_AUTO_TEST_CASE(test_addlinktag_and_dellinktag) {
  NoteSet ns(16);
  runCmd(ns, "cr \"P\"");
  runCmd(ns, "cr \"C\"");
  runCmd(ns, "link \"P\" \"C\"");
  BOOST_CHECK_EQUAL(runCmd(ns, "addlinktag \"P\" 1 \"newtag\""),
    "added tag \"newtag\" to 1 note(s) at depth 1 from \"P\"\n");
  std::string seeC = runCmd(ns, "see \"C\"");
  BOOST_TEST(seeC.find("newtag") != std::string::npos);
  BOOST_CHECK_EQUAL(runCmd(ns, "dellinktag \"P\" 1 \"newtag\""),
    "removed tag \"newtag\" from 1 note(s) at depth 1 from \"P\"\n");
  seeC = runCmd(ns, "see \"C\"");
  BOOST_TEST(seeC.find("newtag") == std::string::npos);
}

BOOST_AUTO_TEST_SUITE_END()
