#include "ir_slalom_function.hpp"

#include "llvm/IR/IRBuilder.h"

IRSlalomFunction::IRSlalomFunction(Value* dataStruct) {
  irStruct = dataStruct;
}

IRSlalomFunction::~IRSlalomFunction() {}

Value* IRSlalomFunction::getValue() {
  return irStruct;
}

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

void IRSlalomFunction::setArguments(IRArgumentsQueue* args, BasicBlock* block) {
  Value* argsValue = args->getValue();
  Value* argsPtr = getElementPointer(2, block);
  IRBuilder<> builder(block);
  builder.CreateStore(argsValue, argsPtr);
}

void IRSlalomFunction::setReturn(BasicBlock* block) {
  IRBuilder<> builder(block);
  builder.CreateRet(irStruct);
}

Type* IRSlalomFunction::getType() {
  return getPointerType()->getPointerElementType();
}

Type* IRSlalomFunction::getPointerType() {
  return irStruct->getType();
}

Type* IRSlalomFunction::getArityType() {
  return getType()->getStructElementType(0);
}

Type* IRSlalomFunction::getNameType() {
  return getType()->getStructElementType(1);
}

Value* IRSlalomFunction::getArityPointer(BasicBlock* block) {
  return getElementPointer(0, block);
}

Value* IRSlalomFunction::getNamePointer(BasicBlock* block) {
  return getElementPointer(1, block);
}

Value* IRSlalomFunction::getElementPointer(int i, BasicBlock* block) {
  IRBuilder<> builder(block);
  std::vector<Value*> idxList(2);
  idxList[0] = ConstantInt::get(Type::getInt64Ty(block->getContext()), 0);
  idxList[1] = ConstantInt::get(Type::getInt32Ty(block->getContext()), i);
  return builder.CreateGEP(irStruct, idxList);
}
