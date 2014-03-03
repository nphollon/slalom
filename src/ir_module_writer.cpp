#include "ir_module_writer.hpp"

#include "llvm/Support/Casting.h"

#include <vector>

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

void IRSlalomFunction::setReturn(BasicBlock* block) {
  IRBuilder<> builder(block);
  builder.CreateRet(irStruct);
}

Value* IRSlalomFunction::getSize(LLVMContext& context) {
  return ConstantExpr::getSizeOf(getType(context));
}

Type* IRSlalomFunction::getType(LLVMContext& context) {
  Type* arityTy = Type::getInt32Ty(context);
  return StructType::get(arityTy, NULL);
}

Type* IRSlalomFunction::getType() {
  return irStruct->getType();
}

Type* IRSlalomFunction::getArityType() {
  return getType()->getPointerElementType()->getStructElementType(0);
}

Value* IRSlalomFunction::getArityPointer(BasicBlock* block) {
  IRBuilder<> builder(block);
  Value* idxZero = ConstantInt::get(Type::getInt32Ty(block->getContext()), 0);
  std::vector<Value*> idxList(2, idxZero);
  return builder.CreateGEP(irStruct, idxList);
}

ModuleWriter* IRModuleWriter::createModuleWriter(Module *module) {
  IRModuleWriter* writer = new IRModuleWriter(module);
  writer->generateFramework();
  return writer;
}

IRModuleWriter::IRModuleWriter(Module *m) : module(m) {
}

SlalomFunction* IRModuleWriter::createICombinator() {
  return NULL;
}

SlalomFunction* IRModuleWriter::createKCombinator() {
  return NULL;
}

SlalomFunction* IRModuleWriter::createSCombinator() {
  return NULL;
}

SlalomFunction* IRModuleWriter::createDerivedCombinator(const std::string&) {
  return NULL;
}

SlalomFunction* IRModuleWriter::createApplication(SlalomFunction*, SlalomFunction*) {
  return NULL;
}

Function* IRModuleWriter::declareMalloc(Module* module) {
  Type* intPtrTy = Type::getInt8PtrTy(module->getContext());
  Type* allocTy = Type::getInt64Ty(module->getContext());
  Constant* mallocC = module->getOrInsertFunction("malloc", intPtrTy, allocTy, NULL);
  return cast<Function>(mallocC);
}

BasicBlock* IRModuleWriter::openFactoryFunction(const std::string& name, Module* module) {
  Type* retTy = IRSlalomFunction::getPointerType(module->getContext());
  Constant* functionAsConstant = module->getOrInsertFunction(name, retTy, NULL);
  Function* function = cast<Function>(functionAsConstant);
  return BasicBlock::Create(module->getContext(), "entry", function);
}

void IRModuleWriter::generateFramework() {
  Function* malloc = declareMalloc(module);
  BasicBlock* block = openFactoryFunction("createICombinator", module);
  IRSlalomFunction* sfs = new IRSlalomFunction(malloc, block);
  sfs->setArity(1, block);
  sfs->setReturn(block);

  module->dump();
}
