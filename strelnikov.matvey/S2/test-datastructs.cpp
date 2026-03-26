#include "boost/test/unit_test.hpp"
#include "datastructs.hpp"

using namespace strelnikov;

BOOST_AUTO_TEST_SUITE(STACK_AND_QUEUE_TESTS)


BOOST_AUTO_TEST_CASE(STACK_EMPTY_TEST)
{
	Stack< int > st;

	BOOST_CHECK_EQUAL(st.empty(), true);
	st.push(5);
	BOOST_CHECK_EQUAL(st.empty(), false);
}

BOOST_AUTO_TEST_CASE(STACK_GET_TEST)
{
	Stack< int > st;
	st.push(5);
	BOOST_CHECK_EQUAL(st.get(), 5);
}

BOOST_AUTO_TEST_CASE(STACK_PUSH_TEST)
{
	Stack< int > st;
	st.push(5);
	BOOST_CHECK_EQUAL(st.get(), 5);
}

BOOST_AUTO_TEST_CASE(STACK_POP_TEST)
{
	Stack< int > st;
	st.push(5);
	BOOST_CHECK_EQUAL(st.get(), 5);
	st.pop();
	BOOST_CHECK_EQUAL(st.empty(), true);
}

BOOST_AUTO_TEST_CASE(QUEUE_EMPTY_TEST)
{
	Queue< int > q;

	BOOST_CHECK_EQUAL(q.empty(), true);
	q.push(5);
	BOOST_CHECK_EQUAL(q.empty(), false);
}

BOOST_AUTO_TEST_CASE(QUEUE_GET_TEST)
{
	Queue< int > q;
	q.push(5);
	BOOST_CHECK_EQUAL(q.get(), 5);
}

BOOST_AUTO_TEST_CASE(QUEUE_PUSH_TEST)
{
	Queue< int > q;
	q.push(5);
	BOOST_CHECK_EQUAL(q.get(), 5);
}

BOOST_AUTO_TEST_CASE(QUEUE_POP_TEST)
{
	Queue< int > q;
	q.push(5);
	BOOST_CHECK_EQUAL(q.get(), 5);
	q.pop();
	BOOST_CHECK_EQUAL(q.empty(), true);
}

BOOST_AUTO_TEST_SUITE_END()