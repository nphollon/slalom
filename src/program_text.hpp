#ifndef H_PROGRAM_TEXT
#define H_PROGRAM_TEXT

#include <string>
#include <vector>

class ProgramText {
public:
  ProgramText(const std::string&);
  ~ProgramText();

  const std::string toString() const;

  static const std::string WHITESPACE;
  static const std::string NON_NAME_CHARS;

  bool isValid() const;
  bool isEmpty() const;
  std::vector<std::string> splitAtLastToken() const;
  ProgramText* trim() const;
  bool isWrapped() const;
  int findLastOpenParen() const;
  std::string substrFromEnds(size_t startPos, size_t lenFromEnd) const;
  std::string substrFromStart(size_t firstCharPos, size_t lastCharPos) const;
  char lastChar() const;

private:
  const std::string *text;
};

#endif
