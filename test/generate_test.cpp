#include <boost/test/unit_test.hpp>

#include "../src/parse.hpp"
#include "mock.hpp"

ModuleWriterMock writeToMock(const std::string& program) {
  ModuleWriterMock mock;
  generate(mock, parse(program));
  return mock;
}

BOOST_AUTO_TEST_SUITE( test_generator )

BOOST_AUTO_TEST_CASE( writer_should_create_i_when_program_is_i ) {
  ModuleWriterMock writer = writeToMock("I");
  BOOST_CHECK( writer.didCreateI() );
}

BOOST_AUTO_TEST_SUITE_END()
