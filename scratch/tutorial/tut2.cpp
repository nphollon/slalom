/*
  tut2.cpp
  Creates LLVM module with function that computes greatest common denominator:
    int gcd(int x, int y) {
      if (x == y) return x;
      if (x < y) {
        return gcd(x, y - x);
      }
      return gcd(x - y, y);
    }
  See gcd.ll for annotations of the resulting IR code.
*/

#include "llvm/Module.h"
#include "llvm/Function.h"
#include "llvm/PassManager.h"
#include "llvm/CallingConv.h"
#include "llvm/Analysis/Verifier.h"
#include "llvm/Assembly/PrintModulePass.h"
#include "llvm/Support/IRBuilder.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

Module* makeLLVMModule();

int main(int argc, char** argv) {
  Module* Mod = makeLLVMModule();

  verifyModule(*Mod, PrintMessageAction);

  PassManager PM;
  PM.add(createPrintModulePass(&outs()));
  PM.run(*Mod);

  delete Mod;
  return 0;
}

Module* makeLLVMModule() {
  Module* mod = new Module("tut2", getGlobalContext());
  Constant* c = 
    mod->getOrInsertFunction("gcd",
                             IntegerType::get(getGlobalContext(), 32),
                             IntegerType::get(getGlobalContext(), 32),
                             IntegerType::get(getGlobalContext(), 32),
                             NULL); // varargs terminated with null
  Function* gcd = cast<Function>(c);
  gcd->setCallingConv(CallingConv::C);

  Function::arg_iterator args = gcd->arg_begin();
  Value* x = args++;
  x->setName("x");
  Value* y = args++;
  y->setName("y");

  BasicBlock* entry = BasicBlock::Create(getGlobalContext(),
                                         "entry", gcd);
  BasicBlock* ret = BasicBlock::Create(getGlobalContext(),
                                       "return", gcd);
  BasicBlock* cond_false = BasicBlock::Create(getGlobalContext(),
                                       "cond_false", gcd);
  BasicBlock* cond_true = BasicBlock::Create(getGlobalContext(),
                                       "cond_true", gcd);
  BasicBlock* cond_false_2 = BasicBlock::Create(getGlobalContext(),
                                       "cond_false", gcd);
  
  IRBuilder<> builder(entry);
  Value* xEqualsY = builder.CreateICmpEQ(x, y, "tmp");
  builder.CreateCondBr(xEqualsY, ret, cond_false);

  builder.SetInsertPoint(ret);
  builder.CreateRet(x);

  builder.SetInsertPoint(cond_false);
  Value* xLessThanY = builder.CreateICmpULT(x, y, "tmp");
  builder.CreateCondBr(xLessThanY, cond_true, cond_false_2);

  builder.SetInsertPoint(cond_true);
  Value* yMinusX = builder.CreateSub(y, x, "tmp");
  // Function calls with arity < 6 can be created with special methods:
  Value* recur_1 = builder.CreateCall2(gcd, x, yMinusX, "tmp");
  builder.CreateRet(recur_1);

  builder.SetInsertPoint(cond_false_2);
  Value* xMinusY = builder.CreateSub(x, y, "tmp");
  // Function calls can also be given their args in a vector:
  std::vector<Value*> args2;
  args2.push_back(xMinusY);
  args2.push_back(y);
  Value* recur_2 = builder.CreateCall(gcd, args2, "tmp");
  builder.CreateRet(recur_2);
 
  return mod;
}
