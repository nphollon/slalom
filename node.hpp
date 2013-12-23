#ifndef H_NODE
#define H_NODE

#include <string>

using namespace std;

class Node {
private:
  const string* name;
public:
  Node(const string&);
  Node(const Node&, const Node&);
  ~Node();  

  bool isTerminal() const;
  string getName() const;
  const Node* getApplicator() const;
  const Node* getInput() const;

  bool operator==(const Node&) const;
  bool operator!=(const Node&) const;
};

class NodeDoesNotExist {};

#endif
