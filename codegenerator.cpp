#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"

#include "codegenerator.hpp"

CodeGenerator::CodeGenerator() {
  module = new Module("Slalom", llvm::getGlobalContext());
}

CodeGenerator::~CodeGenerator() {
  delete module;
}

void CodeGenerator::generate() const {
  Constant* c = module->getOrInsertFunction("add",
                              IntegerType::get(getGlobalContext(), 32), 
                              IntegerType::get(getGlobalContext(), 32),
                              IntegerType::get(getGlobalContext(), 32),
                              NULL);
  Function* add = cast<Function>(c);
  Function::arg_iterator args = add->arg_begin();
  Value* x = args++;
  Value* y = args++;

  BasicBlock* block = BasicBlock::Create(getGlobalContext(), "entry", add);
  IRBuilder<> builder(block);
  Value* tmp = builder.CreateBinOp(Instruction::Add, x, y, "tmp");
  builder.CreateRet(tmp);
}

Module* CodeGenerator::getModule() const {
  return module;
}
