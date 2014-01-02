#include "tester.hpp"

#include <iostream>
#include "parse.hpp"

Tester::Tester() {}
Tester::~Tester() {}

void Tester::assert(const bool testExpression, const string errorMessage) {
  if (!testExpression) {
    cout << "Failure. " << errorMessage << endl;
    errorCount++;
  }
}

void Tester::assertParse(const string& program, const Node *expectedParseTree) {
  const Node *actualParseTree = parse(program);

  const string errorMessage = "Unexpected parse tree for program.\nProgram text: " + 
    (program.empty() ? "<empty>" : program) + "\nExpected parse tree: " +
    expectedParseTree->getName() + "\nActual parse tree: " +
    actualParseTree->getName();

  assert(*actualParseTree == *expectedParseTree, errorMessage);
  delete actualParseTree;
}

void Tester::assertParseError(const string& program) {
  bool badExpression = false;
  try {
    parse(program);
  } catch (ParenthesesDoNotMatch e) {
    badExpression = true;
  }
  assert(badExpression, "Expected program '(A' to cause parse exception");
}

void Tester::printReport() const {
  if (errorCount == 0) {
    cout << "All tests passed." << endl;
  } else {
    cout << errorCount << " tests failed." << endl;
  }
}
