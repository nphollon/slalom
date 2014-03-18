#include "ir_type_manager.hpp"

IRTypeManager::IRTypeManager(Module* newModule) {
  module = newModule;
  malloc = declareMalloc();
}

Function* IRTypeManager::getMalloc() {
  return malloc;
}

Function* IRTypeManager::declareMalloc() {
  Type* intPtrTy = Type::getInt8PtrTy(module->getContext());
  Type* allocTy = Type::getInt64Ty(module->getContext());
  Constant* mallocC = module->getOrInsertFunction("malloc", intPtrTy, allocTy, NULL);
  return cast<Function>(mallocC);
}

