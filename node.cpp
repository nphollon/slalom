#include <string>
#include "node.hpp"

using namespace std;

Node::Node(const string& n) {
  name = new string(n);
}

Node::Node(const Node& app, const Node& inp) {
  name = new string("(A B)");
}

Node::~Node() {
  delete name;
}


bool Node::isTerminal() const {
  return true;
}

string Node::getName() const {
  return *name;
}

const Node* Node::getApplicator() const {
  throw NodeDoesNotExist();
}

const Node* Node::getInput() const {
  throw NodeDoesNotExist();
}


bool Node::operator==(const Node& n) const {
  return *name == n.getName();
}

bool Node::operator!=(const Node& n) const {
  return !(*this == n);
}
