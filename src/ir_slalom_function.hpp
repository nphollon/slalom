#ifndef H_IR_SLALOM_FUNCTION
#define H_IR_SLALOM_FUNCTION

#include "llvm/IR/Module.h"
#include "llvm/IR/LLVMContext.h"

#include "module_writer.hpp"
#include "ir_arguments_queue.hpp"

using namespace llvm;

class IRArgumentsQueue;

class IRSlalomFunction : public SlalomFunction {
public:
  static PointerType* getPointerType(LLVMContext&);
  static IRSlalomFunction* getNull(LLVMContext&);

  IRSlalomFunction(Function*, BasicBlock*);
  ~IRSlalomFunction();

  Value* getValue();
  void setArity(int, BasicBlock*);
  void setName(const std::string&, BasicBlock*);
  void setArguments(IRArgumentsQueue*, BasicBlock*);
  void setReturn(BasicBlock*);
private:
  static Value* getSize(LLVMContext&);
  static Type* getType(LLVMContext&);

  IRSlalomFunction(LLVMContext&);

  Value* irStruct;

  Type* getType();
  Type* getPointerType();
  Type* getArityType();
  Type* getNameType();
  Value* getArityPointer(BasicBlock*);
  Value* getNamePointer(BasicBlock*);
  Value* getElementPointer(int, BasicBlock*);
};

#endif
