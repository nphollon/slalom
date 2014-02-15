#include <boost/test/unit_test.hpp>

#include "hippomocks.h"

#include "../src/node.hpp"
#include "../src/generate.hpp"

static MockRepository mocks;

struct GeneratorFixture {
  ModuleWriter *writer;

  GeneratorFixture() {
    writer = mocks.InterfaceMock<ModuleWriter>();
  };

  ~GeneratorFixture() {};

  void generateFromProgram(const std::string &program) {
    generate(writer, Node::parse(program));
  }
};

BOOST_FIXTURE_TEST_SUITE( test_generator, GeneratorFixture )

BOOST_AUTO_TEST_CASE( program_i_should_generate_i_comb ) {
  mocks.ExpectCall(writer, ModuleWriter::createICombinator);
  generateFromProgram("I");
}

BOOST_AUTO_TEST_CASE( program_k_should_generate_k_comb ) {
  mocks.ExpectCall(writer, ModuleWriter::createKCombinator);
  generateFromProgram("K");
}

BOOST_AUTO_TEST_CASE( program_s_should_generate_s_comb ) {
  mocks.ExpectCall(writer, ModuleWriter::createSCombinator);
  generateFromProgram("S");
}

BOOST_AUTO_TEST_CASE( generic_name_node_should_generate_generic_combinator ) {
  mocks.ExpectCall(writer, ModuleWriter::createDerivedCombinator).With("name1");
  mocks.ExpectCall(writer, ModuleWriter::createDerivedCombinator).With("name2");
  generateFromProgram("name1");
  generateFromProgram("name2");
}

BOOST_AUTO_TEST_SUITE_END()
