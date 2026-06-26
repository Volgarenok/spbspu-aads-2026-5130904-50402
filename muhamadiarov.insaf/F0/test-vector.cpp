#include <boost/test/unit_test.hpp>
#include "vector.hpp"

using namespace muhamadiarov;

BOOST_AUTO_TEST_SUITE(VectorTest)

BOOST_AUTO_TEST_CASE(ConstructorWithCapacity)
{
  Vector< int > v(10);
  BOOST_CHECK_EQUAL(v.size(), 0);
  BOOST_CHECK_EQUAL(v.capacity(), 10);
  BOOST_CHECK(v.isEmpty());
}

BOOST_AUTO_TEST_CASE(CopyConstructor)
{
  Vector< int > v1;
  v1.pushBack(10);
  v1.pushBack(20);
  v1.pushBack(30);

  Vector< int > v2(v1);
  BOOST_CHECK_EQUAL(v2.size(), 3);
  BOOST_CHECK_EQUAL(v2[0], 10);
  BOOST_CHECK_EQUAL(v2[1], 20);
  BOOST_CHECK_EQUAL(v2[2], 30);
}

BOOST_AUTO_TEST_CASE(MoveConstructor)
{
  Vector< int > v1;
  v1.pushBack(10);
  v1.pushBack(20);
  v1.pushBack(30);

  Vector< int > v2(std::move(v1));
  BOOST_CHECK_EQUAL(v2.size(), 3);
  BOOST_CHECK_EQUAL(v2[0], 10);
  BOOST_CHECK_EQUAL(v2[1], 20);
  BOOST_CHECK_EQUAL(v2[2], 30);
  BOOST_CHECK_EQUAL(v1.size(), 0);
  BOOST_CHECK_EQUAL(v1.capacity(), 0);
}

BOOST_AUTO_TEST_CASE(CopyAssignment)
{
  Vector< int > v1;
  v1.pushBack(10);
  v1.pushBack(20);
  v1.pushBack(30);

  Vector< int > v2;
  v2 = v1;

  BOOST_CHECK_EQUAL(v2.size(), 3);
  BOOST_CHECK_EQUAL(v2[0], 10);
  BOOST_CHECK_EQUAL(v2[1], 20);
  BOOST_CHECK_EQUAL(v2[2], 30);
}

BOOST_AUTO_TEST_CASE(MoveAssignment)
{
  Vector< int > v1;
  v1.pushBack(10);
  v1.pushBack(20);
  v1.pushBack(30);

  Vector< int > v2;
  v2 = std::move(v1);

  BOOST_CHECK_EQUAL(v2.size(), 3);
  BOOST_CHECK_EQUAL(v2[0], 10);
  BOOST_CHECK_EQUAL(v2[1], 20);
  BOOST_CHECK_EQUAL(v2[2], 30);
  BOOST_CHECK_EQUAL(v1.size(), 0);
  BOOST_CHECK_EQUAL(v1.capacity(), 0);
}

BOOST_AUTO_TEST_CASE(OperatorBracket)
{
  Vector< int > v;
  v.pushBack(10);

  BOOST_CHECK_EQUAL(v[0], 10);

  v[0] = 99;
  BOOST_CHECK_EQUAL(v[0], 99);
}

BOOST_AUTO_TEST_CASE(AtMethod)
{
  Vector< int > v;
  v.pushBack(10);
  v.pushBack(20);
  v.pushBack(30);

  BOOST_CHECK_EQUAL(v.at(0), 10);
  BOOST_CHECK_EQUAL(v.at(1), 20);
  BOOST_CHECK_EQUAL(v.at(2), 30);

  v.at(1) = 99;
  BOOST_CHECK_EQUAL(v.at(1), 99);
}

BOOST_AUTO_TEST_CASE(PushBackLValue)
{
  Vector< int > v;
  int value = 10;
  v.pushBack(value);

  BOOST_CHECK_EQUAL(v.size(), 1);
  BOOST_CHECK_EQUAL(v[0], 10);
}

BOOST_AUTO_TEST_CASE(PushBackRValue)
{
  Vector< int > v;
  v.pushBack(10);

  BOOST_CHECK_EQUAL(v.size(), 1);
  BOOST_CHECK_EQUAL(v[0], 10);
}

BOOST_AUTO_TEST_CASE(PushBackMultiple)
{
  Vector< int > v;
  for (int i = 0; i < 100; ++i)
  {
      v.pushBack(i);
  }

  BOOST_CHECK_EQUAL(v.size(), 100);
  BOOST_CHECK_GE(v.capacity(), 100);

  for (int i = 0; i < 100; ++i)
  {
      BOOST_CHECK_EQUAL(v[i], i);
  }
}

BOOST_AUTO_TEST_CASE(PopBack)
{
  Vector< int > v;
  v.pushBack(10);
  v.pushBack(20);
  v.pushBack(30);

  v.popBack();
  BOOST_CHECK_EQUAL(v.size(), 2);
  BOOST_CHECK_EQUAL(v[0], 10);
  BOOST_CHECK_EQUAL(v[1], 20);

  v.popBack();
  BOOST_CHECK_EQUAL(v.size(), 1);
  BOOST_CHECK_EQUAL(v[0], 10);

  v.popBack();
  BOOST_CHECK_EQUAL(v.size(), 0);
  BOOST_CHECK(v.isEmpty());
}

