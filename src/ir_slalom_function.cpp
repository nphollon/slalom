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

void IRSlalomFunction::setName(const std::string& name, BasicBlock* block) {
  IRBuilder<> builder(block);
  Value* nameValue = builder.CreateGlobalStringPtr(name);
  builder.CreateStore(nameValue, getNamePointer(block));
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
  Type* nameTy = Type::getInt8PtrTy(context);
  return StructType::get(arityTy, nameTy, NULL);
}

Type* IRSlalomFunction::getType() {
  return getPointerType()->getPointerElementType();
}

Type* IRSlalomFunction::getPointerType() {
  return irStruct->getType();
}

Type* IRSlalomFunction::getArityType() {
  return getPointerType()->getPointerElementType()->getStructElementType(0);
}

Type* IRSlalomFunction::getNameType() {
  return getPointerType()->getPointerElementType()->getStructElementType(1);
}

Value* IRSlalomFunction::getArityPointer(BasicBlock* block) {
  IRBuilder<> builder(block);
  std::vector<Value*> idxList(2);
  idxList[0] = ConstantInt::get(Type::getInt64Ty(block->getContext()), 0);
  idxList[1] = ConstantInt::get(Type::getInt32Ty(block->getContext()), 0);
  return builder.CreateGEP(irStruct, idxList);
}

Value* IRSlalomFunction::getNamePointer(BasicBlock* block) {
  IRBuilder<> builder(block);
  std::vector<Value*> idxList(2);
  idxList[0] = ConstantInt::get(Type::getInt64Ty(block->getContext()), 0);
  idxList[1] = ConstantInt::get(Type::getInt32Ty(block->getContext()), 1);
  return builder.CreateGEP(irStruct, idxList);
}

