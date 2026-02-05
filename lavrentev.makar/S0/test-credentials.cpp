#define BOOST_TEST_DYN_LINK 
#define BOOST_TEST_MODULE CredentialsTest

#include <boost/test/unit_test.hpp>
#include <sstream>
#include "credentials.hpp"

BOOST_AUTO_TEST_CASE(credentials_test)
{
    std::ostringstream out;
    lavrentev::out_credentials(out);
    BOOST_TEST(out.str() == "lavrentev.makar");
};
