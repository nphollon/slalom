#ifndef H_IR_ARGUMENTS_QUEUE
#define H_IR_ARGUMENTS_QUEUE

#include "llvm/IR/Module.h"
#include "llvm/IR/LLVMContext.h"

#include "ir_queue_node.hpp"

using namespace llvm;

class IRQueueNode;

class IRArgumentsQueue {
public:
  IRArgumentsQueue(Value*);
  ~IRArgumentsQueue();

  void setLength(int, BasicBlock*);
  void setHead(IRQueueNode*, BasicBlock*);
  void setTail(IRQueueNode*, BasicBlock*);
  Value* getValue();

private:
  Value* irStruct;

  Type* getType();
  Type* getLengthType();
  Value* getElementPointer(int, BasicBlock*);
};

#endif
