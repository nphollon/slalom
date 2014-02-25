#include "ir_module_writer.hpp"

#include <vector>

ModuleWriter* IRModuleWriter::createModuleWriter(Module *module, LLVMContext *context) {
  IRModuleWriter* writer = new IRModuleWriter(module, context);
  writer->generateFramework();
  return writer;
}

IRModuleWriter::IRModuleWriter(Module *m, LLVMContext *c) : module(m), context(c), builder(*c) {
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

Value* IRModuleWriter::createSlalomFunctionStruct(Type* sfTy) {
  Type* retTy = PointerType::get(sfTy, 0);
  // Declare malloc
  Type* intPtrTy = Type::getInt8PtrTy(*context);
  Type* allocTy = Type::getInt64Ty(*context);
  Constant* mallocC = module->getOrInsertFunction("malloc", intPtrTy, allocTy, NULL);
  Function* mallocF = cast<Function>(mallocC);
  
  // Allocate memory for a struct using malloc
  Value* allocSize = ConstantExpr::getSizeOf(sfTy);
  Value* mallocResult = builder.CreateCall(mallocF, allocSize);
  return builder.CreateBitCast(mallocResult, retTy);
}

void IRModuleWriter::generateFramework() {
  // Create SlalomFunction struct type
  Type* arityTy = Type::getInt32Ty(*context);
  Type* sfTy = StructType::create("sf", arityTy, NULL);
  Type* retTy = PointerType::get(sfTy, 0);

  // Create createICombinator() function
  Constant* f = module->getOrInsertFunction("createICombinator", retTy, NULL);
  Function* cic = cast<Function>(f);

  // Create IRBuilder for the function
  BasicBlock* block = BasicBlock::Create(*context, "entry", cic);
  builder.SetInsertPoint(block);


  Value* retVal = createSlalomFunctionStruct(sfTy);

  // Get a pointer to the arity element
  Value* idxZero = ConstantInt::get(Type::getInt32Ty(*context), 0);
  std::vector<Value*> idxList(2, idxZero);
  Value* arityPtr = builder.CreateGEP(retVal, idxList);

  // Set the arity to 1
  Value* arity1 = ConstantInt::get(arityTy, 1);
  builder.CreateStore(arity1, arityPtr);
  
  // Return pointer to the struct
  builder.CreateRet(retVal);

  module->dump();
}
