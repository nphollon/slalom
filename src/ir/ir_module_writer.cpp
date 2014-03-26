#include "ir_module_writer.hpp"

#include <vector>

#include "llvm/IR/IRBuilder.h"

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
  tm->describeFactoryFunction("createICombinator", 1, "I");
  tm->describeFactoryFunction("createKCombinator", 2, "K");
}
