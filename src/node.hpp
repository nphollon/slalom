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

  static Node* copy(const Node& n);
  Node(const string&, const Node*, const Node*);

  static const string WHITESPACE;
  static const string NON_NAME_CHARS;

  static const Node* constructParseTree(const string&);
  // TODO: improve name
  static bool validate(const string&);
  static void validateNoNameAt(const string&, const int&);
  static vector<string> splitAtLastToken(const string&);
  static string trim(const string&);
  static bool isWrapped(const string&);
  static int findLastOpenParen(const string&);
public:
  // TODO: be consistent about consts
  static Node* byName(const string&);
  static Node* byChildren(const Node&, const Node&);
  const static Node* parse(const string&);
  ~Node();  

  bool isTerminal() const;
  string getName() const;
  const Node* getApplicator() const;
  const Node* getInput() const;

  bool operator==(const Node&) const;
  bool operator!=(const Node&) const;
};

std::ostream& operator<<(std::ostream& os, const Node& obj);

#endif
