#ifndef H_QUEUE_NODE
#define H_QUEUE_NODE

#include "llvm/IR/Module.h"

#include "ir_slalom_function.hpp"

using namespace llvm;

class IRSlalomFunction;

class IRQueueNode {
public:
  static Type* getPointerType(LLVMContext&);

  IRQueueNode(Function*, BasicBlock*);
  
  void setData(IRSlalomFunction*, BasicBlock*);
  Value* getValue();

private:
  static const std::string NAME;

  static StructType* type;
  Value* irStruct;

  static Value* getSize(LLVMContext&);
  static Type* getType(LLVMContext&);

  Value* getDataPointer(BasicBlock*);
  Value* getElementPointer(int i, BasicBlock*);
};

#endif
