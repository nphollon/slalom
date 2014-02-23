#include <boost/test/unit_test.hpp>

#include "../src/module_writer.hpp"
#include "../src/generator.hpp"
#include "test_jit.hpp"

struct ModuleFixture {
  TestJIT *jit;

  ModuleFixture() {
    jit = new TestJIT();
  }

  ~ModuleFixture() {
    delete jit;
  }
};

BOOST_FIXTURE_TEST_SUITE( test_module_writer, ModuleFixture )

BOOST_AUTO_TEST_CASE( i_comb_arity_is_1 ) {
  FactoryFunction createI = jit->getFunctionPointer("createICombinator");
  //  SlalomFunctionStruct *sfs = createI();
  //BOOST_CHECK_EQUAL(sfs->arity, 1);
}

BOOST_AUTO_TEST_SUITE_END()

