#ifndef H_QUEUE_NODE
#define H_QUEUE_NODE

#include "llvm/IR/Module.h"

#include "ir_slalom_function.hpp"

using namespace llvm;

class IRSlalomFunction;

class IRQueueNode {
public:
  IRQueueNode(Value*);
  
  void setData(IRSlalomFunction*, BasicBlock*);
  Value* getValue();

private:
  Value* irStruct;

  Value* getDataPointer(BasicBlock*);
  Value* getElementPointer(int i, BasicBlock*);
};

#endif
