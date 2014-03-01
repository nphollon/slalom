#include "ir_module_writer.hpp"

#include <vector>

IRSlalomFunction::IRSlalomFunction(Function* malloc, BasicBlock* block) {
  IRBuilder<> builder(block);

  Type* irStructType = type(block->getContext());
  Value* allocSize = ConstantExpr::getSizeOf(irStructType);
  Value* mallocResult = builder.CreateCall(malloc, allocSize);

  irStruct = builder.CreateBitCast(mallocResult, irStructType->getPointerTo());
}

IRSlalomFunction::~IRSlalomFunction() {}

StructType* IRSlalomFunction::type(LLVMContext& context) {
  Type* arityTy = Type::getInt32Ty(context);
  return StructType::get(arityTy, NULL);
}

Value* IRSlalomFunction::value() {
  return irStruct;
}

void IRSlalomFunction::setArity(int arity, BasicBlock* block) {
  // Get a pointer to the arity element
  IRBuilder<> builder(block);
  Value* idxZero = ConstantInt::get(Type::getInt32Ty(block->getContext()), 0);
  std::vector<Value*> idxList(2, idxZero);
  Value* arityPtr = builder.CreateGEP(irStruct, idxList);

  // Set the arity to 1
  Type* arityTy = type(block->getContext())->getElementType(0);
  Value* arity1 = ConstantInt::get(arityTy, arity);
  builder.CreateStore(arity1, arityPtr);
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

  Type* retTy = IRSlalomFunction::type(module->getContext())->getPointerTo();

  // Create createICombinator() function
  Constant* f = module->getOrInsertFunction("createICombinator", retTy, NULL);
  Function* cic = cast<Function>(f);
  BasicBlock* block = BasicBlock::Create(module->getContext(), "entry", cic);

  // Create IRBuilder for the function
 
  IRSlalomFunction* sfs = new IRSlalomFunction(module->getFunction("malloc"), block);
  sfs->setArity(1, block);
  
  // Return pointer to the struct
  IRBuilder<> builder(block);
  builder.CreateRet(sfs->value());

  module->dump();
}
