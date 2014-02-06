#include "llvm/IR/Constants.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/ExecutionEngine/JIT.h"
#include "llvm/Support/TargetSelect.h"

#include "node.hpp"
#include "parse.hpp"
#include "tester.hpp"

void testParser(Tester*, const NodeFactory*);
void testGenerator(Tester*);

int main() {
  Tester *tester = new Tester();
  const NodeFactory *factory = new NodeFactory();

  testParser(tester, factory);
  testGenerator(tester);

  tester->printReport();

  delete factory;
  delete tester;
  return 0;
}

int (*tmpUnderTest())() {
    llvm::InitializeNativeTarget();
    llvm::Module *module = new llvm::Module("Slalom Test", llvm::getGlobalContext());

    llvm::Constant* c = module->getOrInsertFunction("add",
                                                    llvm::IntegerType::get(llvm::getGlobalContext(), 32),
                                                    NULL);
    llvm::Function* addIR = llvm::cast<llvm::Function>(c);
    llvm::Value* x = llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(32, 0));

    llvm::BasicBlock* block = llvm::BasicBlock::Create(llvm::getGlobalContext(), "entry", addIR);
    llvm::IRBuilder<> builder(block);
    builder.CreateRet(x);

    llvm::ExecutionEngine *engine = llvm::EngineBuilder(module).create();

    llvm::Function *lf = module->getFunction("add");
    void *underTest = engine->getPointerToFunction(lf);
    int (*add)() = (int (*)())(intptr_t) underTest;
    return add;
}

void testGenerator(Tester* tester) {
  { // Test running code generator
    int (*add)() = tmpUnderTest();

    int theAnswer = add();

    tester->verify(theAnswer == 0, "Expected 0.");
  }
}

