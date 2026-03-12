#define BOOST_TEST_MODULE ListTest
#include <boost/test/included/unit_test.hpp>
#include <sstream>
#include "List.hpp"

BOOST_AUTO_TEST_CASE(clear_test)
{
  lavrentev::List<int> k{};
  lavrentev::LIter<int> it;
  it = k.insert(it, 3);
  it = k.insert(it, 4);
  k.clear();
  BOOST_CHECK(k.cbegin() == lavrentev::LCIter<int>());
}

BOOST_AUTO_TEST_CASE(insert_test)
{
  /*lavrentev::List<int> k{};
  lavrentev::LIter<int> it;
  it = k.insert(it, 3);
  it = k.insert(it, 4);
  k.clear();
  BOOST_TEST(k.cbegin() == nullptr);*/
}

/*BOOST_AUTO_TEST_CASE(print_test)
{
  std::ostringstream out;
  lavrentev::List<int> k{};
  lavrentev::LIter<int> curr;
  curr = k.insert(curr, 3);
  curr = k.insert(curr, 4);
  lavrentev::LCIter<int> it;
  //it.curr = k; метод для добавления головы в конст итератор
  it.printList(out);
  BOOST_TEST(out.str() == "3 4");
}*/

