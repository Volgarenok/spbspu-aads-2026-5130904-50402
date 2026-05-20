#include <iterators.hpp>
#include <list.hpp>
#include <boost/test/unit_test.hpp>

using namespace karpovich;

BOOST_AUTO_TEST_SUITE(ListMethodsTests)

BOOST_AUTO_TEST_CASE(test_default_constructor)
{
  List< int > list;
  BOOST_CHECK_EQUAL(list.size(), 0);
  BOOST_CHECK(list.begin() == list.end());
}

BOOST_AUTO_TEST_CASE(test_copy_constructor)
{
  List< int > list1;
  list1.pushBack(1);
  list1.pushBack(2);

  List< int > list2(list1);
  BOOST_CHECK_EQUAL(list2.size(), 2);
  BOOST_CHECK_EQUAL(*list2.begin(), 1);
}

BOOST_AUTO_TEST_CASE(test_move_constructor)
{
  List< int > list1;
  list1.pushBack(1);
  list1.pushBack(2);

  List< int > list2(std::move(list1));
  BOOST_CHECK_EQUAL(list2.size(), 2);
  BOOST_CHECK_EQUAL(list1.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_copy_assignment)
{
  List< int > list1;
  list1.pushBack(1);

  List< int > list2;
  list2 = list1;

  BOOST_CHECK_EQUAL(list2.size(), 1);
  BOOST_CHECK_EQUAL(*list2.begin(), 1);
}

BOOST_AUTO_TEST_CASE(test_move_assignment)
{
  List< int > list1;
  list1.pushBack(1);

  List< int > list2;
  list2 = std::move(list1);

  BOOST_CHECK_EQUAL(list2.size(), 1);
  BOOST_CHECK_EQUAL(list1.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_destructor)
{
  {
    List< int > list;
    list.pushBack(1);
    list.pushBack(2);
  }
  BOOST_CHECK(true);
}

BOOST_AUTO_TEST_CASE(test_begin)
{
  List< int > list;
  list.pushBack(5);

  BOOST_CHECK_EQUAL(*list.begin(), 5);
}

BOOST_AUTO_TEST_CASE(test_end)
{
  List< int > list;
  list.pushBack(1);

  LIter< int > it = list.begin();
  ++it;
  BOOST_CHECK(it == list.end());
}

BOOST_AUTO_TEST_CASE(test_begin_const)
{
  const List< int > list;
  BOOST_CHECK(list.cbegin() == list.cend());
}

BOOST_AUTO_TEST_CASE(test_end_const)
{
  const List< int > list;
  BOOST_CHECK(list.cbegin() == list.cend());
}

BOOST_AUTO_TEST_CASE(test_push_front)
{
  List< int > list;
  list.pushFront(2);
  list.pushFront(1);

  BOOST_CHECK_EQUAL(*list.begin(), 1);
  BOOST_CHECK_EQUAL(list.size(), 2);
}

BOOST_AUTO_TEST_CASE(test_push_back)
{
  List< int > list;
  list.pushBack(1);
  list.pushBack(2);

  LIter< int > it = list.begin();
  BOOST_CHECK_EQUAL(*it, 1);
  it++;
  BOOST_CHECK_EQUAL(*it, 2);
}

BOOST_AUTO_TEST_CASE(test_pop_front)
{
  List< int > list;
  list.pushBack(1);
  list.pushBack(2);

  list.popFront();
  BOOST_CHECK_EQUAL(list.size(), 1);
  BOOST_CHECK_EQUAL(*list.begin(), 2);
}

BOOST_AUTO_TEST_CASE(test_pop_back)
{
  List< int > list;
  list.pushBack(1);
  list.pushBack(2);

  list.popBack();
  BOOST_CHECK_EQUAL(list.size(), 1);
  BOOST_CHECK_EQUAL(*list.begin(), 1);
}

BOOST_AUTO_TEST_CASE(test_clear)
{
  List< int > list;
  list.pushBack(1);
  list.pushBack(2);

  list.clear();
  BOOST_CHECK_EQUAL(list.size(), 0);
  BOOST_CHECK(list.begin() == list.end());
}

BOOST_AUTO_TEST_CASE(test_size)
{
  List< int > list;
  BOOST_CHECK_EQUAL(list.size(), 0);

  list.pushBack(1);
  BOOST_CHECK_EQUAL(list.size(), 1);

  list.pushBack(2);
  BOOST_CHECK_EQUAL(list.size(), 2);
}

BOOST_AUTO_TEST_CASE(test_empty)
{
  List< int > list;
  BOOST_CHECK_EQUAL(list.empty(), true);
}

BOOST_AUTO_TEST_CASE(test_insert)
{
  List< int > list;
  list.pushBack(1);
  list.pushBack(2);
  list.pushBack(3);
  LIter< int > it = list.begin();
  it++;
  it = list.insert(it, 4);
  BOOST_CHECK_EQUAL(*it, 4);
}

BOOST_AUTO_TEST_CASE(test_front)
{
  List< int > list;
  list.pushBack(1);
  list.pushBack(2);
  BOOST_CHECK_EQUAL(list.front(), 1);
  list.front() = 10;
  BOOST_CHECK_EQUAL(*list.begin(), 10);
}

BOOST_AUTO_TEST_CASE(test_back)
{
  List< int > list;
  list.pushBack(1);
  list.pushBack(2);
  BOOST_CHECK_EQUAL(list.back(), 2);
  list.back() = 20;
  LIter< int > it = list.end();
  it--;
  BOOST_CHECK_EQUAL(*it, 20);
}

BOOST_AUTO_TEST_CASE(test_front_back_const)
{
  List< int > list;
  list.pushBack(5);
  const List< int > &cref = list;
  BOOST_CHECK_EQUAL(cref.front(), 5);
  BOOST_CHECK_EQUAL(cref.back(), 5);
}

BOOST_AUTO_TEST_CASE(test_erase)
{
  List< int > list;
  list.pushBack(1);
  list.pushBack(2);
  list.pushBack(3);
  LIter< int > it = list.begin();
  ++it;
  LIter< int > next = list.erase(it);
  BOOST_CHECK_EQUAL(*next, 3);
  BOOST_CHECK_EQUAL(list.size(), 2);
  BOOST_CHECK_EQUAL(list.front(), 1);
  BOOST_CHECK_EQUAL(list.back(), 3);
}

BOOST_AUTO_TEST_CASE(test_splice_all)
{
  List< int > list1;
  list1.pushBack(1);
  list1.pushBack(2);

  List< int > list2;
  list2.pushBack(10);
  list2.pushBack(20);

  list1.splice(list1.end(), list2);
  BOOST_CHECK_EQUAL(list1.size(), 4);
  BOOST_CHECK_EQUAL(list2.size(), 0);
  BOOST_CHECK(list2.empty());

  LIter< int > it = list1.begin();
  BOOST_CHECK_EQUAL(*it++, 1);
  BOOST_CHECK_EQUAL(*it++, 2);
  BOOST_CHECK_EQUAL(*it++, 10);
  BOOST_CHECK_EQUAL(*it, 20);
}

BOOST_AUTO_TEST_CASE(test_splice_single)
{
  List< int > list1;
  list1.pushBack(1);
  list1.pushBack(3);

  List< int > list2;
  list2.pushBack(10);
  list2.pushBack(20);

  LIter< int > it2 = list2.begin();
  ++it2;
  list1.splice(list1.end(), list2, it2);

  BOOST_CHECK_EQUAL(list1.size(), 3);
  BOOST_CHECK_EQUAL(list2.size(), 1);
  BOOST_CHECK_EQUAL(list2.front(), 10);

  LIter< int > it1 = list1.begin();
  BOOST_CHECK_EQUAL(*it1++, 1);
  BOOST_CHECK_EQUAL(*it1++, 3);
  BOOST_CHECK_EQUAL(*it1, 20);
}

BOOST_AUTO_TEST_CASE(test_splice_range)
{
  List< int > list1;
  list1.pushBack(1);
  list1.pushBack(2);

  List< int > list2;
  list2.pushBack(10);
  list2.pushBack(20);
  list2.pushBack(30);
  list2.pushBack(40);

  LIter< int > first = list2.begin();
  ++first;
  LIter< int > last = list2.end();
  --last;

  list1.splice(list1.begin(), list2, first, last);
  BOOST_CHECK_EQUAL(list1.size(), 4);
  BOOST_CHECK_EQUAL(list2.size(), 2);

  LIter< int > it = list1.begin();
  BOOST_CHECK_EQUAL(*it++, 20);
  BOOST_CHECK_EQUAL(*it++, 30);
  BOOST_CHECK_EQUAL(*it++, 1);
  BOOST_CHECK_EQUAL(*it, 2);
}

BOOST_AUTO_TEST_CASE(test_splice_empty_range)
{
  List< int > list1;
  list1.pushBack(1);
  List< int > list2;
  list2.pushBack(10);

  list1.splice(list1.begin(), list2, list2.end(), list2.end());
  BOOST_CHECK_EQUAL(list1.size(), 1);
  BOOST_CHECK_EQUAL(list2.size(), 1);
}

BOOST_AUTO_TEST_CASE(test_sort_default)
{
  List< int > list;
  list.pushBack(5);
  list.pushBack(2);
  list.pushBack(8);
  list.pushBack(1);

  list.sort();
  BOOST_CHECK_EQUAL(list.size(), 4);

  LIter< int > it = list.begin();
  BOOST_CHECK_EQUAL(*it++, 1);
  BOOST_CHECK_EQUAL(*it++, 2);
  BOOST_CHECK_EQUAL(*it++, 5);
  BOOST_CHECK_EQUAL(*it, 8);
}

BOOST_AUTO_TEST_CASE(test_sort_custom)
{
  List< int > list;
  list.pushBack(5);
  list.pushBack(2);
  list.pushBack(8);
  list.pushBack(1);

  list.sort(std::greater< int >{});
  BOOST_CHECK_EQUAL(list.size(), 4);

  LIter< int > it = list.begin();
  BOOST_CHECK_EQUAL(*it++, 8);
  BOOST_CHECK_EQUAL(*it++, 5);
  BOOST_CHECK_EQUAL(*it++, 2);
  BOOST_CHECK_EQUAL(*it, 1);
}

BOOST_AUTO_TEST_CASE(test_sort_empty_and_single)
{
  List< int > empty;
  empty.sort();
  BOOST_CHECK(empty.empty());

  List< int > single;
  single.pushBack(42);
  single.sort();
  BOOST_CHECK_EQUAL(single.size(), 1);
  BOOST_CHECK_EQUAL(single.front(), 42);
}

BOOST_AUTO_TEST_CASE(test_merge_default)
{
  List< int > list1;
  list1.pushBack(1);
  list1.pushBack(3);
  list1.pushBack(5);

  List< int > list2;
  list2.pushBack(2);
  list2.pushBack(4);
  list2.pushBack(6);

  list1.merge(list2);
  BOOST_CHECK_EQUAL(list1.size(), 6);
  BOOST_CHECK_EQUAL(list2.size(), 0);

  LIter< int > it = list1.begin();
  for (int i = 1; i <= 6; ++i) {
    BOOST_CHECK_EQUAL(*it++, i);
  }
}

BOOST_AUTO_TEST_CASE(test_merge_custom)
{
  List< int > list1;
  list1.pushBack(5);
  list1.pushBack(3);
  list1.pushBack(1);

  List< int > list2;
  list2.pushBack(6);
  list2.pushBack(4);
  list2.pushBack(2);

  list1.merge(list2, std::greater< int >{});
  BOOST_CHECK_EQUAL(list1.size(), 6);
  BOOST_CHECK_EQUAL(list2.size(), 0);

  LIter< int > it = list1.begin();
  BOOST_CHECK_EQUAL(*it++, 6);
  BOOST_CHECK_EQUAL(*it++, 5);
  BOOST_CHECK_EQUAL(*it++, 4);
  BOOST_CHECK_EQUAL(*it++, 3);
  BOOST_CHECK_EQUAL(*it++, 2);
  BOOST_CHECK_EQUAL(*it, 1);
}

BOOST_AUTO_TEST_CASE(test_merge_with_empty)
{
  List< int > list1;
  list1.pushBack(10);
  List< int > list2;

  list1.merge(list2);
  BOOST_CHECK_EQUAL(list1.size(), 1);
  BOOST_CHECK_EQUAL(list2.size(), 0);
  BOOST_CHECK_EQUAL(list1.front(), 10);
}

BOOST_AUTO_TEST_CASE(test_partition_basic)
{
  List< int > list;
  list.pushBack(1);
  list.pushBack(2);
  list.pushBack(3);
  list.pushBack(4);
  list.pushBack(5);

  LIter< int > split = list.partition([](int v) {
    return v % 2 == 0;
  });

  BOOST_CHECK_EQUAL(*split, 1);
  LIter< int > it = list.begin();
  BOOST_CHECK_EQUAL(*it++, 2);
  BOOST_CHECK_EQUAL(*it++, 4);
  BOOST_CHECK_EQUAL(*it++, 1);
  BOOST_CHECK_EQUAL(*it++, 3);
  BOOST_CHECK_EQUAL(*it, 5);
}

BOOST_AUTO_TEST_CASE(test_partition_all_true)
{
  List< int > list;
  list.pushBack(2);
  list.pushBack(4);

  LIter< int > split = list.partition([](int v) {
    return v % 2 == 0;
  });
  BOOST_CHECK(split == list.end());
}

BOOST_AUTO_TEST_CASE(test_partition_all_false)
{
  List< int > list;
  list.pushBack(1);
  list.pushBack(3);

  LIter< int > split = list.partition([](int v) {
    return v % 2 == 0;
  });
  BOOST_CHECK(split == list.begin());
}

BOOST_AUTO_TEST_CASE(test_partition_empty)
{
  List< int > list;
  LIter< int > split = list.partition([](int) {
    return true;
  });
  BOOST_CHECK(split == list.end());
  BOOST_CHECK(list.empty());
}

BOOST_AUTO_TEST_SUITE_END()
