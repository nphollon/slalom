#ifndef H_IR_TYPE_MANAGER
#define H_IR_TYPE_MANAGER

#include "llvm/IR/Module.h"
#include "ir_queue_node.hpp"

using namespace llvm;

class IRTypeManager {
public:
  IRTypeManager(Module*);

  IRQueueNode* buildQueueNode(BasicBlock*);
  IRArgumentsQueue* buildArgumentsQueue(BasicBlock*);
  IRSlalomFunction* buildSlalomFunction(BasicBlock*);
private:
  Module *module;
  Function *malloc;

  Function* declareMalloc();
};

#endif
