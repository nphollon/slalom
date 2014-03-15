#ifndef H_IR_MODULE_WRITER
#define H_IR_MODULE_WRITER

#include "llvm/IR/Module.h"

#include "module_writer.hpp"

using namespace llvm;

class IRTypeManager {
public:
  IRTypeManager(Module*);

  Function* getMalloc();
private:
  Module *module;
  Function *malloc;

  Function* declareMalloc();
};

class IRModuleWriter : public ModuleWriter {
public:
  static ModuleWriter* createModuleWriter(Module*);
  
  SlalomFunction* createICombinator();
  SlalomFunction* createKCombinator();
  SlalomFunction* createSCombinator();
  SlalomFunction* createDerivedCombinator(const std::string&);
  SlalomFunction* createApplication(SlalomFunction*, SlalomFunction*);
private:
  Module *module;

  IRModuleWriter(Module*);
  
  void generateFramework();
  BasicBlock* openFactoryFunction(const std::string&, Module*);
};

#endif
