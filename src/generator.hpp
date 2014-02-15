#ifndef H_GENERATE
#define H_GENERATE

#include "node.hpp"

class ModuleWriter {
public:
  virtual ~ModuleWriter() {}
  virtual void createICombinator() = 0;
  virtual void createKCombinator() = 0;
  virtual void createSCombinator() = 0;
  virtual void createDerivedCombinator(const std::string&) = 0;
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
