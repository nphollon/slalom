#include "ir_type_manager.hpp"

#include "llvm/IR/IRBuilder.h"

IRTypeManager::IRTypeManager(Module* newModule) {
  module = newModule;
  declareMalloc();
  describeTypes();
}

void IRTypeManager::declareMalloc() {
  Type* intPtrTy = Type::getInt8PtrTy(module->getContext());
  Type* allocTy = Type::getInt64Ty(module->getContext());
  Constant* mallocC = module->getOrInsertFunction("malloc", intPtrTy, allocTy, NULL);
  malloc = cast<Function>(mallocC);
}

IRQueueNode* IRTypeManager::buildQueueNode(BasicBlock* block) {
  return new IRQueueNode(allocate(queueNodeType, block));
}

IRQueueNode* IRTypeManager::nullQueueNode() {
  return new IRQueueNode(nullPointer(queueNodeType));
}

IRArgumentsQueue* IRTypeManager::buildArgumentsQueue(BasicBlock* block) {
  return new IRArgumentsQueue(allocate(queueType, block));
}

IRSlalomFunction* IRTypeManager::buildSlalomFunction(BasicBlock* block) {
  return new IRSlalomFunction(allocate(functionType, block));
}

IRSlalomFunction* IRTypeManager::nullSlalomFunction() {
  return new IRSlalomFunction(nullPointer(functionType));
}

BasicBlock* IRTypeManager::openFactoryFunction(const std::string& name) {
  Type* returnType = functionType->getPointerTo();
  Constant* functionAsConstant = module->getOrInsertFunction(name, returnType, NULL);
  Function* function = cast<Function>(functionAsConstant);
  return BasicBlock::Create(module->getContext(), "entry", function);
}

Type* IRTypeManager::getFunctionPointerType() {
  return functionType->getPointerTo();
}

Value* IRTypeManager::allocate(Type* type, BasicBlock* block) {
  IRBuilder<> builder(block);
  Value* size = ConstantExpr::getSizeOf(type);
  Value* mallocResult = builder.CreateCall(malloc, size);
  return builder.CreateBitCast(mallocResult, type->getPointerTo());
}

Value* IRTypeManager::nullPointer(Type* type) {
  return ConstantPointerNull::get(type->getPointerTo());
}

void IRTypeManager::describeTypes() {
  queueNodeType = StructType::create(module->getContext(), "queue_node");
  queueType = StructType::create(module->getContext(), "arguments_queue");
  functionType = StructType::create(module->getContext(), "slalom_function");
  Type* arityType = Type::getInt32Ty(module->getContext());
  Type* nameType = Type::getInt8PtrTy(module->getContext());
  
  queueNodeType->setBody(functionType->getPointerTo(),
                         queueNodeType->getPointerTo(),
                         NULL, NULL); // need 2 nulls to disambiguate setBody()
  queueType->setBody(arityType,
                     queueNodeType->getPointerTo(),
                     queueNodeType->getPointerTo(),
                     NULL);
  functionType->setBody(arityType,
                        nameType,
                        queueType->getPointerTo(),
                        NULL);
}
