#ifndef H_TESTER
#define H_TESTER

#include <string>
#include "node.hpp"

using namespace std;

class Tester {
private:
  int errorCount;
public:
  Tester();
  ~Tester();
  void verify(const bool&, const string&);
  void verifyParse(const string&, const Node*);
  void verifyParseError(const string& program);
  void printReport() const;
};

#endif
