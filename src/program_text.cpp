#include "program_text.hpp"

const std::string ProgramText::WHITESPACE = "\t ";
const std::string ProgramText::NON_NAME_CHARS = WHITESPACE + "()";

ProgramText::ProgramText(const std::string t) {
  text = t;
  length = text.length();
}

ProgramText::~ProgramText() {}

std::string ProgramText::toString() const {
  return text;
}

bool ProgramText::isValid() const {
  int nestLevel = 0;
  for (int i = 0; i < length && nestLevel <= 0; i++) {
    if (text.at(i) == '(') {
      nestLevel--;
    } else if (text.at(i) == ')') {
      nestLevel++;
    }
  }

  return nestLevel == 0;
}

bool ProgramText::isEmpty() const {
  return text.empty();
}

// Returns true if expression is wrapped in parens
bool ProgramText::isWrapped() const {
  return (length > 0) && (findLastOpenParen() == 0);
}

// Identifies last token in expression, as delimited by whitespace or parens
// Splits expression at delimiter and returns 2 subexpressions
// Returns full expression if only one token exists
// -- Examples --
// "A B C" returns {"A B", "C"}
// "A (B C)" returns {"A", "(B C)"}
// "A" returns {"A"}
// "(A (B C))" returns {"(A (B C))"}
std::vector<ProgramText> ProgramText::splitAtLastToken() const {
  int lastTokenPos = findLastOpenParen();
  
  if (lastTokenPos == length) {
    lastTokenPos = text.find_last_of(NON_NAME_CHARS) + 1;
  }

  ProgramText lastToken = cropFromEnds(lastTokenPos, 0);
  std::vector<ProgramText> tokens;
  if (lastToken.text != text) {
    ProgramText firstToken = cropFromStart(0, lastTokenPos-1);
    tokens.push_back(firstToken);
  }
  tokens.push_back(lastToken);
  return tokens;
}

// Returns expression without leading/trailing whitespace and wrapping parens
ProgramText ProgramText::trim() const {
  size_t firstCharPos = text.find_first_not_of(WHITESPACE);

  if (isEmpty() || firstCharPos == std::string::npos) {
    return ProgramText("");
  }

  size_t lastCharPos = text.find_last_not_of(WHITESPACE);
  ProgramText trimmed = cropFromStart(firstCharPos, lastCharPos) ;

  if (trimmed.isWrapped()) {
    return trimmed.cropFromEnds(1, 1).trim();
  }

  return trimmed;
}

// If last character of expression is not ')',
//    return length of expression
// If last ')' has no matching '('
//    return -1
// Else
//    return position of '(' matching last ')'
int ProgramText::findLastOpenParen() const {
  if (lastChar() != ')') {
    return length;
  }

  int nestLevel = 1;
  int i = length - 1;
  while (nestLevel > 0 && i > 0) {
    i--;
    if (text.at(i) == '(') {
      nestLevel--;
    } else if (text.at(i) == ')') {
      nestLevel++;
    }
  }
  return i;
}

ProgramText ProgramText::cropFromEnds(size_t startPos, size_t lengthFromEnd) const {
  int substrLength = length - lengthFromEnd - startPos;
  return ProgramText(text.substr(startPos, substrLength));
}

ProgramText ProgramText::cropFromStart(size_t firstCharPos, size_t lastCharPos) const {
  int substrLength = lastCharPos - firstCharPos + 1;
  return ProgramText(text.substr(firstCharPos, substrLength));
}

char ProgramText::lastChar() const {
  return text.at(length - 1);
}
