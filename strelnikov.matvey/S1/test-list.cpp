#include "boost/test/unit_test.hpp"
#include "../common/list.hpp"
#include "funcs.hpp"

using namespace strelnikov;

BOOST_AUTO_TEST_SUITE(List_Tests)

BOOST_AUTO_TEST_CASE(Def_Constructort)
{
  List< int > list;

  BOOST_CHECK(list.empty());
  BOOST_CHECK_EQUAL(size(list), 0);
  BOOST_CHECK(list.cbegin() == list.cend());
}

BOOST_AUTO_TEST_CASE(Copy_Constructor)
{
  List< int > original;
  original.push_front(123);
  original.push_front(333);

  List< int > copy(original);

  BOOST_CHECK(!copy.empty());
  BOOST_CHECK_EQUAL(size(copy), 2);

  auto it = copy.begin();
  BOOST_CHECK(*it == 333);
  ++it;
  BOOST_CHECK(*it == 123);
  ++it;
  BOOST_CHECK(it == copy.end());

  original.pop_front();
  BOOST_CHECK_EQUAL(size(original), 1);
  BOOST_CHECK_EQUAL(size(copy), 2);
}

BOOST_AUTO_TEST_CASE(Move_Constructor_RVal)
{
  List< int > source;
  source.push_front(123);
  source.push_front(333);

  List< int > dest(std::move(source));

  BOOST_CHECK(dest.empty() == false);
  BOOST_CHECK_EQUAL(size(dest), 2);

  auto it = dest.begin();
  BOOST_CHECK(*it == 333);
  ++it;
  BOOST_CHECK(*it == 123);
  ++it;
  BOOST_CHECK(it == dest.end());

  BOOST_CHECK(source.empty());
}

BOOST_AUTO_TEST_CASE(Copy_Constructor_Str)
{
  List< std::string > original;
  original.push_front("World");
  original.push_front("Hello");

  List< std::string > copy(original);

  BOOST_CHECK_EQUAL(size(copy), 2);
  BOOST_CHECK(*(copy.begin()) == "Hello");
  BOOST_CHECK(*(++copy.begin()) == "World");
}

BOOST_AUTO_TEST_CASE(Copy_Assignment)
{
  List< int > list1;
  list1.push_front(123);

  List< int > list2;
  list2.push_front(333);
  list2 = list1;

  BOOST_CHECK_EQUAL(size(list2), 1);
  BOOST_CHECK_EQUAL(*list2.begin(), 123);
}

BOOST_AUTO_TEST_CASE(Move_Assignment)
{
  List< int > list1;
  list1.push_front(123);

  List< int > list2;
  list2.push_front(333);
  list2 = std::move(list1);

  BOOST_CHECK_EQUAL(size(list2), 1);
  BOOST_CHECK_EQUAL(*list2.begin(), 123);
  BOOST_CHECK(list1.empty());
}

BOOST_AUTO_TEST_CASE(Destructor_Cleans_List)
{
  {
    List< int > list;
    list.push_front(123);

    BOOST_CHECK_EQUAL(size(list), 1);
  }

  BOOST_CHECK(true);
}

BOOST_AUTO_TEST_CASE(Push_Front)
{
  List< int > list;
  list.push_front(123);

  BOOST_CHECK_EQUAL(size(list), 1);
  BOOST_CHECK_EQUAL(*list.begin(), 123);
}

BOOST_AUTO_TEST_CASE(Pop_Front)
{
  List< int > list;
  list.push_front(123);
  list.push_front(333);

  list.pop_front();

  BOOST_CHECK_EQUAL(size(list), 1);
  BOOST_CHECK_EQUAL(*list.begin(), 123);
  BOOST_CHECK(list.cbegin() != list.cend());
}

BOOST_AUTO_TEST_CASE(Clear_List)
{
  List< int > list;
  for (int i = 0; i < 5; ++i) {
    list.push_front(123);
  }

  BOOST_CHECK_EQUAL(size(list), 5);

  list.clear();

  BOOST_CHECK(list.empty());
  BOOST_CHECK_EQUAL(size(list), 0);
  BOOST_CHECK(list.cbegin() == list.cend());
}

BOOST_AUTO_TEST_CASE(Insert_After)
{
  List< int > list;
  list.push_front(123);
  list.push_front(333);

  auto it = list.begin();

  it = list.insert_after(it, 444);

  BOOST_CHECK_EQUAL(size(list), 3);

  auto check_it = list.begin();
  BOOST_CHECK(*check_it == 333);
  ++check_it;
  BOOST_CHECK(*check_it == 444);
  ++check_it;
  BOOST_CHECK(*check_it == 123);
}

