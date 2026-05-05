#include <boost/test/unit_test.hpp>
#include "List_collection.hpp"

namespace chernikov {

  BOOST_AUTO_TEST_SUITE(CONSTRUCTORS)

  BOOST_AUTO_TEST_CASE(default_constructor)
  {
    List< int > list;
    BOOST_TEST(list.empty());
    BOOST_TEST(list.size() == 0);
    BOOST_TEST(list.begin() == list.end());
  }
  BOOST_AUTO_TEST_CASE(copy_costructor)
  {
    List< int > original;
    original.add(42);
    original.add(13);
    List< int > copy(original);
    auto orig_it = original.begin();
    auto cp_it = copy.begin();
    BOOST_TEST(*cp_it == *orig_it);
    ++cp_it;
    ++orig_it;
    BOOST_TEST(*cp_it == *orig_it);
  }
  BOOST_AUTO_TEST_CASE(move_constructor_transfers_ownership)
  {
    List< int > original;
    original.add(42);
    List< int > moved(std::move(original));
    BOOST_TEST(moved.size() == 1);
    BOOST_TEST(*moved.begin() == 42);
    BOOST_TEST(original.empty());
  }
  BOOST_AUTO_TEST_SUITE_END()

  BOOST_AUTO_TEST_SUITE(LIST_ACTIONS)

  BOOST_AUTO_TEST_CASE(add_adds_to_front)
  {
    List< int > list;
    list.add(1);
    list.add(2);
    list.add(3);
    BOOST_TEST(list.size() == 3);
    auto it = list.begin();
    BOOST_TEST(*it == 3);
    ++it;
    BOOST_TEST(*it == 2);
    ++it;
    BOOST_TEST(*it == 1);
  }
  BOOST_AUTO_TEST_CASE(front_returns_first_element)
  {
    List< int > list;
    list.add(42);
    list.add(13);
    BOOST_TEST(list.front() == 13);
  }

  BOOST_AUTO_TEST_CASE(front_can_modify_element)
  {
    List< int > list;
    list.add(42);
    list.front() = 100;
    BOOST_TEST(*list.begin() == 100);
  }
  BOOST_AUTO_TEST_CASE(insert_after)
  {
    List< int > list;
    list.add(2);
    auto it = list.begin();
    auto it_inserted = list.insert_after(it, 3);
    BOOST_TEST(*it_inserted == 3);
  }
  BOOST_AUTO_TEST_CASE(first_delete)
  {
    List< int > list;
    list.add(3);
    list.add(2);
    list.add(1);
    list.first_delete();
    BOOST_TEST(list.size() == 2);
    auto it = list.begin();
    BOOST_TEST(*it == 2);
    ++it;
    BOOST_TEST(*it == 3);
  }
  BOOST_AUTO_TEST_CASE(clear_removes_all_elements)
  {
    List< int > list;
    list.add(1);
    list.add(2);
    list.add(3);
    BOOST_TEST(list.size() == 3);
    list.clear();
    BOOST_TEST(list.empty());
    BOOST_TEST(list.size() == 0);
    BOOST_TEST(list.begin() == list.end());
  }
  BOOST_AUTO_TEST_SUITE_END()

  BOOST_AUTO_TEST_SUITE(ITERATORS)

  BOOST_AUTO_TEST_CASE(const_iterator_from_const_list)
  {
    List< int > list;
    list.add(42); // было 100, тест прошел успешно
    const List< int > &const_list = list;
    auto it = const_list.begin();
    BOOST_TEST(*it == 42);
    // *it = 100;  // Ошибка компиляции
  }
  BOOST_AUTO_TEST_CASE(iterator_equality)
  {
    List< int > list;
    list.add(1);
    list.add(2);
    auto it1 = list.begin();
    auto it2 = list.begin();
    BOOST_TEST(it1 == it2);
    ++it2;
    BOOST_TEST(it1 != it2);
  }
  BOOST_AUTO_TEST_SUITE_END()

  BOOST_AUTO_TEST_SUITE(INCRIMENTS)

  BOOST_AUTO_TEST_CASE(postfix_increment)
  {
    List< int > list;
    list.add(2);
    list.add(1);
    auto it = list.begin();
    auto old = it++;
    BOOST_TEST(*old == 1);
    BOOST_TEST(*it == 2);
  }
  BOOST_AUTO_TEST_SUITE_END()
}
