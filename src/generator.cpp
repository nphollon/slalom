#include "generator.hpp"

Generator::Generator(ModuleWriter &mw) {
  writer = &mw;
}

Generator::~Generator() {}

void Generator::generate(const std::string &program) const {
  generateFromParseTree(Node::parse(program));
}


void Generator::generateFromParseTree(const Node *parseTree) const {
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
}
