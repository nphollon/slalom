#include <boost/test/unit_test.hpp>
#include "hippomocks.h"
#include "../src/node.hpp"
#include "../src/generator.hpp"

static MockRepository mocks;

struct GeneratorFixture {
  Generator *generator;
  ModuleWriter *writer;

  GeneratorFixture() {
    writer = mocks.InterfaceMock<ModuleWriter>();
    generator = new Generator(*writer);
  }

  ~GeneratorFixture() {
    // don't need to explicitly delete writer mock
    delete generator;
  }
};

BOOST_FIXTURE_TEST_SUITE( test_generator, GeneratorFixture )

BOOST_AUTO_TEST_CASE( program_i_should_generate_i_comb ) {
  mocks.ExpectCall(writer, ModuleWriter::createICombinator).Return(NULL);
  generator->generate("I");
}

BOOST_AUTO_TEST_CASE( program_k_should_generate_k_comb ) {
  mocks.ExpectCall(writer, ModuleWriter::createKCombinator).Return(NULL);
  generator->generate("K");
}

BOOST_AUTO_TEST_CASE( program_s_should_generate_s_comb ) {
  mocks.ExpectCall(writer, ModuleWriter::createSCombinator).Return(NULL);
  generator->generate("S");
}

BOOST_AUTO_TEST_CASE( generic_name_node_should_generate_generic_combinator ) {
  mocks.ExpectCall(writer, ModuleWriter::createDerivedCombinator).With("name1").Return(NULL);
  mocks.ExpectCall(writer, ModuleWriter::createDerivedCombinator).With("name2").Return(NULL);
  generator->generate("name1");
  generator->generate("name2");
}

BOOST_AUTO_TEST_CASE( program_k_i_should_generate_combs_and_apply_them ) {
  SlalomFunction *kComb = new SlalomFunction();
  SlalomFunction *iComb = new SlalomFunction();
  mocks.ExpectCall(writer, ModuleWriter::createKCombinator).Return(kComb);
  mocks.ExpectCall(writer, ModuleWriter::createICombinator).Return(iComb);
  mocks.ExpectCall(writer, ModuleWriter::createApplication).With(kComb, iComb).Return(NULL);
  generator->generate("K I");
}

BOOST_AUTO_TEST_SUITE_END()
