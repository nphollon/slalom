#ifndef H_IR_SLALOM_FUNCTION
#define H_IR_SLALOM_FUNCTION

#include "llvm/IR/Module.h"
#include "llvm/IR/LLVMContext.h"

#include "../module_writer.hpp"
#include "ir_arguments_queue.hpp"

using namespace llvm;

class IRArgumentsQueue;

class IRSlalomFunction : public SlalomFunction {
public:
  IRSlalomFunction(Value*);
  ~IRSlalomFunction();

  Value* getValue();
  void setArity(int, BasicBlock*);
  void setName(const std::string&, BasicBlock*);
  void setArguments(IRArgumentsQueue*, BasicBlock*);
  void setReturn(BasicBlock*);
private:
  Value* irStruct;

  Type* getType();
  Type* getPointerType();
  Type* getArityType();
  Type* getNameType();
  Value* getElementPointer(int, BasicBlock*);
};

#endif
