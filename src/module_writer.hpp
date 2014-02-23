#ifndef H_MODULE_WRITER
#define H_MODULE_WRITER

#include "llvm/IR/Module.h"
#include "llvm/IR/LLVMContext.h"

class SlalomFunction {
public:
  SlalomFunction() {}
};

class ModuleWriter {
public:
  virtual ~ModuleWriter() {}
  virtual SlalomFunction* createICombinator() = 0;
  virtual SlalomFunction* createKCombinator() = 0;
  virtual SlalomFunction* createSCombinator() = 0;
  virtual SlalomFunction* createDerivedCombinator(const std::string&) = 0;
  virtual SlalomFunction* createApplication(SlalomFunction*, SlalomFunction*) = 0;
};

class IRModuleWriter : public ModuleWriter {
public:
  IRModuleWriter(llvm::Module*, llvm::LLVMContext*);
  SlalomFunction* createICombinator();
  SlalomFunction* createKCombinator();
  SlalomFunction* createSCombinator();
  SlalomFunction* createDerivedCombinator(const std::string&);
  SlalomFunction* createApplication(SlalomFunction*, SlalomFunction*);

private:
  llvm::Module *module;
  llvm::LLVMContext *context;
};

#endif
