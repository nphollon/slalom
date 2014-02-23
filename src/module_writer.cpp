#include "module_writer.hpp"

#include "llvm/IR/IRBuilder.h"

IRModuleWriter::IRModuleWriter(llvm::Module *m, llvm::LLVMContext *c) {
  module = m;
  context = c;

  llvm::Type* arityTy = llvm::Type::getInt32Ty(*context);
  llvm::Type* retTy = llvm::StructType::create("sf", arityTy, NULL);
  llvm::Constant* f = module->getOrInsertFunction("createICombinator", retTy, NULL);
  llvm::Function* cic = llvm::cast<llvm::Function>(f);
  llvm::BasicBlock* block = llvm::BasicBlock::Create(*context, "entry", cic);
  llvm::IRBuilder<> builder(block);
  llvm::Constant* retVal = llvm::ConstantInt::get(retTy, 1);
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

