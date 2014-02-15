#ifndef H_NODE_FACTORY
#define H_NODE_FACTORY

#include "../src/node.hpp"

class NodeFactory {
private:
  vector<Node *> *nodes;
public:
  NodeFactory() {
    nodes = new vector<Node *>();
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

#endif
