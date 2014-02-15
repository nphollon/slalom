#ifndef H_PROGRAM_TEXT
#define H_PROGRAM_TEXT

#include <string>

class ProgramText {
public:
  ProgramText(const std::string&);
  ~ProgramText();
  const std::string toString() const;
private:
  const std::string *text;
};

#endif
