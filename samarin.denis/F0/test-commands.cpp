#include <boost/test/unit_test.hpp>
#include <cstdio>
#include <fstream>
#include <sstream>
#include <string>
#include "commands.hpp"
#include "documents.hpp"

namespace {
  const char* const sample_path = "f0_test_sample.txt";

  void writeFile(const std::string& path, const std::string& content)
  {
    std::ofstream file(path.c_str());
    file << content;
  }

  std::string run(const std::string& script)
  {
    samarin::Documents docs;
    std::istringstream in(script);
    std::ostringstream out;
    samarin::processCommands(in, out, docs);
    return out.str();
  }

  struct Sample {
    Sample()
    {
      writeFile(sample_path, "the cat sat\nthe dog ran\n");
    }

    ~Sample()
    {
      std::remove(sample_path);
    }
  };
}

BOOST_FIXTURE_TEST_SUITE(commands, Sample)

BOOST_AUTO_TEST_CASE(load_and_show)
{
  const std::string out = run("load f0_test_sample.txt d\nshow d\nexit\n");
  BOOST_TEST(out == "the cat sat\nthe dog ran\n");
}

BOOST_AUTO_TEST_CASE(dump_index_sorted)
{
  const std::string out = run("load f0_test_sample.txt d\ndump-index d\nexit\n");
  const std::string expected =
    "cat: (1,2)\n"
    "dog: (2,2)\n"
    "ran: (2,3)\n"
    "sat: (1,3)\n"
    "the: (1,1) (2,1)\n";
  BOOST_TEST(out == expected);
}

BOOST_AUTO_TEST_CASE(find_with_context)
{
  const std::string out = run("load f0_test_sample.txt d\nfind d the --context=1\nexit\n");
  BOOST_TEST(out == "1 1: [the] cat\n2 1: [the] dog\n");
}

BOOST_AUTO_TEST_CASE(find_missing_word)
{
  const std::string out = run("load f0_test_sample.txt d\nfind d zzz\nexit\n");
  BOOST_TEST(out == "<EMPTY>\n");
}

BOOST_AUTO_TEST_CASE(replace_command)
{
  const std::string out = run("load f0_test_sample.txt d\nreplace d 1 2 CAT\nshow d\nexit\n");
  BOOST_TEST(out == "the CAT sat\nthe dog ran\n");
}

BOOST_AUTO_TEST_CASE(concat_command)
{
  const std::string out = run("load f0_test_sample.txt d\nconcat d d big\nshow big\nexit\n");
  BOOST_TEST(out == "the cat sat\nthe dog ran\nthe cat sat\nthe dog ran\n");
}

BOOST_AUTO_TEST_CASE(unknown_command)
{
  BOOST_TEST(run("nonsense\nexit\n") == "<INVALID COMMAND>\n");
}

BOOST_AUTO_TEST_CASE(empty_list)
{
  BOOST_TEST(run("list\nexit\n") == "<EMPTY>\n");
}

BOOST_AUTO_TEST_CASE(find_count)
{
  BOOST_TEST(run("load f0_test_sample.txt d\nfind-count d the\nexit\n") == "2\n");
  BOOST_TEST(run("load f0_test_sample.txt d\nfind-count d missing\nexit\n") == "0\n");
}

BOOST_AUTO_TEST_CASE(find_first_and_last)
{
  BOOST_TEST(run("load f0_test_sample.txt d\nfind-first d the 1\nexit\n") == "1 1: [the]\n");
  BOOST_TEST(run("load f0_test_sample.txt d\nfind-last d the 1\nexit\n") == "2 1: [the]\n");
}

BOOST_AUTO_TEST_CASE(find_around)
{
  BOOST_TEST(run("load f0_test_sample.txt d\nfind-around d cat 1\nexit\n") == "1 2: the [cat] sat\n");
}

BOOST_AUTO_TEST_SUITE_END()
