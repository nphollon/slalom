#include <map>

#include "generate.hpp"

void generate(ModuleWriter *writer, const Node *parseTree) {
  const std::string name = parseTree->getName();
  if (name == "I") {
    writer->createICombinator();
  } else if (name == "K") {
    writer->createKCombinator();
  } else if (name == "S") {
    writer->createSCombinator();
  } else {
    writer->createDerivedCombinator(name);
  }
};
