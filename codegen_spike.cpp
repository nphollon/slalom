#include <iostream>
#include <vector>

#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/PassManager.h"
#include "llvm/Assembly/PrintModulePass.h"
#include "llvm/Support/raw_ostream.h"

#include "node.hpp"

using namespace llvm;

class CodeGenerator {
private:
  Module* module;
  IRBuilder<>* builder;
  Type* firstClassFunctionType;
  void buildBaseFunctions();
public:
  CodeGenerator();
  ~CodeGenerator();
  Value* generateFromNode(const Node&) const;
  Module* getModule() const;
};

CodeGenerator::CodeGenerator() {
  firstClassFunctionType = IntegerType::get(getGlobalContext(), 2); 
  //StructType::create(getGlobalContext());
  module = new Module("Slalom", getGlobalContext());
  builder = new IRBuilder<>(getGlobalContext());
  buildBaseFunctions();
}

CodeGenerator::~CodeGenerator() {
  delete builder;
  delete module;
}

void CodeGenerator::buildBaseFunctions() {
  // Define I
  Function* iComb = cast<Function>
    (module->getOrInsertFunction("I", firstClassFunctionType,
                                 firstClassFunctionType, NULL));
  Value* iArg = iComb->arg_begin();
  BasicBlock *iEntry = BasicBlock::Create(getGlobalContext(), "entry", iComb);
  builder->SetInsertPoint(iEntry);
  builder->CreateRet(iArg);
  
  // Define K
  Function* kComb = cast<Function>
    (module->getOrInsertFunction("K", firstClassFunctionType,
                                 firstClassFunctionType, firstClassFunctionType, NULL));
  Function::arg_iterator kArgIter = kComb->arg_begin();
  Value* kArg1 = kArgIter;
  kArgIter++;
  Value* kArg2 = kArgIter;
  BasicBlock *kEntry = BasicBlock::Create(getGlobalContext(), "entry", kComb);
  builder->SetInsertPoint(kEntry);
  builder->CreateRet(kArg1);

  FunctionType* iCombType = iComb->getFunctionType();
  Function* apply = cast<Function>
    (module->getOrInsertFunction("apply", firstClassFunctionType,
                                 firstClassFunctionType, firstClassFunctionType, NULL));
  BasicBlock *applyEntry = BasicBlock::Create(getGlobalContext(), "entry", apply);
  builder->SetInsertPoint(applyEntry);
  
  
  cerr << "Function apply is unfinished\n";
  return;
  
  // Define S
  Function* sComb = cast<Function>
    (module->getOrInsertFunction("S", firstClassFunctionType, firstClassFunctionType,
                                 firstClassFunctionType, firstClassFunctionType, NULL));
  Function::arg_iterator sArgIter = sComb->arg_begin();
  Value* sArg1 = sArgIter;
  sArgIter++;
  Value* sArg2 = sArgIter;
  sArgIter++;
  Value* sArg3 = sArgIter;
  BasicBlock *sEntry = BasicBlock::Create(getGlobalContext(), "entry", sComb);
  builder->SetInsertPoint(sEntry);
  Value* apply1To3 = builder->CreateCall2(apply, sArg1, sArg3, "applyCall");
  Value* apply2To3 = builder->CreateCall2(apply, sArg2, sArg3, "applyCall");
  Value* sReturn = builder->CreateCall2(apply, apply1To3, apply2To3, "applyCall");
  builder->CreateRet(sReturn);
  //*/
}

// Need to worry about circular dependency?
Value* CodeGenerator::generateFromNode(const Node& node) const {
  if (!node.isTerminal()) {
    Value* applicatorCode = generateFromNode(*node.getApplicator());
    Value* inputCode = generateFromNode(*node.getInput());
    Function* apply = module->getFunction("apply");
    return builder->CreateCall2(apply, applicatorCode, inputCode, "applyCall");
  }

  /*
  if [name is known] {
    Function* f = [look up function from name]
    int a = [arity of *f]
    return [create new slalfunc(f, name, a, vector<>())]
  }

  return [create new slalfunc(NULL, name, -1, vector<>())]
  */
  return 0;
}

Module* CodeGenerator::getModule() const {
  return module;
}

int main() {
  CodeGenerator *cg = new CodeGenerator();
  if (cg == NULL) {
    cerr << "CodeGenerator object could not be instantiated.\n";
    return 1;
  }

  const Node node("A");
  if (cg->generateFromNode(node) == NULL) {
    cerr << "Code generation from parse tree failed.\n";
  }

  PassManager PM;
  PM.add(createPrintModulePass(&outs()));
  PM.run(*cg->getModule());

  delete cg;
  return 0;
}
