#include <boost/test/unit_test.hpp>

#include "../src/module_writer.hpp"
#include "../src/generator.hpp"
#include "test_jit.hpp"

struct ModuleFixture {
  TestJIT *jit;
  SlalomFunctionStruct *iComb;
  SlalomFunctionStruct *kComb;

  ModuleFixture() {
    jit = new TestJIT();
    FactoryFunction createI = jit->getFactoryFunctionPointer("createICombinator");
    iComb = createI();
    FactoryFunction createK = jit->getFactoryFunctionPointer("createKCombinator");
    kComb = createK();
  }

  ~ModuleFixture() {
    delete iComb;
    delete jit;
  }
};

BOOST_FIXTURE_TEST_SUITE( test_module_writer, ModuleFixture )

BOOST_AUTO_TEST_CASE( print_module ) {
  jit->dumpModule();
}

BOOST_AUTO_TEST_CASE( i_comb_arity_is_1 ) {
  BOOST_CHECK_EQUAL(iComb->arity, 1);
}

BOOST_AUTO_TEST_CASE( i_comb_name_is_I ) {
  BOOST_CHECK_EQUAL(iComb->name, "I");
}

BOOST_AUTO_TEST_CASE( i_comb_args_queue_is_empty ) {
  QueueStruct *args = iComb->arguments;
  BOOST_CHECK_EQUAL(args->length, 0);

  QueueNodeStruct *head = args->head;
  BOOST_CHECK(!head->data);
  BOOST_CHECK(!head->next);

  QueueNodeStruct *tail = args->tail;
  BOOST_CHECK(!tail->data);  
  BOOST_CHECK(!tail->next);
}

BOOST_AUTO_TEST_CASE( k_comb_arity_is_2 ) {
  BOOST_CHECK_EQUAL(kComb->arity, 2);
}

BOOST_AUTO_TEST_CASE( k_comb_name_is_K ) {
  BOOST_CHECK_EQUAL(kComb->name, "K");
}

BOOST_AUTO_TEST_CASE( k_comb_args_queue_is_empty ) {
  QueueStruct *args = kComb->arguments;
  BOOST_CHECK_EQUAL(args->length, 0);

  QueueNodeStruct *head = args->head;
  BOOST_CHECK(!head->data);
  BOOST_CHECK(!head->next);
}

BOOST_AUTO_TEST_SUITE_END()
