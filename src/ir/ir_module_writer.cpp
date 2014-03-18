#include "ir_module_writer.hpp"

#include <vector>

#include "ir_slalom_function.hpp"

ModuleWriter* IRModuleWriter::createModuleWriter(Module *module) {
  IRModuleWriter* writer = new IRModuleWriter(module);
  writer->generateFramework();
  return writer;
}

IRModuleWriter::IRModuleWriter(Module *m) : module(m) {
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

BasicBlock* IRModuleWriter::openFactoryFunction(const std::string& name, Module* module) {
  Type* retTy = IRSlalomFunction::getPointerType(module->getContext());
  Constant* functionAsConstant = module->getOrInsertFunction(name, retTy, NULL);
  Function* function = cast<Function>(functionAsConstant);
  return BasicBlock::Create(module->getContext(), "entry", function);
}

void IRModuleWriter::generateFramework() {
  BasicBlock* block = openFactoryFunction("createICombinator", module);

  IRTypeManager* tm = new IRTypeManager(module);
  Function* malloc = tm->getMalloc();

  IRQueueNode* qn = new IRQueueNode(malloc, block);
  qn->setData(IRSlalomFunction::getNull(module->getContext()), block);

  IRArgumentsQueue* q = new IRArgumentsQueue(malloc, block);
  q->setLength(0, block);
  q->setHead(qn, block);

  IRSlalomFunction* sfs = new IRSlalomFunction(malloc, block);
  sfs->setArity(1, block);
  sfs->setName("I", block);
  sfs->setArguments(q, block);

  sfs->setReturn(block);
}
