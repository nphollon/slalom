#include <boost/test/unit_test.hpp>

#include "../node.hpp"

struct NameNodeFixture {
  NodeFactory factory;
  const Node *aNode;

  NameNodeFixture() {
    aNode = factory.buildNode("A");
  }
  ~NameNodeFixture() {
    factory.deleteNodes();
  }
};

BOOST_FIXTURE_TEST_SUITE( test_name_node, NameNodeFixture )

BOOST_AUTO_TEST_CASE( node_should_equal_node_with_same_name ) {
  const Node *identicalNode = factory.buildNode("A");
  BOOST_CHECK( *aNode == *identicalNode );
  BOOST_CHECK( *identicalNode == *aNode );
  BOOST_CHECK( !(*aNode != *identicalNode) );
}

BOOST_AUTO_TEST_CASE( node_should_not_equal_node_with_different_name ) {
  const Node *differentNode = factory.buildNode("B");
  BOOST_CHECK( !(*aNode == *differentNode) );
  BOOST_CHECK( *aNode != *differentNode );
  BOOST_CHECK( *differentNode != *aNode );
}

BOOST_AUTO_TEST_CASE( name_node_should_be_terminal ) {
  BOOST_CHECK( aNode->isTerminal() );
  BOOST_CHECK( !aNode->getApplicator() );
  BOOST_CHECK( !aNode->getInput() );
}

BOOST_AUTO_TEST_CASE( getName_on_name_node_should_return_name ) {
  BOOST_CHECK_EQUAL( aNode->getName(), "A" );
}

BOOST_AUTO_TEST_CASE( name_node_should_be_immutable ) {
  std::string mutatedName = aNode->getName();
  mutatedName += "more stuff";
  BOOST_CHECK_EQUAL( aNode->getName(), "A" );
}

BOOST_AUTO_TEST_SUITE_END()
