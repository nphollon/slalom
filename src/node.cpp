#include "node.hpp"
#include "strutil.hpp"

const Node* Node::parse(const string& program) {
  if (validate(program)) {
    return constructParseTree(program);
  }
  return NULL;
}

Node* Node::byName(const string& name) {
  return new Node(name, NULL, NULL);
}

Node* Node::byChildren(const Node& applicator, const Node& input) {
  const string name = "(" + applicator.getName() + " " + input.getName() + ")";
  return new Node(name, Node::copy(applicator), Node::copy(input));
}

Node* Node::copy(const Node& original) {
  if (original.isTerminal()) {
    return new Node(original.getName(), NULL, NULL);
  }
  return new Node(original.getName(),
                  Node::copy(*original.getApplicator()),
                  Node::copy(*original.getInput()));
}

string Node::getName() const {
  return *name;
}

const Node* Node::getApplicator() const {
  return applicator;
}

const Node* Node::getInput() const {
  return input;
}

bool Node::isTerminal() const {
  return applicator == NULL;
}

bool Node::operator==(const Node& n) const {
  if (isTerminal()) {
    return n.isTerminal() && *name == n.getName();
  }

  return !n.isTerminal() &&
    *applicator == *n.getApplicator() &&
    *input == *n.getInput();
}

bool Node::operator!=(const Node& n) const {
  return !(*this == n);
}

std::ostream& operator<<(std::ostream& os, const Node& obj) {
  os << obj.getName();
  return os;
}

Node::Node(const string& n, const Node *app, const Node *inp) {
  name = new string(n);
  applicator = app;
  input = inp;
}

Node::~Node() {
  delete name;
  if (!isTerminal()) {
    delete applicator;
    delete input;
  }
}

// Private functions
const string Node::WHITESPACE = "\t ";
const string Node::NON_NAME_CHARS = WHITESPACE + "()";

bool Node::validate(const string& expression) {
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

// Recursively construct parse tree
// Assumes valid program syntax
const Node* Node::constructParseTree(const string& expression) {
  string trimmed = trim(expression);

  if (trimmed.empty()) {
    return Node::byName("I");
  }

  const vector<string> tokens = splitAtLastToken(trimmed);

  if (tokens.size() == 1) {
    return Node::byName(tokens[0]);
  }

  const Node *input = constructParseTree(tokens[1]);
  const Node *applicator = constructParseTree(tokens[0]);
  const Node *composed = Node::byChildren(*applicator, *input);
  delete applicator;
  delete input;
  return composed;
}

// Returns expression without leading/trailing whitespace and wrapping parens
string Node::trim(const string& expression) {
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

// Identifies last token in expression, as delimited by whitespace or parens
// Splits expression at delimiter and returns 2 subexpressions
// Returns full expression if only one token exists
// -- Examples --
// "A B C" returns {"A B", "C"}
// "A (B C)" returns {"A", "(B C)"}
// "A" returns {"A"}
// "(A (B C))" returns {"(A (B C))"}
vector<string> Node::splitAtLastToken(const string& expression) {
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

// Returns true if expression is wrapped in parens
bool Node::isWrapped(const string& expression) {
  return (expression.length() > 0) && (findLastOpenParen(expression) == 0);
}

// If last character of expression is not ')',
//    return length of expression
// If last ')' has no matching '('
//    return -1
// Else
//    return position of '(' matching last ')'
int Node::findLastOpenParen(const string& expression) {
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
