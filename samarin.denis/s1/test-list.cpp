#include <boost/test/unit_test.hpp>
#include "list.hpp"

using samarin::List;
using samarin::LIter;
using samarin::LCIter;

BOOST_AUTO_TEST_CASE(test_empty_list)
{
  List< int > lst;
  BOOST_TEST(lst.empty());
  BOOST_TEST((lst.begin() == lst.end()));
}

BOOST_AUTO_TEST_CASE(test_push_front)
{
  List< int > lst;
  lst.push_front(3);
  lst.push_front(2);
  lst.push_front(1);
  BOOST_TEST(!lst.empty());
  BOOST_TEST(lst.front() == 1);

  auto it = lst.begin();
  BOOST_TEST(*it == 1);
  ++it;
  BOOST_TEST(*it == 2);
  ++it;
  BOOST_TEST(*it == 3);
  ++it;
  BOOST_TEST((it == lst.end()));
}

BOOST_AUTO_TEST_CASE(test_pop_front)
{
  List< int > lst;
  lst.push_front(2);
  lst.push_front(1);
  BOOST_TEST(lst.front() == 1);
  lst.pop_front();
  BOOST_TEST(lst.front() == 2);
  lst.pop_front();
  BOOST_TEST(lst.empty());
}

BOOST_AUTO_TEST_CASE(test_insert_after)
{
  List< int > lst;
  auto tail = lst.before_begin();
  tail = lst.insert_after(tail, 1);
  tail = lst.insert_after(tail, 2);
  tail = lst.insert_after(tail, 3);

  auto it = lst.begin();
  BOOST_TEST(*it == 1);
  ++it;
  BOOST_TEST(*it == 2);
  ++it;
  BOOST_TEST(*it == 3);
}

BOOST_AUTO_TEST_CASE(test_insert_after_middle)
{
  List< int > lst;
  lst.push_front(3);
  lst.push_front(1);
  auto it = lst.begin();
  lst.insert_after(it, 2);

  it = lst.begin();
  BOOST_TEST(*it == 1);
  ++it;
  BOOST_TEST(*it == 2);
  ++it;
  BOOST_TEST(*it == 3);
}

BOOST_AUTO_TEST_CASE(test_clear)
{
  List< int > lst;
  lst.push_front(3);
  lst.push_front(2);
  lst.push_front(1);
  lst.clear();
  BOOST_TEST(lst.empty());
  BOOST_TEST((lst.begin() == lst.end()));
}

BOOST_AUTO_TEST_CASE(test_copy_constructor)
{
  List< int > lst;
  lst.push_front(3);
  lst.push_front(2);
  lst.push_front(1);

  List< int > copy(lst);
  BOOST_TEST(copy.front() == 1);

  auto it1 = lst.begin();
  auto it2 = copy.begin();
  while (it1 != lst.end()) {
    BOOST_TEST(*it1 == *it2);
    ++it1;
    ++it2;
  }
  BOOST_TEST((it2 == copy.end()));

  copy.pop_front();
  BOOST_TEST(lst.front() == 1);
  BOOST_TEST(copy.front() == 2);
}

BOOST_AUTO_TEST_CASE(test_copy_assignment)
{
  List< int > lst;
  lst.push_front(2);
  lst.push_front(1);

  List< int > other;
  other.push_front(99);
  other = lst;
  BOOST_TEST(other.front() == 1);

  other.pop_front();
  BOOST_TEST(lst.front() == 1);
  BOOST_TEST(other.front() == 2);
}

BOOST_AUTO_TEST_CASE(test_self_assignment)
{
  List< int > lst;
  lst.push_front(1);
  lst = lst;
  BOOST_TEST(lst.front() == 1);
}

BOOST_AUTO_TEST_CASE(test_move_constructor)
{
  List< int > lst;
  lst.push_front(2);
  lst.push_front(1);

  List< int > moved(std::move(lst));
  BOOST_TEST(moved.front() == 1);
  BOOST_TEST(!moved.empty());
}

BOOST_AUTO_TEST_CASE(test_move_assignment)
{
  List< int > lst;
  lst.push_front(2);
  lst.push_front(1);

  List< int > other;
  other.push_front(99);
  other = std::move(lst);
  BOOST_TEST(other.front() == 1);
}

BOOST_AUTO_TEST_CASE(test_const_iterator)
{
  List< int > lst;
  lst.push_front(2);
  lst.push_front(1);

  const List< int >& clst = lst;
  auto it = clst.begin();
  BOOST_TEST(*it == 1);
  ++it;
  BOOST_TEST(*it == 2);
  ++it;
  BOOST_TEST((it == clst.end()));
}

BOOST_AUTO_TEST_CASE(test_cbegin_cend)
{
  List< int > lst;
  lst.push_front(1);

  auto it = lst.cbegin();
  BOOST_TEST(*it == 1);
  ++it;
  BOOST_TEST((it == lst.cend()));
}

BOOST_AUTO_TEST_CASE(test_const_front)
{
  List< int > lst;
  lst.push_front(42);
  const List< int >& clst = lst;
  BOOST_TEST(clst.front() == 42);
}

BOOST_AUTO_TEST_CASE(test_postfix_increment)
{
  List< int > lst;
  lst.push_front(2);
  lst.push_front(1);

  auto it = lst.begin();
  auto prev = it++;
  BOOST_TEST(*prev == 1);
  BOOST_TEST(*it == 2);
}

BOOST_AUTO_TEST_CASE(test_iterator_arrow)
{
  List< std::pair< int, int > > lst;
  lst.push_front(std::make_pair(1, 2));
  auto it = lst.begin();
  BOOST_TEST(it->first == 1);
  BOOST_TEST(it->second == 2);
}

BOOST_AUTO_TEST_CASE(test_clear_then_reuse)
{
  List< int > lst;
  lst.push_front(1);
  lst.clear();
  lst.push_front(2);
  BOOST_TEST(lst.front() == 2);
  BOOST_TEST(!lst.empty());
}

BOOST_AUTO_TEST_CASE(test_copy_empty_list)
{
  List< int > lst;
  List< int > copy(lst);
  BOOST_TEST(copy.empty());
}

BOOST_AUTO_TEST_CASE(test_move_empty_list)
{
  List< int > lst;
  List< int > moved(std::move(lst));
  BOOST_TEST(moved.empty());
}