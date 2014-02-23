#ifndef H_GENERATE
#define H_GENERATE

#include "node.hpp"
#include "module_writer.hpp"

class Generator {
public:
  Generator(ModuleWriter&);
  ~Generator();
  void generate(const std::string&) const;

private:
  ModuleWriter* writer;

  SlalomFunction* generateFromParseTree(const Node*) const;
  SlalomFunction* generateFromName(const std::string&) const;
  SlalomFunction* generateApplication(const Node*, const Node*) const;
};

#endif
