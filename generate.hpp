#ifndef H_GENERATE
#define H_GENERATE

#include "node.hpp"

class ModuleWriter {
public:
  virtual void createICombinator() = 0;
};

void generate(ModuleWriter&, const Node*);

#endif
