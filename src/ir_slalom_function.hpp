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
  void setName(const std::string&, BasicBlock*);
  void setReturn(BasicBlock*);
private:
  static Value* getSize(LLVMContext&);
  static Type* getType(LLVMContext&);

  Value* irStruct;

  Type* getType();
  Type* getPointerType();
  Type* getArityType();
  Type* getNameType();
  Value* getArityPointer(BasicBlock*);
  Value* getNamePointer(BasicBlock*);
};

#endif
