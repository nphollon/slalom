#include "node.hpp"

Node::Node(const string& n) {
  applicator = NULL;
  input = NULL;
  name = new string(n);
}

Node::Node(const Node& app, const Node& inp) {
  applicator = new Node(app);
  input = new Node(inp);
  name = new string("(" + app.getName() + " " + inp.getName() + ")");
}

Node::Node(const Node& original) {
  if (original.isTerminal()) {
    applicator = NULL;
    input = NULL;
  } else {
    applicator = new Node(original.getApplicator());
    input = new Node(original.getInput());
  }
  name = new string(original.getName());
}

Node::~Node() {
  delete name;
  if (!isTerminal()) {
    delete applicator;
    delete input;
  }
}

bool Node::isTerminal() const {
  return applicator == NULL;
}

string Node::getName() const {
  return *name;
}

const Node Node::getApplicator() const {
  if (isTerminal()) {
    throw NodeDoesNotExist();
  }
  return *applicator;
}

const Node Node::getInput() const {
  if (isTerminal()) {
    throw NodeDoesNotExist();
  }
  return *input;
}

bool Node::operator==(const Node& n) const {
  if (isTerminal()) {
    return n.isTerminal() && *name == n.getName();
  }

  return !n.isTerminal() &&
    *applicator == n.getApplicator() &&
    *input == n.getInput();
}

bool Node::operator!=(const Node& n) const {
  return !(*this == n);
}



NodeFactory::NodeFactory() {
  nodes = new vector<Node *>();
}

NodeFactory::~NodeFactory() {
  deleteNodes();
  delete nodes;
}

const Node* NodeFactory::buildNode(const string& name) const {
  nodes->push_back(new Node(name));
  return nodes->back();
}

const Node* NodeFactory::buildNode(const Node& applicator, const Node& input) const {
  nodes->push_back(new Node(applicator, input));
  return nodes->back();
}

void NodeFactory::deleteNodes() const {
  while (!nodes->empty()) {
    delete nodes->back();
    nodes->pop_back();
  }
}
