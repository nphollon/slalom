#include <boost/test/unit_test.hpp>

#include "../src/module_writer.hpp"
#include "../src/generator.hpp"
#include "test_jit.hpp"

struct ModuleFixture {
  TestJIT *jit;
  SlalomFunctionStruct *sfs;

  ModuleFixture() {
    jit = new TestJIT();
    FactoryFunction createI = jit->getFactoryFunctionPointer("createICombinator");
    sfs = createI();
  }

  ~ModuleFixture() {
    delete sfs;
    delete jit;
  }
};

BOOST_FIXTURE_TEST_SUITE( test_module_writer, ModuleFixture )

BOOST_AUTO_TEST_CASE( i_comb_arity_is_1 ) {
  BOOST_CHECK_EQUAL(sfs->arity, 1);
}

BOOST_AUTO_TEST_CASE( i_comb_name_is_I ) {
  BOOST_CHECK_EQUAL(sfs->name, "I");
}

BOOST_AUTO_TEST_CASE( i_comb_has_a_queue_of_length_0 ) {
  QueueStruct *args = sfs->arguments;
  BOOST_CHECK_EQUAL(args->length, 0);
}

BOOST_AUTO_TEST_CASE( i_comb_has_a_queue_with_head_null ) {
  QueueStruct *args = sfs->arguments;
  QueueNodeStruct *head = args->head;
  BOOST_CHECK(!head->data);
  BOOST_CHECK(!head->next);
}

BOOST_AUTO_TEST_CASE( i_comb_has_a_queue_with_tail_null ) {
  QueueStruct *args = sfs->arguments;
  QueueNodeStruct *tail = args->tail;
  BOOST_CHECK(!tail->data);  
  BOOST_CHECK(!tail->next);
}

BOOST_AUTO_TEST_SUITE_END()
