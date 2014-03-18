#include "ir_type_manager.hpp"

IRTypeManager::IRTypeManager(Module* newModule) {
  module = newModule;
  malloc = declareMalloc();
}

Function* IRTypeManager::declareMalloc() {
  Type* intPtrTy = Type::getInt8PtrTy(module->getContext());
  Type* allocTy = Type::getInt64Ty(module->getContext());
  Constant* mallocC = module->getOrInsertFunction("malloc", intPtrTy, allocTy, NULL);
  return cast<Function>(mallocC);
}

IRQueueNode* IRTypeManager::buildQueueNode(BasicBlock* block) {
  return new IRQueueNode(malloc, block);
}

IRArgumentsQueue* IRTypeManager::buildArgumentsQueue(BasicBlock* block) {
  return new IRArgumentsQueue(malloc, block);
}

IRSlalomFunction* IRTypeManager::buildSlalomFunction(BasicBlock* block) {
  return new IRSlalomFunction(malloc, block);
}
