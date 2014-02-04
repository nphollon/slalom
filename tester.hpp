#ifndef H_TESTER
#define H_TESTER

#include <string>

#include "llvm/IR/Module.h"
#include "llvm/ExecutionEngine/ExecutionEngine.h"

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

class TestJIT {
private:
  llvm::Module* module;
  llvm::ExecutionEngine* engine;
public:
  TestJIT();
  ~TestJIT();
  void* getFunction(const std::string&) const;
};

#endif
