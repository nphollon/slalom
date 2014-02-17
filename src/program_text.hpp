#ifndef H_PROGRAM_TEXT
#define H_PROGRAM_TEXT

#include <string>
#include <vector>

class ProgramText {
public:
  ProgramText(std::string);
  ~ProgramText();

  bool isValid() const;
  bool isEmpty() const;
  std::vector<ProgramText> splitAtLastToken() const;
  ProgramText trim() const;
  std::string toString() const;

private:
  static const std::string WHITESPACE;
  static const std::string NON_NAME_CHARS;

  std::string text;
  size_t length;

  bool isWrapped() const;
  int findLastOpenParen() const;
  ProgramText cropFromEnds(size_t startPos, size_t lenFromEnd) const;
  ProgramText cropFromStart(size_t firstCharPos, size_t lastCharPos) const;
  char lastChar() const;
};

#endif
