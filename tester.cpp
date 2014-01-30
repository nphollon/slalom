#include "tester.hpp"

#include <iostream>
#include "parse.hpp"

string getProgramText(const string&);

Tester::Tester() {}
Tester::~Tester() {}

void Tester::verify(const bool& testExpression, const string& errorMessage) {
  if (!testExpression) {
    cout << "Failure. " << errorMessage << endl;
    errorCount++;
  }
}

void Tester::verifyParse(const string& program, const Node *expectedParseTree) {
  const Node *actualParseTree = parse(program);

  const string errorMessage = "Unexpected parse tree for program.\n" +
    getProgramText(program) +
    "\nExpected parse tree: " + expectedParseTree->getName() +
    "\nActual parse tree: " + actualParseTree->getName();

  verify(*actualParseTree == *expectedParseTree, errorMessage);
  delete actualParseTree;
}

void Tester::verifyParseError(const string& program) {
  verify(parse(program) == NULL, "Expected parse error.\n" + getProgramText(program));
}

void Tester::printReport() const {
  if (errorCount == 0) {
    cout << "All tests passed." << endl;
  } else {
    cout << errorCount << " tests failed." << endl;
  }
}

string getProgramText(const string& program) {
  return "Program text: " + (program.empty() ? "<empty>" : program);
}
