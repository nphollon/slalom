#include "generator.hpp"

Generator::Generator(ModuleWriter &mw) {
  writer = &mw;
}

Generator::~Generator() {}

void Generator::generate(const std::string &program) const {
  generateFromParseTree(Node::parse(program));
}

SlalomFunction* Generator::generateFromParseTree(const Node *parseTree) const {
  if (parseTree->isTerminal()) {
    return generateFromName(parseTree->getName());
  } else {
    return generateApplication(parseTree->getApplicator(), parseTree->getInput());
  }
}

SlalomFunction* Generator::generateFromName(const std::string& name) const {
  if (name == "I") {
    return writer->createICombinator();
  } else if (name == "K") {
    return writer->createKCombinator();
  } else if (name == "S") {
    return writer->createSCombinator();
  } else {
    return writer->createDerivedCombinator(name);
  }
}

SlalomFunction* Generator::generateApplication(const Node *applicator, const Node *input) const {
  SlalomFunction* appFunc = generateFromParseTree(applicator);
  SlalomFunction* inpFunc = generateFromParseTree(input);
  return writer->createApplication(appFunc, inpFunc);
}
