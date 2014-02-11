#include <iostream>

#include "node.hpp"
#include "parse.hpp"

using namespace std;

const string PROMPT = ">> ";

void printInstructions();
void prompt();
void processLine(const string& line);
int end();

int main() {
  printInstructions();
  prompt();

  string line;
  while (getline(cin, line)) {
    processLine(line);
    prompt();
  }

  return end();
}

void processLine(const string& line) {
  const Node *parseTree = parse(line);
  if (parseTree) {
    cout << parseTree->getName() << endl;
    delete parseTree;
  } else {
    cout << "ERROR: Parentheses do not match" << endl;
  }
}

void printInstructions() {
  cout << "Press Ctl-D to quit" << endl;
}

void prompt() {
  cout << PROMPT;
}

int end() {
  cout << endl;
  return 0;
}
