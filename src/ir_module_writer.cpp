#include "ir_module_writer.hpp"

#include <vector>

IRSlalomFunction::IRSlalomFunction(Function* malloc, IRBuilder<> &builder) {
  Type* arityTy = Type::getInt32Ty(malloc->getContext());
  irStructType = StructType::create("sf", arityTy, NULL);

  Value* allocSize = ConstantExpr::getSizeOf(irStructType);
  Value* mallocResult = builder.CreateCall(malloc, allocSize);

  irStruct = builder.CreateBitCast(mallocResult, irStructType->getPointerTo());
}

IRSlalomFunction::~IRSlalomFunction() {}

Type* IRSlalomFunction::type() {
  return irStructType;
}

Value* IRSlalomFunction::value() {
  return irStruct;
}

void IRSlalomFunction::setArity(int arity) {
  // Get a pointer to the arity element
  Value* idxZero = ConstantInt::get(Type::getInt32Ty(module->getContext()), 0);
  std::vector<Value*> idxList(2, idxZero);
  Value* arityPtr = builder.CreateGEP(, idxList);

  // Set the arity to 1
  Type* arityTy = sfTy->getElementType(0);
  Value* arity1 = ConstantInt::get(arityTy, arity);
  builder.CreateStore(arity1, arityPtr);
}

ModuleWriter* IRModuleWriter::createModuleWriter(Module *module) {
  IRModuleWriter* writer = new IRModuleWriter(module);
  writer->generateFramework();
  return writer;
}

IRModuleWriter::IRModuleWriter(Module *m) : module(m), builder(m->getContext()) {
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

void IRModuleWriter::setArity(Value* slalomFunction, int arity) {
}

void IRModuleWriter::declareMalloc() {
  Type* intPtrTy = Type::getInt8PtrTy(module->getContext());
  Type* allocTy = Type::getInt64Ty(module->getContext());
  Constant* mallocC = module->getOrInsertFunction("malloc", intPtrTy, allocTy, NULL);
  malloc = cast<Function>(mallocC);
}

void IRModuleWriter::defineSlalomFunctionStruct() {
  Type* arityTy = Type::getInt32Ty(module->getContext());
  sfTy = StructType::create("sf", arityTy, NULL);
}

void IRModuleWriter::generateFramework() {
  declareMalloc();
  defineSlalomFunctionStruct();

  Type* arityTy = Type::getInt32Ty(malloc->getContext());
  Type* retTy = StructType::create("sf", arityTy, NULL)->getPointerTo();

  // Create createICombinator() function
  Constant* f = module->getOrInsertFunction("createICombinator", retTy, NULL);
  Function* cic = cast<Function>(f);

  // Create IRBuilder for the function
  BasicBlock* block = BasicBlock::Create(module->getContext(), "entry", cic);
  builder.SetInsertPoint(block);

  IRSlalomFunction* sfs = new IRSlalomFunction(module->getFunction("malloc"), builder);
  sfs->setArity(1);
  Value* retVal = sfs->value();
  setArity(retVal, 1);
  
  // Return pointer to the struct
  builder.CreateRet(retVal);

  module->dump();
}
