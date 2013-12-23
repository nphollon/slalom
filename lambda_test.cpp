#include <string>
#include <iostream>
#include <vector>
#include "node.hpp"

using namespace std;

class Tester {
private:
  int errorCount;

public:
  Tester() {}
  ~Tester() {}

  void assert(const bool testExpression, const string errorMessage) {
    if (!testExpression) {
      cout << "Failure. " << errorMessage << endl;
      errorCount++;
    }
  }
  /*
  void assertParse(const string& program, const Node *expectedParseTree) {
    const Node *actualParseTree = parse(program);
    const string errorMessage = "Unexpected parse tree for program: " + program;
    assert(*actualParseTree == *expectedParseTree, errorMessage);
    delete actualParseTree;
  }
  */
  void printReport() const {
    if (errorCount == 0) {
      cout << "All tests passed." << endl;
    } else {
      cout << errorCount << " tests failed." << endl;
    }
  }
};


class NodeFactory {
private:
  vector<Node *> *nodes;
public:
  NodeFactory() {
    nodes = new vector<Node *>();
  }

  const Node* buildNode(const string& name) const {
    nodes->push_back(new Node(name));
    return nodes->back();
  }

  const Node* buildNode(const Node& applicator, const Node& input) const {
    nodes->push_back(new Node(applicator, input));
    return nodes->back();
  }

  void deleteNodes() const {
    while (!nodes->empty()) {
      delete nodes->back();
      nodes->pop_back();
    }
  }

  ~NodeFactory() {
    deleteNodes();
    delete nodes;
    }
};


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
    tester->assert(terminalNode->getName() == "A", "Expected name of node A to be immutable");
    factory->deleteNodes();
  }

  { // Test accessors of apply node
    const Node *child1 = factory->buildNode("A");
    const Node *child2 = factory->buildNode("B");
    const Node *parent = factory->buildNode(*child1, *child2);

    tester->assert(parent->getName() == "(A B)", "Expected name of node `A B to be (A B)");
    factory->deleteNodes();
  }
  /*
  { // Test equality of ApplyNode
    const Node *child1 = factory->buildNode("A");
    const Node *child2 = factory->buildNode("B");

    const Node *aParent = factory->buildNode(child1, child2);
    const Node *identicalParent = factory->buildNode(child1, child2);
    const Node *swapParent = factory->buildNode(child2, child1);

    tester->assert(*aParent == *identicalParent, "Expected (A B) == (A B) to be true");
    tester->assert(*aParent != *swapParent, "Expected (A B) != (B A) to be true");
  }

  { // Test parsing empty program
    const Node *expected = factory->buildNode("I");
    tester->assertParse("", expected);
    factory->deleteNodes();
  }
  */
  tester->printReport();

  delete factory;
  delete tester;
  return 0;
}
