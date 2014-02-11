#include <boost/test/unit_test.hpp>

#include "mock.hpp"

BOOST_AUTO_TEST_SUITE( test_mock )

BOOST_AUTO_TEST_CASE( mock_should_verify_createICombinator ) {
  ModuleWriterMock mock;
  BOOST_REQUIRE( !mock.didCreateI() );
  mock.createICombinator();
  BOOST_REQUIRE( mock.didCreateI());
}

BOOST_AUTO_TEST_SUITE_END()
