#include <boost/test/unit_test.hpp>
#include <stdexcept>
#include "Word.hpp"
using namespace lachugin;

BOOST_AUTO_TEST_CASE(constructor_test)
{
  Word word("арбуз");
  BOOST_TEST(word.getTranslations().size() == 1);
}

BOOST_AUTO_TEST_CASE(add_translation_test)
{
  Word word("арбуз");
  word.addTranslation("арбузик");
  BOOST_TEST(word.getTranslations().size() == 2);
}

BOOST_AUTO_TEST_CASE(has_translation_test)
{
  Word word("арбуз");
  word.addTranslation("арбузик");
  BOOST_TEST(word.hasTranslation("арбуз"));
  BOOST_TEST(word.hasTranslation("арбузик"));
  BOOST_TEST(!word.hasTranslation("яблоко"));
}

BOOST_AUTO_TEST_CASE(interpretation_test)
{
  Word word("арбуз");
  word.setInterpretation("ягода");
  BOOST_TEST(word.hasInterpretation());
  BOOST_TEST(word.getInterpretation() == "ягода");
}

BOOST_AUTO_TEST_CASE(remove_interpretation_test)
{
  Word word("арбуз");
  word.setInterpretation("ягода");
  word.removeInterpretation();
  BOOST_TEST(!word.hasInterpretation());
}

BOOST_AUTO_TEST_CASE(part_of_speech_test)
{
  Word word("арбуз");
  word.setPartOfSpeech("noun");
  BOOST_TEST(word.getPartOfSpeech() == "noun");
}



