#include "mock.hpp"

ModuleWriterMock::ModuleWriterMock() {
  iWasCreated = false;
}

ModuleWriterMock::~ModuleWriterMock() {}

void ModuleWriterMock::createICombinator() {
  iWasCreated = true;
}

bool ModuleWriterMock::didCreateI() {
  return iWasCreated;
}
