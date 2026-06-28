#include <boost/test/unit_test.hpp>
#include <string>
#include "cuckoo_table.hpp"
#include "hashers.hpp"

BOOST_AUTO_TEST_SUITE(hashers)

BOOST_AUTO_TEST_CASE(string_hash_is_deterministic)
{
  const samarin::StringHash hash;
  BOOST_TEST(hash("hello") == hash("hello"));
}

BOOST_AUTO_TEST_CASE(string_hash_separates_words)
{
  const samarin::StringHash hash;
  BOOST_TEST(hash("hello") != hash("world"));
  BOOST_TEST(hash("abc") != hash("abd"));
}

BOOST_AUTO_TEST_CASE(mix_spreads_input)
{
  BOOST_TEST(samarin::detail::cuckooMix(1) != 1u);
  BOOST_TEST(samarin::detail::cuckooMix(1) != samarin::detail::cuckooMix(2));
}

BOOST_AUTO_TEST_SUITE_END()
