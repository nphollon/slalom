#include <boost/test/unit_test.hpp>

#include "../src/module_writer.hpp"
#include "../src/generator.hpp"
#include "test_jit.hpp"

struct ModuleFixture {
  TestJIT *jit;
  FactoryFunction createI;

  ModuleFixture() {
    jit = new TestJIT();
    createI = jit->getFactoryFunctionPointer("createICombinator");
  }

  ~ModuleFixture() {
    delete jit;
  }
};

BOOST_FIXTURE_TEST_SUITE( test_module_writer, ModuleFixture )

BOOST_AUTO_TEST_CASE( i_comb_arity_is_1 ) {
  SlalomFunctionStruct *sfs = createI();
  BOOST_CHECK_EQUAL(sfs->arity, 1);
  delete sfs;
}

BOOST_AUTO_TEST_CASE( i_comb_name_is_I ) {
  jit->dumpModule();
  SlalomFunctionStruct *sfs = createI();
  BOOST_CHECK_EQUAL(sfs->name, "I");
  delete sfs;
}

BOOST_AUTO_TEST_SUITE_END()

