#include "parse.hpp"
#include <iostream>
#include "strutil.hpp"

// Helper function prototypes
const Node* constructParseTree(const string& program);
bool validate(const string&);
void validateNoNameAt(const string&, const int&);
vector<string> splitAtLastToken(const string&);
string trim(const string&);
bool isWrapped(const string&);
int findLastOpenParen(const string&);

const string WHITESPACE = "\t ";
const string NON_NAME_CHARS = WHITESPACE + "()";

const Node* parse(const string& program) {
  if (validate(program)) {
    return constructParseTree(program);
  }
  return NULL;
}

// Recursively construct parse tree
// Assumes valid program syntax
const Node* constructParseTree(const string& expression) {
  string trimmed = trim(expression);

  if (trimmed.empty()) {
    return new Node("I");
  }

  const vector<string> tokens = splitAtLastToken(trimmed);

  if (tokens.size() == 1) {
    return new Node(tokens[0]);
  }

  const Node *input = constructParseTree(tokens[1]);
  const Node *applicator = constructParseTree(tokens[0]);
  const Node *composed = new Node(*applicator, *input);
  delete applicator;
  delete input;
  return composed;
}

// Throws ParenthesesDoNotMatch if expression has mismatched parens
bool validate(const string& expression) {
  int nestLevel = 0;
  for (int i = 0; i < expression.length() && nestLevel <= 0; i++) {
    if (expression[i] == '(') {
      nestLevel--;
    } else if (expression[i] == ')') {
      nestLevel++;
    }
  }

  return nestLevel == 0;
}

// Identifies last token in expression, as delimited by whitespace or parens
// Splits expression at delimiter and returns 2 subexpressions
// Returns full expression if only one token exists
// -- Examples --
// "A B C" returns {"A B", "C"}
// "A (B C)" returns {"A", "(B C)"}
// "A" returns {"A"}
// "(A (B C))" returns {"(A (B C))"}
vector<string> splitAtLastToken(const string& expression) {
  int lastTokenPos = findLastOpenParen(expression);
  
  if (lastTokenPos == expression.length()) {
    lastTokenPos = expression.find_last_of(NON_NAME_CHARS) + 1;
  }

  string lastToken = expression.substr(lastTokenPos);

  vector<string> tokens;
  if (lastToken != expression) {
    string firstToken = expression.substr(0, lastTokenPos);
    tokens.push_back(firstToken);
  }
  tokens.push_back(lastToken);
  return tokens;
}

// Returns expression without leading/trailing whitespace and wrapping parens
string trim(const string& expression) {
  size_t firstCharPos = expression.find_first_not_of(WHITESPACE);

  if (expression.empty() || firstCharPos == string::npos) {
    return "";
  }

  size_t lastCharPos = expression.find_last_not_of(WHITESPACE);
  string trimmed = substrFromStart(expression, firstCharPos, lastCharPos);

  if (isWrapped(trimmed)) {
    return trim(substrFromEnds(trimmed, 1, 1));
  }

  return trimmed;
}

// Returns true if expression is wrapped in parens
bool isWrapped(const string& expression) {
  return (expression.length() > 0) && (findLastOpenParen(expression) == 0);
}

// If last character of expression is not ')',
//    return length of expression
// If last ')' has no matching '('
//    return -1
// Else
//    return position of '(' matching last ')'
int findLastOpenParen(const string& expression) {
  if (lastChar(expression) != ')') {
    return expression.length();
  }

  int nestLevel = 1;
  int i = expression.length() - 1;
  while (nestLevel > 0 && i > 0) {
    i--;
    if (expression[i] == '(') {
      nestLevel--;
    } else if (expression[i] == ')') {
      nestLevel++;
    }
  }
  return i;
}
