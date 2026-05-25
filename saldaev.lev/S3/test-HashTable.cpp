#include <functional>
#include <stdexcept>
#include <boost/test/unit_test.hpp>
#include "HashTable.hpp"

namespace saldaev
{
  BOOST_AUTO_TEST_SUITE(HashTableTests)

  using TestTable = HashTable< int, int, std::hash< int >, std::equal_to< int > >;

  TestTable make_sample_table()
  {
    TestTable t(10, std::hash< int >(), std::equal_to< int >());
    t.add(1, 100);
    t.add(2, 200);
    t.add(3, 300);
    return t;
  }

  BOOST_AUTO_TEST_CASE(default_constructor_and_empty)
  {
    TestTable t(8, std::hash< int >(), std::equal_to< int >());
    BOOST_TEST(t.empty());
    BOOST_TEST(t.size() == 0);
    BOOST_TEST(!t.has(42));
  }

  BOOST_AUTO_TEST_CASE(add_and_has)
  {
    TestTable t(4, std::hash< int >(), std::equal_to< int >());
    BOOST_TEST(t.empty());

    t.add(10, 1000);
    BOOST_TEST(!t.empty());
    BOOST_TEST(t.size() == 1);
    BOOST_TEST(t.has(10));
    BOOST_TEST(!t.has(20));

    t.add(20, 2000);
    BOOST_TEST(t.size() == 2);
    BOOST_TEST(t.has(10));
    BOOST_TEST(t.has(20));
  }

  BOOST_AUTO_TEST_CASE(get)
  {
    TestTable t = make_sample_table();
    BOOST_TEST(t.get(1) == 100);
    BOOST_TEST(t.get(2) == 200);
    BOOST_TEST(t.get(3) == 300);

    const TestTable &ct = t;
    BOOST_TEST(ct.get(2) == 200);

    BOOST_CHECK_THROW(t.get(99), std::exception);
    BOOST_CHECK_THROW(ct.get(99), std::exception);
  }

  BOOST_AUTO_TEST_CASE(remove)
  {
    TestTable t = make_sample_table();
    BOOST_TEST(t.size() == 3);

    t.remove(2);
    BOOST_TEST(t.size() == 2);
    BOOST_TEST(!t.has(2));
    BOOST_TEST(t.has(1));
    BOOST_TEST(t.has(3));

    BOOST_CHECK_THROW(t.remove(99), std::exception);
    BOOST_TEST(t.size() == 2);
    BOOST_TEST(t.has(1));
    BOOST_TEST(t.has(3));

    t.remove(1);
    t.remove(3);
    BOOST_TEST(t.empty());
    BOOST_TEST(t.size() == 0);
  }

  BOOST_AUTO_TEST_CASE(rewrite)
  {
    TestTable t = make_sample_table();
    BOOST_TEST(t.get(2) == 200);

    t.rewrite(2, 555);
    BOOST_TEST(t.get(2) == 555);
    BOOST_TEST(t.size() == 3);
    BOOST_TEST(t.has(1));
    BOOST_TEST(t.has(3));

    BOOST_CHECK_THROW(t.rewrite(10, 999), std::exception);
    BOOST_TEST(t.size() == 3);
    BOOST_TEST(t.has(1));
    BOOST_TEST(t.has(2));
    BOOST_TEST(t.has(3));
    BOOST_TEST(t.get(2) == 555);
  }

  BOOST_AUTO_TEST_CASE(rehash)
  {
    TestTable t = make_sample_table();
    size_t old_size = t.size();

    t.rehash(20);
    BOOST_TEST(t.size() == old_size);
    BOOST_TEST(t.has(1));
    BOOST_TEST(t.has(2));
    BOOST_TEST(t.has(3));
    BOOST_TEST(t.get(1) == 100);
    BOOST_TEST(t.get(2) == 200);
    BOOST_TEST(t.get(3) == 300);

    t.rehash(2);
    BOOST_TEST(t.size() == old_size);
    BOOST_TEST(t.has(1));
    BOOST_TEST(t.has(2));
    BOOST_TEST(t.has(3));
  }