BOOST_AUTO_TEST_CASE(Erase_After)
{
  List< int > list;
  list.push_front(123);
  list.push_front(333);
  list.push_front(444);

  auto it = list.begin();

  it = list.erase_after(it);

  BOOST_CHECK_EQUAL(size(list), 2);

  auto check_it = list.begin();
  BOOST_CHECK(*check_it == 444);
  ++check_it;
  BOOST_CHECK(*check_it == 123);
  ++check_it;
  BOOST_CHECK(check_it == list.end());
}

BOOST_AUTO_TEST_CASE(Empty_Check)
{
  List< int > list;
  BOOST_CHECK(list.empty());

  list.push_front(123);
  BOOST_CHECK(!list.empty());
}

BOOST_AUTO_TEST_CASE(Begin_End)
{
  List< int > list;

  list.push_front(123);

  auto it = list.begin();

  BOOST_CHECK(*it == 123);
  ++it;

  BOOST_CHECK(it == list.end());
}

BOOST_AUTO_TEST_CASE(CBegin_CEnd)
{
  List< int > list;
  list.push_front(123);

  auto it = list.cbegin();
  auto end_it = list.cend();

  BOOST_CHECK(it != end_it);
  BOOST_CHECK(*it == 123);
  ++it;
  BOOST_CHECK(it == end_it);
}

BOOST_AUTO_TEST_CASE(Modification_Mutable)
{
  List< int > list;
  list.push_front(123);

  auto it = list.begin();
  *it = 999;

  BOOST_CHECK(*list.begin() == 999);
}

BOOST_AUTO_TEST_CASE(Iterator_Constructor)
{
  List< int > list;
  list.push_front(123);

  auto it = list.begin();

  BOOST_CHECK(*it == 123);
}

BOOST_AUTO_TEST_CASE(Pre_Increment)
{
  List< int > list;
  list.push_front(333);
  list.push_front(444);

  auto it = list.begin();
  BOOST_CHECK(*it == 444);

  ++it;
  BOOST_CHECK(*it == 333);

  ++it;
  BOOST_CHECK(it == list.end());
}

BOOST_AUTO_TEST_CASE(Post_Increment)
{
  List< int > list;
  list.push_front(333);
  list.push_front(444);

  auto it = list.begin();
  auto next_it = it++;

  BOOST_CHECK(*next_it == 444);
  BOOST_CHECK(*it == 333);
}

BOOST_AUTO_TEST_CASE(Equality_Operator)
{
  List< int > empty_list;
  BOOST_CHECK(empty_list.cbegin() == empty_list.cend());

  List< int > single_list;
  single_list.push_front(999);

  auto it_start = single_list.begin();
  auto it_end = single_list.end();

  BOOST_CHECK(it_start != it_end);

  auto it_copy = single_list.begin();
  BOOST_CHECK(it_start == it_copy);

  ++it_start;
  BOOST_CHECK(it_start == it_end);
}

BOOST_AUTO_TEST_CASE(Const_Iterator)
{
  List< int > list;
  list.push_front(777);

  auto it = list.cbegin();
  BOOST_CHECK(*it == 777);

  ++it;
  BOOST_CHECK(it == list.cend());
}

BOOST_AUTO_TEST_CASE(Size_Empty)
{
  List< int > list;
  BOOST_CHECK_EQUAL(size(list), 0);
}

BOOST_AUTO_TEST_CASE(Size_NotEmpty)
{
  List< int > list;
  list.push_front(333);
  list.push_front(444);

  BOOST_CHECK_EQUAL(size(list), 2);

  list.pop_front();
  BOOST_CHECK_EQUAL(size(list), 1);
}

BOOST_AUTO_TEST_CASE(Swap_Empty)
{
  List< int > list1;
  List< int > list2;

  list1.swap(list2);

  BOOST_CHECK(list1.empty());
  BOOST_CHECK(list2.empty());
}

BOOST_AUTO_TEST_CASE(Swap_Hard)
{
  List< int > list1;
  list1.push_front(123);
  list1.push_front(333);

  List< int > list2;
  list2.push_front(444);

  list1.swap(list2);

  BOOST_CHECK_EQUAL(size(list1), 1);
  BOOST_CHECK_EQUAL(*list1.begin(), 444);

  BOOST_CHECK_EQUAL(size(list2), 2);
  BOOST_CHECK_EQUAL(*list2.begin(), 333);
}

BOOST_AUTO_TEST_SUITE_END()
