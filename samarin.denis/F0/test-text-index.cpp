#include <boost/test/unit_test.hpp>
#include <sstream>
#include <string>
#include "text.hpp"
#include "text_index.hpp"

namespace {
  std::string roundTrip(const std::string& source)
  {
    std::istringstream in(source);
    samarin::Text text = samarin::parseText(in);
    samarin::TextIndex index;
    index.build(text);
    std::ostringstream out;
    samarin::writeText(out, index.restore());
    return out.str();
  }
}

BOOST_AUTO_TEST_SUITE(text_index)

BOOST_AUTO_TEST_CASE(restores_plain_text)
{
  const std::string source = "the quick brown fox\nthe lazy dog\n";
  BOOST_TEST(roundTrip(source) == source);
}

BOOST_AUTO_TEST_CASE(keeps_empty_lines)
{
  const std::string source = "hello world\n\nfoo bar\n";
  BOOST_TEST(roundTrip(source) == source);
}

BOOST_AUTO_TEST_CASE(collapses_spaces)
{
  BOOST_TEST(roundTrip("one  two   three\n") == "one two three\n");
}

BOOST_AUTO_TEST_CASE(handles_repeats)
{
  const std::string source = "go go go go\n";
  BOOST_TEST(roundTrip(source) == source);
}

BOOST_AUTO_TEST_CASE(empty_text)
{
  BOOST_TEST(roundTrip("") == "");
}

BOOST_AUTO_TEST_CASE(positions_and_lookup)
{
  std::istringstream in("a b a\nb a\n");
  samarin::Text text = samarin::parseText(in);
  samarin::TextIndex index;
  index.build(text);
  BOOST_TEST(index.has("a"));
  BOOST_TEST(index.has("b"));
  BOOST_TEST(!index.has("c"));
  BOOST_TEST(index.lineCount() == 2u);
}

BOOST_AUTO_TEST_SUITE_END()
