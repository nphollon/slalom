#include "codegenerator.hpp"

CodeGenerator::CodeGenerator() {
  module = new Module("Slalom", llvm::getGlobalContext());
}

CodeGenerator::~CodeGenerator() {
  delete module;
}

void CodeGenerator::generate() const {}

const Module* CodeGenerator::getModule() const {
  return module;
}
