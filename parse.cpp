#include "parse.hpp"

#include "strutil.hpp"

// Helper function prototypes
const Node* constructParseTree(const string& program);
void validate(const string&);
void validateWhitespaceAt(const string&, const int&);
vector<string> splitAtLastToken(const string&);
string trim(const string&);
bool isWrapped(const string&);
int findLastOpenParen(const string&);

const string WHITESPACE = "\t ";

const Node* parse(const string& program) {
  validate(program);
  return constructParseTree(program);
}

const Node* constructParseTree(const string& program) {
  if (program.empty()) {
    return new Node("I");
  }

  const vector<string> tokens = splitAtLastToken(trim(program));

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

void validate(const string& expression) {
  int nestLevel = 0;
  for (int i = 0; i < expression.length() && nestLevel <= 0; i++) {
    if (expression[i] == '(') {
      validateWhitespaceAt(expression, i-1);
      nestLevel--;
    } else if (expression[i] == ')') {
      validateWhitespaceAt(expression, i+1);
      nestLevel++;
    }
  }

  if (nestLevel != 0) {
    throw ParenthesesDoNotMatch();
  }
}

void validateWhitespaceAt(const string& expression, const int& pos) {
  if (pos < 0 || pos >= expression.length()) return;
  if ((WHITESPACE + "()").find(expression[pos]) == string::npos) {
    throw MissingWhitespace();
  }
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
  int lastTokenPos = findLastOpenParen(expression);
  
  if (lastTokenPos == expression.length()) {
    lastTokenPos = expression.find_last_of(WHITESPACE) + 1;
  }

  string lastToken = substrFromEnds(expression, lastTokenPos, 0);

  vector<string> tokens;
  if (lastToken != expression) {
    string firstToken = expression.substr(0, lastTokenPos - 1);
    tokens.push_back(firstToken);
  }
  tokens.push_back(lastToken);
  return tokens;
}

string trim(const string& expression) {
  string trimmed = string(expression);

  size_t firstCharPos = trimmed.find_first_not_of(WHITESPACE);
  size_t lastCharPos = trimmed.find_last_not_of(WHITESPACE);
  trimmed = substrFromStart(trimmed, firstCharPos, lastCharPos);

  if (isWrapped(trimmed)) {
    trimmed = trim(substrFromEnds(trimmed, 1, 1));
  }

  return trimmed;
}

bool isWrapped(const string& expression) {
  return findLastOpenParen(expression) == 0;
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
