#include <boost/test/unit_test.hpp>
#include <sstream>
#include <string>
#include "text.hpp"
#include "text_ops.hpp"

namespace {
  samarin::Text make(const std::string& source)
  {
    std::istringstream in(source);
    return samarin::parseText(in);
  }

  std::string show(const samarin::Text& text)
  {
    std::ostringstream out;
    samarin::writeText(out, text);
    return out.str();
  }
}

BOOST_AUTO_TEST_SUITE(text_ops)

BOOST_AUTO_TEST_CASE(concat_joins)
{
  BOOST_TEST(show(samarin::concat(make("a b\n"), make("c\n"))) == "a b\nc\n");
}

BOOST_AUTO_TEST_CASE(concat_lines_merges)
{
  BOOST_TEST(show(samarin::concatLines(make("a b\nc d\n"), make("x\ny z\n"))) == "a b x\nc d y z\n");
}

BOOST_AUTO_TEST_CASE(repeat_vertical_stacks)
{
  BOOST_TEST(show(samarin::repeatVertical(make("a\n"), 3)) == "a\na\na\n");
}

BOOST_AUTO_TEST_CASE(repeat_horizontal_widens)
{
  BOOST_TEST(show(samarin::repeatHorizontal(make("a b\n"), 2)) == "a b a b\n");
}

BOOST_AUTO_TEST_CASE(interleave_lines_alternates)
{
  BOOST_TEST(show(samarin::interleaveLines(make("a\nb\n"), make("x\ny\n"))) == "a\nx\nb\ny\n");
}

BOOST_AUTO_TEST_CASE(interleave_words_alternates)
{
  BOOST_TEST(show(samarin::interleaveWords(make("a b\n"), make("x y z\n"))) == "a x b y z\n");
}

BOOST_AUTO_TEST_CASE(reverse_lines_flips)
{
  BOOST_TEST(show(samarin::reverseLines(make("a\nb\nc\n"))) == "c\nb\na\n");
}

BOOST_AUTO_TEST_CASE(reverse_words_flips)
{
  BOOST_TEST(show(samarin::reverseWords(make("a b c\n"))) == "c b a\n");
}

BOOST_AUTO_TEST_CASE(transpose_ragged)
{
  BOOST_TEST(show(samarin::transpose(make("1 2 3\n4 5\n"))) == "1 4\n2 5\n3\n");
}

BOOST_AUTO_TEST_CASE(edits)
{
  samarin::Text text = make("one two three\nfour five\n");
  samarin::replaceWord(text, 0, 1, "TWO");
  BOOST_TEST(show(text) == "one TWO three\nfour five\n");
  samarin::swapWords(text, 0, 0, 1, 1);
  BOOST_TEST(show(text) == "five TWO three\nfour one\n");
  samarin::removeLine(text, 0);
  BOOST_TEST(show(text) == "four one\n");
}

BOOST_AUTO_TEST_SUITE_END()