  BOOST_AUTO_TEST_CASE(clear)
  {
    TestTable t = make_sample_table();
    BOOST_TEST(!t.empty());

    t.clear();
    BOOST_TEST(t.empty());
    BOOST_TEST(t.size() == 0);
    BOOST_TEST(!t.has(1));

    BOOST_CHECK_NO_THROW(t.clear());
    BOOST_TEST(t.empty());
  }

  BOOST_AUTO_TEST_CASE(copy_constructor)
  {
    TestTable original = make_sample_table();
    TestTable copy(original);

    BOOST_TEST(copy.size() == original.size());
    BOOST_TEST(copy.get(1) == 100);
    BOOST_TEST(copy.get(2) == 200);
    BOOST_TEST(copy.get(3) == 300);

    copy.rewrite(2, 999);
    copy.remove(3);
    BOOST_TEST(original.get(2) == 200);
    BOOST_TEST(original.has(3));

    TestTable empty(5, std::hash< int >(), std::equal_to< int >());
    TestTable copyEmpty(empty);
    BOOST_TEST(copyEmpty.empty());
  }

  BOOST_AUTO_TEST_CASE(copy_assignment)
  {
    TestTable a = make_sample_table();
    TestTable b(2, std::hash< int >(), std::equal_to< int >());
    b.add(99, 9999);

    b = a;
    BOOST_TEST(b.size() == a.size());
    BOOST_TEST(b.get(1) == 100);
    BOOST_TEST(b.get(2) == 200);
    BOOST_TEST(!b.has(99));

    a = a;
    BOOST_TEST(a.size() == 3);
    BOOST_TEST(a.get(1) == 100);

    TestTable empty(1, std::hash< int >(), std::equal_to< int >());
    b = empty;
    BOOST_TEST(b.empty());
  }

  BOOST_AUTO_TEST_CASE(move_constructor)
  {
    TestTable src = make_sample_table();
    TestTable dst(std::move(src));

    BOOST_TEST(dst.size() == 3);
    BOOST_TEST(dst.get(1) == 100);
    BOOST_TEST(dst.get(2) == 200);
    BOOST_TEST(dst.get(3) == 300);

    TestTable empty(4, std::hash< int >(), std::equal_to< int >());
    TestTable movedEmpty(std::move(empty));
    BOOST_TEST(movedEmpty.empty());
    BOOST_TEST(empty.empty());
  }

  BOOST_AUTO_TEST_CASE(move_assignment)
  {
    TestTable a = make_sample_table();
    TestTable b(1, std::hash< int >(), std::equal_to< int >());
    b.add(7, 77);

    b = std::move(a);
    BOOST_TEST(b.size() == 3);
    BOOST_TEST(b.get(1) == 100);
    BOOST_TEST(!b.has(7));

    TestTable &ref = b;
    b = std::move(ref);
    BOOST_TEST(b.size() == 3);
    BOOST_TEST(b.get(1) == 100);

    TestTable empty(2, std::hash< int >(), std::equal_to< int >());
    TestTable target(4, std::hash< int >(), std::equal_to< int >());
    target.add(42, 4242);
    target = std::move(empty);
    BOOST_TEST(target.empty());
    BOOST_TEST(empty.empty());
  }

  BOOST_AUTO_TEST_CASE(swap)
  {
    TestTable t1(4, std::hash< int >(), std::equal_to< int >());
    t1.add(1, 10);
    t1.add(2, 20);

    TestTable t2(8, std::hash< int >(), std::equal_to< int >());
    t2.add(3, 30);

    t1.swap(t2);

    BOOST_TEST(t1.size() == 1);
    BOOST_TEST(t1.has(3));
    BOOST_TEST(t1.get(3) == 30);
    BOOST_TEST(!t1.has(1));

    BOOST_TEST(t2.size() == 2);
    BOOST_TEST(t2.has(1));
    BOOST_TEST(t2.has(2));
    BOOST_TEST(t2.get(1) == 10);
    BOOST_TEST(t2.get(2) == 20);

    TestTable empty(1, std::hash< int >(), std::equal_to< int >());
    t1.swap(empty);
    BOOST_TEST(t1.empty());
    BOOST_TEST(empty.size() == 1);
    BOOST_TEST(empty.has(3));
  }

  BOOST_AUTO_TEST_SUITE_END()
}
