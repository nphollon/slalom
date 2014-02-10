#define BOOST_TEST_MODULE SlalomTest
#include <boost/test/included/unit_test.hpp>

#include "node.hpp"
#include "parse.hpp"
#include "generate.hpp"
#include "tester.hpp"
#include "mock.hpp"

ModuleWriterMock writeToMock(const string& program) {
  ModuleWriterMock mock;
  generate(mock, parse(program));
  return mock;
}


void testMock(Tester& tester) {
  ModuleWriterMock mock;
  tester.verify(!mock.didCreateI(), "Expected mock.createICombinator() to not be called");
  mock.createICombinator();
  tester.verify(mock.didCreateI(), "Expected mock.createICombinator() to be called");
}

BOOST_AUTO_TEST_SUITE( test_generator )

BOOST_AUTO_TEST_CASE( writer_should_create_i_when_program_is_i ) {
  ModuleWriterMock writer = writeToMock("I");
  BOOST_CHECK( writer.didCreateI() );
}

BOOST_AUTO_TEST_SUITE_END()


struct NameNodeFixture {
  NameNodeFixture() { aNode = NameNodeFixture::factory.buildNode("A"); }
  ~NameNodeFixture() { factory.deleteNodes(); }

  NodeFactory factory;
  const Node *aNode;
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


void testParser(Tester& tester, const NodeFactory& factory) {
  { // Test accessors of apply node
    const Node *child1 = factory.buildNode("A");
    const Node *child2 = factory.buildNode("B");
    const Node *parent12 = factory.buildNode(*child1, *child2);
    const Node *parent21 = factory.buildNode(*child2, *child1);

    tester.verify(parent12->getName() == "(A B)",
                   "Expected name of node `A B to be (A B)");
    tester.verify(parent21->getName() == "(B A)",
                   "Expected name of node `B A to be (B A)");
    tester.verify(!parent12->isTerminal(),
                   "Expected node `A B not to be terminal");
    tester.verify(*parent12->getApplicator() == *child1,
                   "Expected the applicator of `A B to be A");
    tester.verify(*parent12->getInput() == *child2,
                   "Expected the applicator of `A B to be B");
    factory.deleteNodes();
  }
  
  { // Test inequality of name nodes and apply nodes
    const Node *childless = factory.buildNode("(A B)");
    const Node *childA = factory.buildNode("A");
    const Node *childB = factory.buildNode("B");
    const Node *parentAB = factory.buildNode(*childA, *childB);
    const Node *grandparentABC = factory.buildNode(*parentAB, *childless);
    const Node *grandparentCAB = factory.buildNode(*childless, *parentAB);

    tester.verify(*childless != *parentAB,
                   "Expected node `A B to be != to terminal node whose name is (A B)");
    tester.verify(*parentAB != *childless,
                   "Expected terminal node whose name is (A B) to be != to node `A B");
    tester.verify(*grandparentABC != *grandparentCAB,
                   "Expected Node::operator== to test equality of child nodes");
    factory.deleteNodes();
  }

  { // Test copy constructor
    const Node *child1 = factory.buildNode("A");
    const Node *child2 = factory.buildNode("B");
    const Node *original = factory.buildNode(*child1, *child2);
    const Node *copy = new Node(*original);
    factory.deleteNodes();

    tester.verify(copy->getName() == "(A B)",
                   "Expected copy constructor to deep copy name string");
    tester.verify(*copy->getApplicator() == *factory.buildNode("A"),
                   "Expected copy constructor to deep copy applicator");
    tester.verify(*copy->getInput() == *factory.buildNode("B"),
                   "Expected copy constructor to deep copy input");
    delete copy;
  }

  { // Test parsing empty program
    const Node *expected = factory.buildNode("I");
    tester.verifyParse("", expected);
    factory.deleteNodes();
  }
  
  { // Test parsing single node
    const Node *nodeA = factory.buildNode("A");
    const Node *nodeB = factory.buildNode("B");
    const Node *nodeNode = factory.buildNode("Node");
    tester.verifyParse("A", nodeA);
    tester.verifyParse("B", nodeB);
    tester.verifyParse("Node", nodeNode);
    factory.deleteNodes();
  }
  
  { // Test parsing two nodes
    const Node *nodeA = factory.buildNode("Aa");
    const Node *nodeB = factory.buildNode("B");
    const Node *nodeAB = factory.buildNode(*nodeA, *nodeB);
    const Node *nodeBA = factory.buildNode(*nodeB, *nodeA);
    tester.verifyParse("Aa B", nodeAB);
    tester.verifyParse("B Aa", nodeBA);
    factory.deleteNodes();
  }
  
  { // Test parsing three nodes sans parentheses
    const Node *nodeA = factory.buildNode("Aa");
    const Node *nodeB = factory.buildNode("Bb");
    const Node *nodeAB = factory.buildNode(*nodeA, *nodeB);
    const Node *nodeC = factory.buildNode("Cc");
    const Node *nodeABC = factory.buildNode(*nodeAB, *nodeC);
    tester.verifyParse("Aa Bb Cc", nodeABC);
    factory.deleteNodes();
  }

  { // Test parsing three nodes with first two nodes grouped
    const Node *nodeA = factory.buildNode("Aa");
    const Node *nodeB = factory.buildNode("Bb");
    const Node *nodeAB = factory.buildNode(*nodeA, *nodeB);
    const Node *nodeC = factory.buildNode("Cc");
    const Node *nodeABC = factory.buildNode(*nodeAB, *nodeC);
    tester.verifyParse("(Aa Bb) Cc", nodeABC);
    factory.deleteNodes();
  }

  { // Test parsing three nodes with last two nodes grouped
    const Node *nodeA = factory.buildNode("Aa");
    const Node *nodeB = factory.buildNode("Bb");
    const Node *nodeC = factory.buildNode("Cc");
    const Node *nodeBC = factory.buildNode(*nodeB, *nodeC);
    const Node *nodeABC = factory.buildNode(*nodeA, *nodeBC);
    tester.verifyParse("Aa (Bb Cc)", nodeABC);
    factory.deleteNodes();
  }

  { // Test parsing four nodes with pairs grouped
    const Node *nodeA = factory.buildNode("Aa");
    const Node *nodeB = factory.buildNode("Bb");
    const Node *nodeC = factory.buildNode("Cc");
    const Node *nodeD = factory.buildNode("Dd");

    const Node *nodeAB = factory.buildNode(*nodeA, *nodeB);
    const Node *nodeCD = factory.buildNode(*nodeC, *nodeD);
    
    const Node *nodeABCD = factory.buildNode(*nodeAB, *nodeCD);
    tester.verifyParse("(Aa Bb) (Cc Dd)", nodeABCD);
    factory.deleteNodes();
  }

  { // Test parsing with unusual whitespace and parens
    const Node *nodeI = factory.buildNode("I");
    const Node *nodeA = factory.buildNode("A");
    const Node *nodeB = factory.buildNode("B");
    const Node *nodeAB = factory.buildNode(*nodeA, *nodeB);
    tester.verifyParse("()", nodeI);
    tester.verifyParse(" ", nodeI);
    tester.verifyParse(" A", nodeA);
    tester.verifyParse("A ", nodeA);
    tester.verifyParse("\tA", nodeA);
    tester.verifyParse("( A )", nodeA);
    tester.verifyParse("A\tB", nodeAB);
    tester.verifyParse("A ((B))", nodeAB);
    tester.verifyParse("(A)(B)", nodeAB);
    tester.verifyParse("A(B)", nodeAB);
    tester.verifyParse("(A)B", nodeAB);
    tester.verifyParse("(\t A  )\t\t( ((B)  )\t)", nodeAB);
    factory.deleteNodes();
  }

  { // Test parsing erroneous programs
    tester.verifyParseError("(");
    tester.verifyParseError(") (");
  }
}
