#include "program_text.hpp"

ProgramText::ProgramText(const std::string& t) {
  text = new std::string(t);
}

ProgramText::~ProgramText() {
  delete text;
}

const std::string ProgramText::toString() const {
  return *text;
}
