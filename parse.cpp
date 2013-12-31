#include <iostream>
#include "parse.hpp"

char lastChar(const string& text) {
  return text.at(text.length() - 1);
}

char firstChar(const string& text) {
  return text.at(0);
}

// Identifies last token in expression, as delimited by space or parens
// Splits expression at delimiter and returns 2 subexpressions
// Returns full expression if only one token exists
// -- Examples --
// "A B C" returns {"A B", "C"}
// "A (B C)" returns {"A", "(B C)"}
// "A" returns {"A"}
// "(A (B C))" returns {"(A (B C))"}

vector<string> splitAtLastToken(const string& expression) {
  int lastTokenPos;
  if (lastChar(expression) == ')') {
    lastTokenPos = expression.rfind('(');
  } else {
    lastTokenPos = expression.rfind(' ') + 1;
  }

  int lastTokenLen = expression.length() - lastTokenPos;
  string lastToken = expression.substr(lastTokenPos, lastTokenLen);

  vector<string> tokens;
  if (lastToken != expression) {
    string firstToken = expression.substr(0, lastTokenPos - 1);
    tokens.push_back(firstToken);
  }
  tokens.push_back(lastToken);
  return tokens;
}

string trim(const string& expression) {
  if (firstChar(expression) == '(' && lastChar(expression) == ')') {
    return expression.substr(1, expression.length() - 2);
  }
  return expression;
}

const Node* parse(const string& program) {
  if (program.empty()) {
    return new Node("I");
  }

  const vector<string> tokens = splitAtLastToken(trim(program));

  if (tokens.size() == 1) {
    return new Node(tokens[0]);
  }

  const Node *input = parse(tokens[1]);
  const Node *applicator = parse(tokens[0]);
  const Node *composed = new Node(*applicator, *input);
  delete applicator;
  delete input;
  return composed;
}
