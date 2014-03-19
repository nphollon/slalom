#include "llvm/IR/IRBuilder.h"

#include "ir_queue_node.hpp"

IRQueueNode::IRQueueNode(Value* dataStruct) {
  irStruct = dataStruct;
}

void IRQueueNode::setData(IRSlalomFunction* data, BasicBlock* block) {
  Value* dataValue = data->getValue();
  IRBuilder<> builder(block);
  builder.CreateStore(dataValue, getDataPointer(block));
}

Value* IRQueueNode::getValue() {
  return irStruct;
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
