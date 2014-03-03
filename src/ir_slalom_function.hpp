#ifndef H_IR_SLALOM_FUNCTION
#define H_IR_SLALOM_FUNCTION

#include "llvm/IR/Module.h"
#include "llvm/IR/LLVMContext.h"

#include "module_writer.hpp"

using namespace llvm;

class IRSlalomFunction : public SlalomFunction {
public:
  static Type* getPointerType(LLVMContext&);

  IRSlalomFunction(Function*, BasicBlock*);
  ~IRSlalomFunction();

  void setArity(int, BasicBlock*);
  void setReturn(BasicBlock*);
private:
  static Type* getType(LLVMContext&);
  static Value* getSize(LLVMContext&);

  Value* irStruct;

  Type* getType();
  Type* getArityType();
  Value* getArityPointer(BasicBlock*);
};

#endif
