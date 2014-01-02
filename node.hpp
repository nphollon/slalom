#ifndef H_NODE
#define H_NODE

#include <string>
#include <vector>

using namespace std;

class Node {
private:
  const string* name;
  const Node* applicator;
  const Node* input;
public:
  Node(const string&);
  Node(const Node&, const Node&);
  Node(const Node&);
  ~Node();  

  bool isTerminal() const;
  string getName() const;
  const Node getApplicator() const;
  const Node getInput() const;

  bool operator==(const Node&) const;
  bool operator!=(const Node&) const;
};

class NodeDoesNotExist {};

class NodeFactory {
private:
  vector<Node *> *nodes;
public:
  NodeFactory();
  ~NodeFactory();
  const Node* buildNode(const string&) const;
  const Node* buildNode(const Node&, const Node&) const;
  void deleteNodes() const;
};

#endif
