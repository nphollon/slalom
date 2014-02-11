#include <boost/test/unit_test.hpp>

#include "hippomocks.h"

#include "../src/parse.hpp"
#include "../src/generate.hpp"

static MockRepository mocks;

struct GeneratorFixture {
  ModuleWriter *writer;

  GeneratorFixture() {
    writer = mocks.InterfaceMock<ModuleWriter>();
  };

  ~GeneratorFixture() {};

  void generateFromProgram(const std::string &program) {
    generate(writer, parse(program));
  }
};

BOOST_FIXTURE_TEST_SUITE( test_generator, GeneratorFixture )

BOOST_AUTO_TEST_CASE( writer_should_create_i_when_program_is_i ) {
  mocks.ExpectCall(writer, ModuleWriter::createICombinator);
  generateFromProgram("I");
}

BOOST_AUTO_TEST_SUITE_END()
