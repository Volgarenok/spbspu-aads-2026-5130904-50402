#include <utility>

#include <boost/test/unit_test.hpp>

#include "list.hpp"

BOOST_AUTO_TEST_SUITE(list_suite)

BOOST_AUTO_TEST_CASE(default_constructor)
{
  sogdanov::List< int > lst;
  BOOST_TEST(lst.empty());
  BOOST_TEST(lst.size() == 0u);
  BOOST_CHECK(lst.begin() == lst.end());
}

BOOST_AUTO_TEST_CASE(push_front)
{
  sogdanov::List< int > lst;
  lst.push_front(3);
  lst.push_front(2);
  lst.push_front(1);
  BOOST_TEST(lst.size() == 3u);
  BOOST_TEST(!lst.empty());

  auto it = lst.begin();
  BOOST_TEST(*it == 1);
  ++it;
  BOOST_TEST(*it == 2);
  ++it;
  BOOST_TEST(*it == 3);
  ++it;
  BOOST_CHECK(it == lst.end());
}

BOOST_AUTO_TEST_CASE(pop_front)
{
  sogdanov::List< int > lst;
  lst.push_front(2);
  lst.push_front(1);
  lst.pop_front();
  BOOST_TEST(lst.size() == 1u);
  BOOST_TEST(*lst.begin() == 2);

  lst.pop_front();
  BOOST_TEST(lst.empty());
}

BOOST_AUTO_TEST_CASE(pop_front_empty_no_throw)
{
  sogdanov::List< int > lst;
  BOOST_CHECK_NO_THROW(lst.pop_front());
  BOOST_TEST(lst.empty());
}

BOOST_AUTO_TEST_CASE(insert_after)
{
  sogdanov::List< int > lst;
  lst.push_front(1);
  auto it = lst.begin();
  it = lst.insert_after(it, 2);
  it = lst.insert_after(it, 3);

  auto cur = lst.begin();
  BOOST_TEST(*cur == 1);
  ++cur;
  BOOST_TEST(*cur == 2);
  ++cur;
  BOOST_TEST(*cur == 3);
  BOOST_TEST(lst.size() == 3u);
}

BOOST_AUTO_TEST_CASE(insert_after_end_returns_end)
{
  sogdanov::List< int > lst;
  lst.push_front(1);
  auto result = lst.insert_after(lst.end(), 2);
  BOOST_CHECK(result == lst.end());
  BOOST_TEST(lst.size() == 1u);
}

BOOST_AUTO_TEST_CASE(clear)
{
  sogdanov::List< int > lst;
  lst.push_front(1);
  lst.push_front(2);
  lst.clear();
  BOOST_TEST(lst.empty());
  BOOST_TEST(lst.size() == 0u);
  BOOST_CHECK(lst.begin() == lst.end());
}

BOOST_AUTO_TEST_CASE(copy_constructor)
{
  sogdanov::List< int > a;
  a.push_front(3);
  a.push_front(2);
  a.push_front(1);

  sogdanov::List< int > b(a);
  BOOST_TEST(b.size() == 3u);
  b.pop_front();
  BOOST_TEST(a.size() == 3u);
  BOOST_TEST(b.size() == 2u);
}

BOOST_AUTO_TEST_CASE(move_constructor)
{
  sogdanov::List< int > a;
  a.push_front(2);
  a.push_front(1);

  sogdanov::List< int > b(std::move(a));
  BOOST_TEST(b.size() == 2u);
  BOOST_TEST(*b.begin() == 1);
  BOOST_TEST(a.empty());
}

BOOST_AUTO_TEST_CASE(copy_assignment)
{
  sogdanov::List< int > a;
  a.push_front(2);
  a.push_front(1);

  sogdanov::List< int > b;
  b = a;
  BOOST_TEST(b.size() == 2u);
  BOOST_TEST(*b.begin() == 1);
  b.pop_front();
  BOOST_TEST(a.size() == 2u);
}

BOOST_AUTO_TEST_CASE(move_assignment)
{
  sogdanov::List< int > a;
  a.push_front(2);
  a.push_front(1);

  sogdanov::List< int > b;
  b = std::move(a);
  BOOST_TEST(b.size() == 2u);
  BOOST_TEST(*b.begin() == 1);
  BOOST_TEST(a.empty());
}

BOOST_AUTO_TEST_CASE(self_assignment)
{
  sogdanov::List< int > lst;
  lst.push_front(1);
  lst = lst;
  BOOST_TEST(lst.size() == 1u);
  BOOST_TEST(*lst.begin() == 1);
}

BOOST_AUTO_TEST_CASE(const_iteration)
{
  sogdanov::List< int > lst;
  lst.push_front(3);
  lst.push_front(2);
  lst.push_front(1);

  const sogdanov::List< int > &clst = lst;
  auto it = clst.begin();
  BOOST_TEST(*it == 1);
  ++it;
  BOOST_TEST(*it == 2);
  ++it;
  BOOST_TEST(*it == 3);
  ++it;
  BOOST_CHECK(it == clst.end());
}

BOOST_AUTO_TEST_CASE(iter_to_citer)
{
  sogdanov::List< int > lst;
  lst.push_front(42);

  sogdanov::LIter< int > it = lst.begin();
  sogdanov::LCIter< int > cit = it;
  BOOST_TEST(*cit == 42);
}

BOOST_AUTO_TEST_SUITE_END()
