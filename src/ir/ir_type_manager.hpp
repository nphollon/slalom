#ifndef H_IR_TYPE_MANAGER
#define H_IR_TYPE_MANAGER

#include "llvm/IR/Module.h"

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

#endif
