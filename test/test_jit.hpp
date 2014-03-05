#ifndef H_TEST_JIT
#define H_TEST_JIT

#include <string>

#include "llvm/IR/Module.h"
#include "llvm/ExecutionEngine/ExecutionEngine.h"
#include "llvm/ExecutionEngine/JIT.h"

#include "../src/module_writer.hpp"

struct SlalomFunctionStruct {
  int arity;
  char* name;
};

typedef SlalomFunctionStruct*(*FactoryFunction)();

class TestJIT {
private:
  llvm::Module *module;
  llvm::ExecutionEngine *engine;
  ModuleWriter *writer;

public:
  TestJIT();
  ~TestJIT();

  FactoryFunction getFactoryFunctionPointer(const std::string&);
  void dumpModule();
};

#endif
