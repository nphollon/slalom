#include "test_jit.hpp"

#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/Support/TargetSelect.h"

TestJIT::TestJIT() {
  llvm::InitializeNativeTarget();
  module = new llvm::Module("JIT", llvm::getGlobalContext());
  writer = new IRModuleWriter(module, &llvm::getGlobalContext());
  engine = llvm::EngineBuilder(module).create();
}

FactoryFunction TestJIT::getFunctionPointer(const std::string& name) {
  llvm::Function *function = module->getFunction(name);
  return (FactoryFunction)(intptr_t)engine->getPointerToFunction(function);
}

TestJIT:: ~TestJIT() {
}

