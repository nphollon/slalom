#ifndef H_IR_TYPE_MANAGER
#define H_IR_TYPE_MANAGER

#include "llvm/IR/Module.h"
#include "ir_queue_node.hpp"

using namespace llvm;

class IRTypeManager {
public:
  IRTypeManager(Module*);

  void describeFactoryFunction(const std::string&, int, const std::string&);
private:
  Module *module;
  Function *malloc;
  StructType *queueNodeType;
  StructType *queueType;
  StructType *functionType;

  IRQueueNode* buildQueueNode(BasicBlock*);
  IRQueueNode* nullQueueNode();
  IRArgumentsQueue* buildArgumentsQueue(BasicBlock*);
  IRSlalomFunction* buildSlalomFunction(int, const std::string&, BasicBlock*);
  IRSlalomFunction* nullSlalomFunction();
  void declareMalloc();
  void describeTypes();
  Value* allocate(Type*, BasicBlock*);
  Value* nullPointer(Type*);
};

#endif
