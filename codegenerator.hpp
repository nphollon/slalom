#ifndef H_CODE_GENERATOR
#define H_CODE_GENERATOR

#include "llvm/IR/Module.h"

using namespace llvm;

class CodeGenerator {
 private:
  Module* module;
 public:
  CodeGenerator(Module*);
  ~CodeGenerator();
  void generate() const;
  Module* getModule() const;
};

#endif
