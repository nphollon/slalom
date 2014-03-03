#include "ir_slalom_function.hpp"

#include "llvm/IR/IRBuilder.h"

Type* IRSlalomFunction::getPointerType(LLVMContext& context) {
  return getType(context)->getPointerTo();
}

IRSlalomFunction::IRSlalomFunction(Function* malloc, BasicBlock* block) {
  LLVMContext *context = &block->getContext();
  Value* allocSize = getSize(*context);
  IRBuilder<> builder(block);
  Value* mallocResult = builder.CreateCall(malloc, allocSize);
  irStruct = builder.CreateBitCast(mallocResult, getPointerType(*context));
}

IRSlalomFunction::~IRSlalomFunction() {}

void IRSlalomFunction::setArity(int arity, BasicBlock* block) {
  Value* arityValue = ConstantInt::get(getArityType(), arity);
  IRBuilder<> builder(block);
  builder.CreateStore(arityValue, getArityPointer(block));
}

void IRSlalomFunction::setReturn(BasicBlock* block) {
  IRBuilder<> builder(block);
  builder.CreateRet(irStruct);
}

Value* IRSlalomFunction::getSize(LLVMContext& context) {
  return ConstantExpr::getSizeOf(getType(context));
}

Type* IRSlalomFunction::getType(LLVMContext& context) {
  Type* arityTy = Type::getInt32Ty(context);
  return StructType::get(arityTy, NULL);
}

Type* IRSlalomFunction::getType() {
  return irStruct->getType();
}

Type* IRSlalomFunction::getArityType() {
  return getType()->getPointerElementType()->getStructElementType(0);
}

Value* IRSlalomFunction::getArityPointer(BasicBlock* block) {
  IRBuilder<> builder(block);
  Value* idxZero = ConstantInt::get(Type::getInt32Ty(block->getContext()), 0);
  std::vector<Value*> idxList(2, idxZero);
  return builder.CreateGEP(irStruct, idxList);
}

