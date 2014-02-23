#include "module_writer.hpp"

#include <vector>

#include "llvm/IR/IRBuilder.h"

IRModuleWriter::IRModuleWriter(llvm::Module *m, llvm::LLVMContext *c) {
  module = m;
  context = c;

  // Create SlalomFunction struct type
  llvm::Type* arityTy = llvm::Type::getInt32Ty(*context);
  llvm::Type* sfTy = llvm::StructType::create("sf", arityTy, NULL);
  llvm::Type* retTy = llvm::PointerType::get(sfTy, 0);

  // Declare malloc
  llvm::Type* intPtrTy = llvm::Type::getInt8PtrTy(*context);
  llvm::Type* allocTy = llvm::Type::getInt64Ty(*context);
  llvm::Constant* mallocC = module->getOrInsertFunction("malloc", intPtrTy, allocTy, NULL);
  llvm::Function* mallocF = llvm::cast<llvm::Function>(mallocC);

  // Create createICombinator() function
  llvm::Constant* f = module->getOrInsertFunction("createICombinator", retTy, NULL);
  llvm::Function* cic = llvm::cast<llvm::Function>(f);

  // Create IRBuilder for the function
  llvm::BasicBlock* block = llvm::BasicBlock::Create(*context, "entry", cic);
  llvm::IRBuilder<> builder(block);
  
  // Allocate memory for a struct using malloc
  llvm::Value* allocSize = llvm::ConstantExpr::getSizeOf(sfTy);
  llvm::Value* mallocResult = builder.CreateCall(mallocF, allocSize);
  llvm::Value* retVal = builder.CreateBitCast(mallocResult, retTy);
  
  // Get a pointer to the arity element
  llvm::Value* idxZero = llvm::ConstantInt::get(llvm::Type::getInt32Ty(*context), 0);
  std::vector<llvm::Value*> idxList(2, idxZero);
  llvm::Value* arityPtr = builder.CreateGEP(retVal, idxList);

  // Set the arity to 1
  llvm::Value* arity1 = llvm::ConstantInt::get(arityTy, 1);
  builder.CreateStore(arity1, arityPtr);
  
  // Return pointer to the struct
  builder.CreateRet(retVal);

  module->dump();
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

