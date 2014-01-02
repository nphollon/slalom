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
  void assert(const bool&, const string&);
  void assertParse(const string&, const Node*);
  void assertParseError(const string& program);
  void printReport() const;
};

#endif
