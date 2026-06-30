#include <boost/test/unit_test.hpp>
#include <sstream>
#include <string>
#include "commands.hpp"
#include "dataset.hpp"
#include "input.hpp"

namespace {
  samarin::DatasetCollection load(const std::string & text)
  {
    std::istringstream in(text);
    samarin::DatasetCollection datasets;
    samarin::readDatasets(in, datasets);
    return datasets;
  }

  std::string run(samarin::DatasetCollection & datasets, const std::string & commands)
  {
    std::istringstream in(commands);
    std::ostringstream out;
    samarin::processCommands(in, out, datasets);
    return out.str();
  }
}

BOOST_AUTO_TEST_SUITE(command_tests)

BOOST_AUTO_TEST_CASE(print_sorts_by_key)
{
  samarin::DatasetCollection datasets = load("second 4 mouse 1 name 2 keyboard\n");
  BOOST_TEST(run(datasets, "print second\n") == "second 1 name 2 keyboard 4 mouse\n");
}

BOOST_AUTO_TEST_CASE(print_empty_reports_marker)
{
  samarin::DatasetCollection datasets = load("empty\n");
  BOOST_TEST(run(datasets, "print empty\n") == "<EMPTY>\n");
}

BOOST_AUTO_TEST_CASE(unknown_command_is_invalid)
{
  samarin::DatasetCollection datasets = load("a 1 x\n");
  BOOST_TEST(run(datasets, "frobnicate a\n") == "<INVALID COMMAND>\n");
}

BOOST_AUTO_TEST_CASE(missing_dataset_is_invalid)
{
  samarin::DatasetCollection datasets = load("a 1 x\n");
  BOOST_TEST(run(datasets, "print b\n") == "<INVALID COMMAND>\n");
}

BOOST_AUTO_TEST_CASE(reader_skips_blank_lines)
{
  samarin::DatasetCollection datasets = load("\nfirst 1 a\n\nsecond 2 b\n\n");
  const std::string out = run(datasets, "print first\nprint second\n");
  BOOST_TEST(out == "first 1 a\nsecond 2 b\n");
}

BOOST_AUTO_TEST_CASE(set_operations_match_spec)
{
  samarin::DatasetCollection datasets =
      load("first 1 name 2 surname\nsecond 4 mouse 1 name 2 keyboard\n");
  const std::string out = run(datasets,
      "complement third second first\nprint third\n"
      "intersect fourth first second\nprint fourth\n"
      "intersect yafourth second first\nprint yafourth\n"
      "union fifth first second\nprint fifth\n"
      "union yafifth second first\nprint yafifth\n");
  const std::string expected =
      "third 4 mouse\n"
      "fourth 1 name 2 surname\n"
      "yafourth 1 name 2 keyboard\n"
      "fifth 1 name 2 surname 4 mouse\n"
      "yafifth 1 name 2 keyboard 4 mouse\n";
  BOOST_TEST(out == expected);
}

BOOST_AUTO_TEST_SUITE_END()
