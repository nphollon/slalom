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
  IRQueueNode* qn = new IRQueueNode(allocate(queueNodeType, block));
  qn->setData(nullSlalomFunction(), block);
  qn->setNext(nullQueueNode(), block);
  return qn;
}

IRQueueNode* IRTypeManager::nullQueueNode() {
  return new IRQueueNode(nullPointer(queueNodeType));
}

IRArgumentsQueue* IRTypeManager::buildArgumentsQueue(BasicBlock* block) {
  IRQueueNode* qn = buildQueueNode(block);
  IRArgumentsQueue* q = new IRArgumentsQueue(allocate(queueType, block));
  q->setLength(0, block);
  q->setHead(qn, block);
  q->setTail(qn, block);
  return q;
}

IRSlalomFunction* IRTypeManager::buildSlalomFunction(int arity, const std::string& combName,
  BasicBlock* block) {

  IRArgumentsQueue* q = buildArgumentsQueue(block);
  IRSlalomFunction* function = new IRSlalomFunction(allocate(functionType, block));
  function->setArity(arity, block);
  function->setName(combName, block);
  function->setArguments(q, block);
  return function;
}

IRSlalomFunction* IRTypeManager::nullSlalomFunction() {
  return new IRSlalomFunction(nullPointer(functionType));
}

void IRTypeManager::describeFactoryFunction(const std::string& factoryName, int arity,
  const std::string& combName) {

  Type* returnType = functionType->getPointerTo();
  Constant* functionAsConstant = module->getOrInsertFunction(factoryName, returnType, NULL);
  Function* function = cast<Function>(functionAsConstant);

  BasicBlock* block = BasicBlock::Create(module->getContext(), "entry", function);
  IRSlalomFunction* sfs = buildSlalomFunction(arity, combName, block);
  sfs->setReturn(block);
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
