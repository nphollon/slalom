#include <boost/test/unit_test.hpp>

#include "../node.hpp"

struct ApplyNodeFixture {
  NodeFactory factory;
  const Node *childA;
  const Node *childB;
  const Node *parentAB;

  ApplyNodeFixture() {
    childA = factory.buildNode("A");
    childB = factory.buildNode("B");
    parentAB = factory.buildNode(*childA, *childB);
  }
  ~ApplyNodeFixture() {
    factory.deleteNodes();
  }
};

BOOST_FIXTURE_TEST_SUITE( test_apply_node, ApplyNodeFixture )

BOOST_AUTO_TEST_CASE( getName_on_apply_node_should_combine_names_of_children ) {
  BOOST_CHECK_EQUAL( parentAB->getName(), "(A B)" );

  const Node *parentBA = factory.buildNode(*childB, *childA);
  BOOST_CHECK_EQUAL( parentBA->getName(), "(B A)" );
}

BOOST_AUTO_TEST_CASE( apply_node_should_not_be_terminal ) {
  BOOST_CHECK( !parentAB->isTerminal() );
}

BOOST_AUTO_TEST_CASE( apply_node_applicator_should_be_first_child ) {
  BOOST_CHECK_EQUAL( *parentAB->getApplicator(), *childA );
}

BOOST_AUTO_TEST_CASE( apply_node_input_should_be_second_child ) {
  BOOST_CHECK_EQUAL( *parentAB->getInput(), *childB );
}

BOOST_AUTO_TEST_CASE( apply_node_should_not_equal_name_node ) {
  const Node *childless = factory.buildNode("(A B)");
  BOOST_CHECK_NE( *childless, *parentAB );
  BOOST_CHECK_NE( *parentAB, *childless );
}

BOOST_AUTO_TEST_CASE( apply_node_should_check_equality_of_children ) {
  // This test ensures that the node equality operator does a deep comparison,
  // not merely comparing results of getName()
  const Node *childless = factory.buildNode("(A B)");
  const Node *grandparentABC = factory.buildNode(*parentAB, *childless);
  const Node *grandparentCAB = factory.buildNode(*childless, *parentAB);
  BOOST_CHECK_NE( *grandparentABC, *grandparentCAB );
}

BOOST_AUTO_TEST_CASE( copy_constructor_should_create_deep_copy ) {
  const Node *copy = new Node(*parentAB);
  factory.deleteNodes(); // deletes childA, childB, and parentAB

  BOOST_CHECK_EQUAL( copy->getName(), "(A B)" );
  BOOST_CHECK_EQUAL( *copy->getApplicator(), *factory.buildNode("A") );
  BOOST_CHECK_EQUAL( *copy->getInput(), *factory.buildNode("B") );

  delete copy;
}

BOOST_AUTO_TEST_SUITE_END()