BOOST_AUTO_TEST_CASE(PushFrontLValue)
{
  Vector< int > v;
  int value = 10;
  v.pushFront(value);
  BOOST_CHECK_EQUAL(v.size(), 1);
  BOOST_CHECK_EQUAL(v[0], 10);

  v.pushFront(20);
  BOOST_CHECK_EQUAL(v.size(), 2);
  BOOST_CHECK_EQUAL(v[0], 20);
  BOOST_CHECK_EQUAL(v[1], 10);
}

BOOST_AUTO_TEST_CASE(PushFrontRValue)
{
  Vector< int > v;
  v.pushFront(10);
  BOOST_CHECK_EQUAL(v.size(), 1);
  BOOST_CHECK_EQUAL(v[0], 10);

  v.pushFront(20);
  BOOST_CHECK_EQUAL(v.size(), 2);
  BOOST_CHECK_EQUAL(v[0], 20);
  BOOST_CHECK_EQUAL(v[1], 10);
}

BOOST_AUTO_TEST_CASE(PushFrontMultiple)
{
  Vector< int > v;
  for (int i = 0; i < 50; ++i)
  {
      v.pushFront(i);
  }
  BOOST_CHECK_EQUAL(v.size(), 50);
  BOOST_CHECK_EQUAL(v[0], 49);
  BOOST_CHECK_EQUAL(v[49], 0);
}

BOOST_AUTO_TEST_CASE(PopFront)
{
  Vector< int > v;
  v.pushBack(10);
  v.pushBack(20);
  v.pushBack(30);

  v.popFront();
  BOOST_CHECK_EQUAL(v.size(), 2);
  BOOST_CHECK_EQUAL(v[0], 20);
  BOOST_CHECK_EQUAL(v[1], 30);

  v.popFront();
  BOOST_CHECK_EQUAL(v.size(), 1);
  BOOST_CHECK_EQUAL(v[0], 30);

  v.popFront();
  BOOST_CHECK_EQUAL(v.size(), 0);
  BOOST_CHECK(v.isEmpty());
}

BOOST_AUTO_TEST_CASE(InsertLValue)
{
  Vector< int > v;
  v.pushBack(10);
  v.pushBack(20);
  v.pushBack(30);

  int value = 99;
  v.insert(1, value);

  BOOST_CHECK_EQUAL(v.size(), 4);
  BOOST_CHECK_EQUAL(v[0], 10);
  BOOST_CHECK_EQUAL(v[1], 99);
  BOOST_CHECK_EQUAL(v[2], 20);
  BOOST_CHECK_EQUAL(v[3], 30);
}

BOOST_AUTO_TEST_CASE(InsertRValue)
{
  Vector<int> v;
  v.pushBack(10);
  v.pushBack(20);
  v.pushBack(30);

  v.insert(1, 99);

  BOOST_CHECK_EQUAL(v.size(), 4);
  BOOST_CHECK_EQUAL(v[0], 10);
  BOOST_CHECK_EQUAL(v[1], 99);
  BOOST_CHECK_EQUAL(v[2], 20);
  BOOST_CHECK_EQUAL(v[3], 30);
}

BOOST_AUTO_TEST_CASE(EmptyMethod)
{
  Vector< int > v;
  BOOST_CHECK(v.isEmpty());
  v.pushBack(10);
  BOOST_CHECK(!v.isEmpty());
}

BOOST_AUTO_TEST_CASE(ClearMethod)
{
  Vector< int > v(3);
  v.pushBack(10);
  v.pushBack(20);
  v.pushBack(30);

  v.clear();

  BOOST_CHECK_EQUAL(v.size(), 0);
  BOOST_CHECK(v.isEmpty());
  BOOST_CHECK_EQUAL(v.capacity(), 3);
}

BOOST_AUTO_TEST_CASE(EraseMethod)
{
  Vector<int> v;
  v.pushBack(10);
  v.pushBack(20);
  v.pushBack(30);

  v.erase(1);

  BOOST_CHECK_EQUAL(v.size(), 2);
  BOOST_CHECK_EQUAL(v[0], 10);
  BOOST_CHECK_EQUAL(v[1], 30);
}

BOOST_AUTO_TEST_CASE(ReserveMethod)
{
  Vector<int> v;
  v.pushBack(10);
  v.pushBack(20);
  v.pushBack(30);

  v.reserve(100);

  BOOST_CHECK_GE(v.capacity(), 100);
  BOOST_CHECK_EQUAL(v.size(), 3);
  BOOST_CHECK_EQUAL(v[0], 10);
  BOOST_CHECK_EQUAL(v[1], 20);
  BOOST_CHECK_EQUAL(v[2], 30);
}

BOOST_AUTO_TEST_SUITE_END()
