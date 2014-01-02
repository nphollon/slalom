#include "node.hpp"
#include "parse.hpp"
#include "tester.hpp"

using namespace std;

int main() {
  Tester *tester = new Tester();
  const NodeFactory *factory = new NodeFactory();

  { // Test equality of name node
    const Node *aNode = factory->buildNode("A");
    const Node *identicalNode = factory->buildNode("A");
    const Node *differentNode = factory->buildNode("B");

    tester->assert(*aNode == *identicalNode, "Expected A == A to be true");
    tester->assert(*identicalNode == *aNode, "Expected == to be commutative");
    tester->assert(!(*aNode == *differentNode), "Expected A == B to be false");

    tester->assert(*aNode != *differentNode, "Expected A != B to be true");
    tester->assert(*differentNode != *aNode, "Expected != to be commutative");
    tester->assert(!(*aNode != *identicalNode), "Expected A != A to be false");

    factory->deleteNodes();
  }

  { // Test accessors of name node
    const Node *terminalNode = factory->buildNode("A");
    tester->assert(terminalNode->getName() == "A", "Expected name of node A to be A");
    tester->assert(terminalNode->isTerminal(), "Expected node A to be terminal");

    bool hasApplicator = true;
    try {
      terminalNode->getApplicator();
    } catch (NodeDoesNotExist e) {
      hasApplicator = false;
    }
    tester->assert(!hasApplicator, "Expected node A to have no applicator");

    bool hasInput = true;
    try {
      terminalNode->getInput();
    } catch (NodeDoesNotExist e) {
      hasInput = false;
    }
    tester->assert(!hasInput, "Expected node A to have no input");

    factory->deleteNodes();
  }

  { // Test mutability of name node
    const Node *terminalNode = factory->buildNode("A");
    string nodeName = terminalNode->getName();
    nodeName += "more stuff";
    tester->assert(terminalNode->getName() == "A",
                   "Expected name of node A to be immutable");
    factory->deleteNodes();
  }

  { // Test accessors of apply node
    const Node *child1 = factory->buildNode("A");
    const Node *child2 = factory->buildNode("B");
    const Node *parent12 = factory->buildNode(*child1, *child2);
    const Node *parent21 = factory->buildNode(*child2, *child1);

    tester->assert(parent12->getName() == "(A.B)",
                   "Expected name of node `A B to be (A.B)");
    tester->assert(parent21->getName() == "(B.A)",
                   "Expected name of node `B A to be (B.A)");
    tester->assert(!parent12->isTerminal(),
                   "Expected node `A B not to be terminal");
    tester->assert(parent12->getApplicator() == *child1,
                   "Expected the applicator of `A B to be A");
    tester->assert(parent12->getInput() == *child2,
                   "Expected the applicator of `A B to be B");
    factory->deleteNodes();
  }
  
  { // Test inequality of name nodes and apply nodes
    const Node *childless = factory->buildNode("(A B)");
    const Node *childA = factory->buildNode("A");
    const Node *childB = factory->buildNode("B");
    const Node *parentAB = factory->buildNode(*childA, *childB);
    const Node *grandparentABC = factory->buildNode(*parentAB, *childless);
    const Node *grandparentCAB = factory->buildNode(*childless, *parentAB);

    tester->assert(*childless != *parentAB,
                   "Expected node `A B to be != to terminal node whose name is (A.B)");
    tester->assert(*parentAB != *childless,
                   "Expected terminal node whose name is (A B) to be != to node `A B");
    tester->assert(*grandparentABC != *grandparentCAB,
                   "Expected Node::operator== to test equality of child nodes");
    factory->deleteNodes();
  }

  { // Test copy constructor
    const Node *child1 = factory->buildNode("A");
    const Node *child2 = factory->buildNode("B");
    const Node *original = factory->buildNode(*child1, *child2);
    const Node *copy = new Node(*original);
    factory->deleteNodes();

    tester->assert(copy->getName() == "(A.B)",
                   "Expected copy constructor to deep copy name string");
    tester->assert(copy->getApplicator() == *(factory->buildNode("A")),
                   "Expected copy constructor to deep copy applicator");
    tester->assert(copy->getInput() == *(factory->buildNode("B")),
                   "Expected copy constructor to deep copy input");
    delete copy;
  }

  { // Test parsing empty program
    const Node *expected = factory->buildNode("I");
    tester->assertParse("", expected);
    factory->deleteNodes();
  }
  
  { // Test parsing single node
    const Node *nodeA = factory->buildNode("A");
    const Node *nodeB = factory->buildNode("B");
    const Node *nodeNode = factory->buildNode("Node");
    tester->assertParse("A", nodeA);
    tester->assertParse("B", nodeB);
    tester->assertParse("Node", nodeNode);
    factory->deleteNodes();
  }
  
  { // Test parsing two nodes
    const Node *nodeA = factory->buildNode("Aa");
    const Node *nodeB = factory->buildNode("B");
    const Node *nodeAB = factory->buildNode(*nodeA, *nodeB);
    const Node *nodeBA = factory->buildNode(*nodeB, *nodeA);
    tester->assertParse("Aa B", nodeAB);
    tester->assertParse("B Aa", nodeBA);
    factory->deleteNodes();
  }
  
  { // Test parsing three nodes sans parentheses
    const Node *nodeA = factory->buildNode("Aa");
    const Node *nodeB = factory->buildNode("Bb");
    const Node *nodeAB = factory->buildNode(*nodeA, *nodeB);
    const Node *nodeC = factory->buildNode("Cc");
    const Node *nodeABC = factory->buildNode(*nodeAB, *nodeC);
    tester->assertParse("Aa Bb Cc", nodeABC);
    factory->deleteNodes();
  }

  { // Test parsing three nodes with first two nodes grouped
    const Node *nodeA = factory->buildNode("Aa");
    const Node *nodeB = factory->buildNode("Bb");
    const Node *nodeAB = factory->buildNode(*nodeA, *nodeB);
    const Node *nodeC = factory->buildNode("Cc");
    const Node *nodeABC = factory->buildNode(*nodeAB, *nodeC);
    tester->assertParse("(Aa Bb) Cc", nodeABC);
    factory->deleteNodes();
  }

  { // Test parsing three nodes with last two nodes grouped
    const Node *nodeA = factory->buildNode("Aa");
    const Node *nodeB = factory->buildNode("Bb");
    const Node *nodeC = factory->buildNode("Cc");
    const Node *nodeBC = factory->buildNode(*nodeB, *nodeC);
    const Node *nodeABC = factory->buildNode(*nodeA, *nodeBC);
    tester->assertParse("Aa (Bb Cc)", nodeABC);
    factory->deleteNodes();
  }

  { // Test parsing four nodes with pairs grouped
    const Node *nodeA = factory->buildNode("Aa");
    const Node *nodeB = factory->buildNode("Bb");
    const Node *nodeC = factory->buildNode("Cc");
    const Node *nodeD = factory->buildNode("Dd");

    const Node *nodeAB = factory->buildNode(*nodeA, *nodeB);
    const Node *nodeCD = factory->buildNode(*nodeC, *nodeD);
    
    const Node *nodeABCD = factory->buildNode(*nodeAB, *nodeCD);
    tester->assertParse("(Aa Bb) (Cc Dd)", nodeABCD);
    factory->deleteNodes();
  }

  { // Test parsing with unusual whitespace and parens
    const Node *nodeI = factory->buildNode("I");
    const Node *nodeA = factory->buildNode("A");
    const Node *nodeB = factory->buildNode("B");
    const Node *nodeAB = factory->buildNode(*nodeA, *nodeB);
    tester->assertParse("()", nodeI);
    tester->assertParse(" ", nodeI);
    tester->assertParse(" A", nodeA);
    tester->assertParse("A ", nodeA);
    tester->assertParse("\tA", nodeA);
    tester->assertParse("( A )", nodeA);
    tester->assertParse("A\tB", nodeAB);
    tester->assertParse("A ((B))", nodeAB);
    tester->assertParse("(A)(B)", nodeAB);
    tester->assertParse("A(B)", nodeAB);
    tester->assertParse("(A)B", nodeAB);
    tester->assertParse("(\t A  )\t\t( ((B)  )\t)", nodeAB);
    factory->deleteNodes();
  }

  { // Test parsing erroneous programs
    tester->assertParseError("(");
    tester->assertParseError(") (");
  }

  tester->printReport();

  delete factory;
  delete tester;
  return 0;
}
