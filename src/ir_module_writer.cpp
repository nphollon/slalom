#include "ir_module_writer.hpp"

#include "llvm/Support/Casting.h"

#include <vector>

Type* IRSlalomFunction::getType(LLVMContext& context) {
  Type* arityTy = Type::getInt32Ty(context);
  return StructType::get(arityTy, NULL);
}

Type* IRSlalomFunction::getPointerType(LLVMContext& context) {
  return getType(context)->getPointerTo();
}

IRSlalomFunction::IRSlalomFunction(Function* malloc, BasicBlock* block) {
  IRBuilder<> builder(block);

  Type* irStructType = getType(block->getContext());
  Value* allocSize = ConstantExpr::getSizeOf(irStructType);
  Value* mallocResult = builder.CreateCall(malloc, allocSize);

  irStruct = builder.CreateBitCast(mallocResult, irStructType->getPointerTo());
}

IRSlalomFunction::~IRSlalomFunction() {}

void IRSlalomFunction::setArity(int arity, BasicBlock* block) {
  Value* arityPtr = getArityPointer(block);

  // Set the arity to 1
  IRBuilder<> builder(block);
  Type* arityTy = arityPtr->getType()->getPointerElementType();
  Value* arity1 = ConstantInt::get(arityTy, arity);
  builder.CreateStore(arity1, arityPtr);
}

void IRSlalomFunction::setReturn(BasicBlock* block) {
  IRBuilder<> builder(block);
  builder.CreateRet(irStruct);
}

Type* IRSlalomFunction::getType() {
  return irStruct->getType();
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

void IRModuleWriter::declareMalloc() {
  Type* intPtrTy = Type::getInt8PtrTy(module->getContext());
  Type* allocTy = Type::getInt64Ty(module->getContext());
  Constant* mallocC = module->getOrInsertFunction("malloc", intPtrTy, allocTy, NULL);
  malloc = cast<Function>(mallocC);
}

void IRModuleWriter::generateFramework() {
  declareMalloc();

  Type* retTy = IRSlalomFunction::getPointerType(module->getContext());

  Constant* f = module->getOrInsertFunction("createICombinator", retTy, NULL);
  Function* cic = cast<Function>(f);
  BasicBlock* block = BasicBlock::Create(module->getContext(), "entry", cic);

  IRSlalomFunction* sfs = new IRSlalomFunction(module->getFunction("malloc"), block);
  sfs->setArity(1, block);
  sfs->setReturn(block);

  module->dump();
}
