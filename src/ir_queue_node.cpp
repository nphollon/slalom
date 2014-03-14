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
