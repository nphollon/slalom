#include "test_jit.hpp"

#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/Support/TargetSelect.h"

#include "../src/ir_module_writer.hpp"

TestJIT::TestJIT() {
  llvm::InitializeNativeTarget();
  module = new llvm::Module("JIT", llvm::getGlobalContext());
  writer = IRModuleWriter::createModuleWriter(module);
  engine = llvm::EngineBuilder(module).create();
}

TestJIT:: ~TestJIT() {
  delete module;
  delete writer;
}

FactoryFunction TestJIT::getFactoryFunctionPointer(const std::string& name) {
  llvm::Function *function = module->getFunction(name);
  return (FactoryFunction)(intptr_t)engine->getPointerToFunction(function);
}

void TestJIT::dumpModule() {
  module->dump();
}
