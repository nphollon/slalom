#ifndef H_TESTER
#define H_TESTER

#include <string>

#include "node.hpp"

class Tester {
private:
  int errorCount;
public:
  Tester();
  ~Tester();
  void verify(const bool&, const std::string&);
  void verifyParse(const std::string&, const Node*);
  void verifyParseError(const std::string& program);
  void printReport() const;
};

#endif
