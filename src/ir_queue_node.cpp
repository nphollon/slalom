#include "llvm/IR/IRBuilder.h"

#include "ir_queue_node.hpp"

const std::string IRQueueNode::NAME = "queue_node";

StructType* IRQueueNode::type = NULL;

Type* IRQueueNode::getPointerType(LLVMContext& context) {
  if (!type) {
    type = StructType::create(context, NAME);
  }
  return type->getPointerTo();
}

IRQueueNode::IRQueueNode(Function* malloc, BasicBlock* block) {
  LLVMContext *context = &block->getContext();
  IRBuilder<> builder(block);
  Value* allocSize = getSize(*context);
  Value* mallocResult = builder.CreateCall(malloc, allocSize);
  irStruct = builder.CreateBitCast(mallocResult, getPointerType(*context));
}

void IRQueueNode::setData(IRSlalomFunction* data, BasicBlock* block) {
  Value* dataValue = data->getValue();
  IRBuilder<> builder(block);
  builder.CreateStore(dataValue, getDataPointer(block));
}

Value* IRQueueNode::getValue() {
  return irStruct;
}

Value* IRQueueNode::getSize(LLVMContext& context) {
  return ConstantExpr::getSizeOf(getType(context));
}

Type* IRQueueNode::getType(LLVMContext& context) {
  if (type->isOpaque()) {
    Type* dataType = IRSlalomFunction::getPointerType(context);
    type->setBody(dataType, NULL, NULL); // need 2 nulls to disambiguate function call
  }
  return type;
}

Value* IRQueueNode::getDataPointer(BasicBlock* block) {
  return getElementPointer(0, block);
}

Value* IRQueueNode::getElementPointer(int i, BasicBlock* block) {
  IRBuilder<> builder(block);
  std::vector<Value*> idxList(2);
  idxList[0] = ConstantInt::get(Type::getInt64Ty(block->getContext()), 0);
  idxList[1] = ConstantInt::get(Type::getInt32Ty(block->getContext()), i);
  return builder.CreateGEP(irStruct, idxList);
}
