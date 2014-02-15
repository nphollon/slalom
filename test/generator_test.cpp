#include <boost/test/unit_test.hpp>
#include "hippomocks.h"
#include "factory.hpp"
#include "../src/node.hpp"
#include "../src/generator.hpp"

static MockRepository mocks;

struct GeneratorFixture {
  FunctionFactory factory;
  Generator *generator;
  ModuleWriter *writer;

  GeneratorFixture() {
    mocks.autoExpect = false;
    writer = mocks.InterfaceMock<ModuleWriter>();
    generator = new Generator(*writer);
  }

  ~GeneratorFixture() {
    // don't need to explicitly delete writer mock
    factory.deleteFuncs();
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
  SlalomFunction *f1 = factory.buildFunction();
  SlalomFunction *f2 = factory.buildFunction();

  mocks.ExpectCall(writer, ModuleWriter::createKCombinator).Return(f1);
  mocks.ExpectCall(writer, ModuleWriter::createICombinator).Return(f2);
  mocks.ExpectCall(writer, ModuleWriter::createApplication).With(f1, f2).Return(NULL);

  generator->generate("K I");
}

BOOST_AUTO_TEST_CASE( program_s_k_i_p_should_generate_applications_recursively ) {
  SlalomFunction *f1 = factory.buildFunction();
  SlalomFunction *f2 = factory.buildFunction();
  SlalomFunction *f3 = factory.buildFunction();
  SlalomFunction *f4 = factory.buildFunction();
  SlalomFunction *apply12 = factory.buildFunction();
  SlalomFunction *apply34 = factory.buildFunction();

  mocks.ExpectCall(writer, ModuleWriter::createSCombinator).Return(f1);
  mocks.ExpectCall(writer, ModuleWriter::createKCombinator).Return(f2);
  mocks.ExpectCall(writer, ModuleWriter::createApplication).With(f1, f2).Return(apply12);

  mocks.ExpectCall(writer, ModuleWriter::createICombinator).Return(f3);
  mocks.ExpectCall(writer, ModuleWriter::createDerivedCombinator).With("P").Return(f4);
  mocks.ExpectCall(writer, ModuleWriter::createApplication).With(f3, f4).Return(apply34);

  mocks.ExpectCall(writer, ModuleWriter::createApplication).With(apply12, apply34).Return(NULL);

  generator->generate("S K (I P)");
}

BOOST_AUTO_TEST_SUITE_END()
