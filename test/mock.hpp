#ifndef H_MOCK
#define H_MOCK

#include "../src/generate.hpp"

class ModuleWriterMock : public ModuleWriter {
private:
  bool iWasCreated;
public:
  ModuleWriterMock();
  ~ModuleWriterMock();
  void createICombinator();
  bool didCreateI();
};

#endif