void testParser(Tester* tester, const NodeFactory* factory) {
  { // Test equality of name node
    const Node *aNode = factory->buildNode("A");
    const Node *identicalNode = factory->buildNode("A");
    const Node *differentNode = factory->buildNode("B");

    tester->verify(*aNode == *identicalNode, "Expected A == A to be true");
    tester->verify(*identicalNode == *aNode, "Expected == to be commutative");
    tester->verify(!(*aNode == *differentNode), "Expected A == B to be false");

    tester->verify(*aNode != *differentNode, "Expected A != B to be true");
    tester->verify(*differentNode != *aNode, "Expected != to be commutative");
    tester->verify(!(*aNode != *identicalNode), "Expected A != A to be false");

    factory->deleteNodes();
  }

  { // Test accessors of name node
    const Node *terminalNode = factory->buildNode("A");
    tester->verify(terminalNode->getName() == "A", "Expected name of node A to be A");
    tester->verify(terminalNode->isTerminal(), "Expected node A to be terminal");
    tester->verify(terminalNode->getApplicator() == NULL,
                   "Expected node A to have no applicator");
    tester->verify(terminalNode->getInput() == NULL, "Expected node A to have no input");

    factory->deleteNodes();
  }

  { // Test mutability of name node
    const Node *terminalNode = factory->buildNode("A");
    string nodeName = terminalNode->getName();
    nodeName += "more stuff";
    tester->verify(terminalNode->getName() == "A",
                   "Expected name of node A to be immutable");
    factory->deleteNodes();
  }

  { // Test accessors of apply node
    const Node *child1 = factory->buildNode("A");
    const Node *child2 = factory->buildNode("B");
    const Node *parent12 = factory->buildNode(*child1, *child2);
    const Node *parent21 = factory->buildNode(*child2, *child1);

    tester->verify(parent12->getName() == "(A B)",
                   "Expected name of node `A B to be (A B)");
    tester->verify(parent21->getName() == "(B A)",
                   "Expected name of node `B A to be (B A)");
    tester->verify(!parent12->isTerminal(),
                   "Expected node `A B not to be terminal");
    tester->verify(*parent12->getApplicator() == *child1,
                   "Expected the applicator of `A B to be A");
    tester->verify(*parent12->getInput() == *child2,
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

    tester->verify(*childless != *parentAB,
                   "Expected node `A B to be != to terminal node whose name is (A B)");
    tester->verify(*parentAB != *childless,
                   "Expected terminal node whose name is (A B) to be != to node `A B");
    tester->verify(*grandparentABC != *grandparentCAB,
                   "Expected Node::operator== to test equality of child nodes");
    factory->deleteNodes();
  }

  { // Test copy constructor
    const Node *child1 = factory->buildNode("A");
    const Node *child2 = factory->buildNode("B");
    const Node *original = factory->buildNode(*child1, *child2);
    const Node *copy = new Node(*original);
    factory->deleteNodes();

    tester->verify(copy->getName() == "(A B)",
                   "Expected copy constructor to deep copy name string");
    tester->verify(*copy->getApplicator() == *factory->buildNode("A"),
                   "Expected copy constructor to deep copy applicator");
    tester->verify(*copy->getInput() == *factory->buildNode("B"),
                   "Expected copy constructor to deep copy input");
    delete copy;
  }

  { // Test parsing empty program
    const Node *expected = factory->buildNode("I");
    tester->verifyParse("", expected);
    factory->deleteNodes();
  }
  
  { // Test parsing single node
    const Node *nodeA = factory->buildNode("A");
    const Node *nodeB = factory->buildNode("B");
    const Node *nodeNode = factory->buildNode("Node");
    tester->verifyParse("A", nodeA);
    tester->verifyParse("B", nodeB);
    tester->verifyParse("Node", nodeNode);
    factory->deleteNodes();
  }
  
  { // Test parsing two nodes
    const Node *nodeA = factory->buildNode("Aa");
    const Node *nodeB = factory->buildNode("B");
    const Node *nodeAB = factory->buildNode(*nodeA, *nodeB);
    const Node *nodeBA = factory->buildNode(*nodeB, *nodeA);
    tester->verifyParse("Aa B", nodeAB);
    tester->verifyParse("B Aa", nodeBA);
    factory->deleteNodes();
  }
  
  { // Test parsing three nodes sans parentheses
    const Node *nodeA = factory->buildNode("Aa");
    const Node *nodeB = factory->buildNode("Bb");
    const Node *nodeAB = factory->buildNode(*nodeA, *nodeB);
    const Node *nodeC = factory->buildNode("Cc");
    const Node *nodeABC = factory->buildNode(*nodeAB, *nodeC);
    tester->verifyParse("Aa Bb Cc", nodeABC);
    factory->deleteNodes();
  }

  { // Test parsing three nodes with first two nodes grouped
    const Node *nodeA = factory->buildNode("Aa");
    const Node *nodeB = factory->buildNode("Bb");
    const Node *nodeAB = factory->buildNode(*nodeA, *nodeB);
    const Node *nodeC = factory->buildNode("Cc");
    const Node *nodeABC = factory->buildNode(*nodeAB, *nodeC);
    tester->verifyParse("(Aa Bb) Cc", nodeABC);
    factory->deleteNodes();
  }

  { // Test parsing three nodes with last two nodes grouped
    const Node *nodeA = factory->buildNode("Aa");
    const Node *nodeB = factory->buildNode("Bb");
    const Node *nodeC = factory->buildNode("Cc");
    const Node *nodeBC = factory->buildNode(*nodeB, *nodeC);
    const Node *nodeABC = factory->buildNode(*nodeA, *nodeBC);
    tester->verifyParse("Aa (Bb Cc)", nodeABC);
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
    tester->verifyParse("(Aa Bb) (Cc Dd)", nodeABCD);
    factory->deleteNodes();
  }

  { // Test parsing with unusual whitespace and parens
    const Node *nodeI = factory->buildNode("I");
    const Node *nodeA = factory->buildNode("A");
    const Node *nodeB = factory->buildNode("B");
    const Node *nodeAB = factory->buildNode(*nodeA, *nodeB);
    tester->verifyParse("()", nodeI);
    tester->verifyParse(" ", nodeI);
    tester->verifyParse(" A", nodeA);
    tester->verifyParse("A ", nodeA);
    tester->verifyParse("\tA", nodeA);
    tester->verifyParse("( A )", nodeA);
    tester->verifyParse("A\tB", nodeAB);
    tester->verifyParse("A ((B))", nodeAB);
    tester->verifyParse("(A)(B)", nodeAB);
    tester->verifyParse("A(B)", nodeAB);
    tester->verifyParse("(A)B", nodeAB);
    tester->verifyParse("(\t A  )\t\t( ((B)  )\t)", nodeAB);
    factory->deleteNodes();
  }

  { // Test parsing erroneous programs
    tester->verifyParseError("(");
    tester->verifyParseError(") (");
  }
}
