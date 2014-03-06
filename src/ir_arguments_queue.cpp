#include "llvm/IR/IRBuilder.h"

#include "ir_arguments_queue.hpp"

Type* IRArity::getType(LLVMContext& context) {
  return Type::getInt32Ty(context);
}

Type* IRArgumentsQueue::getPointerType(LLVMContext& context) {
  return getType(context)->getPointerTo();
}


IRArgumentsQueue::IRArgumentsQueue(Function* malloc, BasicBlock* block) {
  LLVMContext *context = &block->getContext();
  Value* argsAllocSize = getSize(*context);
  IRBuilder<> builder(block);
  Value* mallocResult = builder.CreateCall(malloc, argsAllocSize);
  irStruct = builder.CreateBitCast(mallocResult, getPointerType(*context));
}

IRArgumentsQueue::~IRArgumentsQueue() {}

void IRArgumentsQueue::setLength(int length, BasicBlock* block) {
  std::vector<Value*> idxList(2);
  idxList[0] = ConstantInt::get(Type::getInt64Ty(block->getContext()), 0);
  idxList[1] = ConstantInt::get(Type::getInt32Ty(block->getContext()), 0);
  IRBuilder<> builder(block);
  Value* argsLengthPtr = builder.CreateGEP(irStruct, idxList);  
  Value* argsLengthValue = ConstantInt::get(getLengthType(), 0);
  builder.CreateStore(argsLengthValue, argsLengthPtr);
}

Value* IRArgumentsQueue::getValue() {
  return irStruct;
}

Value* IRArgumentsQueue::getSize(LLVMContext& context) {
  return ConstantExpr::getSizeOf(getType(context));
}

Type* IRArgumentsQueue::getType(LLVMContext& context) {
  return StructType::get(IRArity::getType(context), NULL);
}

Type* IRArgumentsQueue::getType() {
  return irStruct->getType()->getPointerElementType();
}

Type* IRArgumentsQueue::getLengthType() {
  return getType()->getStructElementType(0);
}
