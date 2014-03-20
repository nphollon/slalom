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

void IRModuleWriter::generateFramework() {
  IRTypeManager* tm = new IRTypeManager(module);

  BasicBlock* block = tm->openFactoryFunction("createICombinator");

  IRQueueNode* qn = tm->buildQueueNode(block);
  qn->setData(tm->nullSlalomFunction(), block);
  
  IRArgumentsQueue* q = tm->buildArgumentsQueue(block);
  q->setLength(0, block);
  q->setHead(qn, block);
  
  IRSlalomFunction* sfs = tm->buildSlalomFunction(block);
  sfs->setArity(1, block);
  sfs->setName("I", block);
  sfs->setArguments(q, block);
  
  sfs->setReturn(block);
}
