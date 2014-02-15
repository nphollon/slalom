#ifndef H_NODE_FACTORY
#define H_NODE_FACTORY

#include "../src/node.hpp"
#include "../src/generator.hpp"

class NodeFactory {
private:
  vector<const Node *> *nodes;
public:
  NodeFactory() {
    nodes = new vector<const Node *>();
  }

  ~NodeFactory() {
    deleteNodes();
    delete nodes;
  }

  const Node* buildNode(const string& name) const {
    nodes->push_back(Node::byName(name));
    return nodes->back();
  }

  const Node* buildNode(const Node& applicator, const Node& input) const {
    nodes->push_back(Node::byChildren(applicator, input));
    return nodes->back();
  }

  void deleteNodes() const {
    while (!nodes->empty()) {
      delete nodes->back();
      nodes->pop_back();
    }
  }
};

class FunctionFactory {
private:
  vector<SlalomFunction *> *funcs;
public:
  FunctionFactory() {
    funcs = new vector<SlalomFunction*>();
  }

  ~FunctionFactory() {
    deleteFuncs();
    delete funcs;
  }

  SlalomFunction* buildFunction() {
    funcs->push_back(new SlalomFunction());
    return funcs->back();
  }

  void deleteFuncs() {
    while (!funcs->empty()) {
      delete funcs->back();
      funcs->pop_back();
    }
  }
};

#endif
