#ifndef H_GENERATE
#define H_GENERATE

#include "node.hpp"

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

class Generator {
public:
  Generator(ModuleWriter&);
  ~Generator();
  void generate(const std::string&) const;
private:
  ModuleWriter* writer;
  void generateFromParseTree(const Node*) const;
};

#endif
