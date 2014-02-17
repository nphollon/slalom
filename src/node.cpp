#include "node.hpp"

const Node* Node::byName(const string& name) {
  return new Node(name, NULL, NULL);
}

const Node* Node::byChildren(const Node& applicator, const Node& input) {
  const string name = "(" + applicator.getName() + " " + input.getName() + ")";
  return new Node(name, Node::copy(applicator), Node::copy(input));
}

const Node* Node::parse(const string& program) {
  ProgramText pt(program);
  if (pt.isValid()) {
    return constructParseTree(pt);
  }
  return NULL;
}

bool Node::isTerminal() const {
  return applicator == NULL;
}

const string Node::getName() const {
  return *name;
}

const Node* Node::getApplicator() const {
  return applicator;
}

const Node* Node::getInput() const {
  return input;
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

Node::~Node() {
  delete name;
  if (!isTerminal()) {
    delete applicator;
    delete input;
  }
}

// Private functions

Node::Node(const string& n, const Node *app, const Node *inp) {
  name = new string(n);
  applicator = app;
  input = inp;
}

const Node* Node::copy(const Node& original) {
  if (original.isTerminal()) {
    return new Node(original.getName(), NULL, NULL);
  }
  return new Node(original.getName(),
                  Node::copy(*original.getApplicator()),
                  Node::copy(*original.getInput()));
}

// Recursively construct parse tree
// Assumes valid program syntax
const Node* Node::constructParseTree(const ProgramText& program) {
  ProgramText *trimmed = program.trim();

  if (trimmed->isEmpty()) {
    return Node::byName("I");
  }

  const vector<string> tokens = trimmed->splitAtLastToken();

  if (tokens.size() == 1) {
    return Node::byName(tokens[0]);
  }

  const Node *input = constructParseTree(ProgramText(tokens[1]));
  const Node *applicator = constructParseTree(ProgramText(tokens[0]));
  const Node *composed = Node::byChildren(*applicator, *input);
  delete applicator;
  delete input;
  return composed;
}

