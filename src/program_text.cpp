#include "program_text.hpp"

const std::string ProgramText::WHITESPACE = "\t ";
const std::string ProgramText::NON_NAME_CHARS = WHITESPACE + "()";

ProgramText::ProgramText(const std::string& t) {
  text = new std::string(t);
}

ProgramText::~ProgramText() {
  delete text;
}

bool ProgramText::isValid() const {
  int nestLevel = 0;
  for (int i = 0; i < text->length() && nestLevel <= 0; i++) {
    if (text->at(i) == '(') {
      nestLevel--;
    } else if (text->at(i) == ')') {
      nestLevel++;
    }
  }

  return nestLevel == 0;
}

bool ProgramText::isEmpty() const {
  return text->empty();
}

// Identifies last token in expression, as delimited by whitespace or parens
// Splits expression at delimiter and returns 2 subexpressions
// Returns full expression if only one token exists
// -- Examples --
// "A B C" returns {"A B", "C"}
// "A (B C)" returns {"A", "(B C)"}
// "A" returns {"A"}
// "(A (B C))" returns {"(A (B C))"}
std::vector<std::string> ProgramText::splitAtLastToken() const {
  int lastTokenPos = findLastOpenParen();
  
  if (lastTokenPos == text->length()) {
    lastTokenPos = text->find_last_of(NON_NAME_CHARS) + 1;
  }

  std::string lastToken = text->substr(lastTokenPos);

  std::vector<std::string> tokens;
  if (lastToken != *text) {
    std::string firstToken = text->substr(0, lastTokenPos);
    tokens.push_back(firstToken);
  }
  tokens.push_back(lastToken);
  return tokens;
}

// Returns expression without leading/trailing whitespace and wrapping parens
ProgramText* ProgramText::trim() const {
  size_t firstCharPos = text->find_first_not_of(WHITESPACE);

  if (text->empty() || firstCharPos == std::string::npos) {
    return new ProgramText("");
  }

  size_t lastCharPos = text->find_last_not_of(WHITESPACE);
  std::string trimmed = substrFromStart(firstCharPos, lastCharPos);

  ProgramText *pTrimmed = new ProgramText(trimmed);
  if (pTrimmed->isWrapped()) {
    ProgramText pNoParens = ProgramText(pTrimmed->substrFromEnds(1, 1));
    ProgramText *recurseTrimmed = pNoParens.trim();
    delete pTrimmed;
    return recurseTrimmed;
  }

  return pTrimmed;
}

// Returns true if expression is wrapped in parens
bool ProgramText::isWrapped() const {
  return (text->length() > 0) && (findLastOpenParen() == 0);
}

// If last character of expression is not ')',
//    return length of expression
// If last ')' has no matching '('
//    return -1
// Else
//    return position of '(' matching last ')'
int ProgramText::findLastOpenParen() const {
  if (lastChar() != ')') {
    return text->length();
  }

  int nestLevel = 1;
  int i = text->length() - 1;
  while (nestLevel > 0 && i > 0) {
    i--;
    if (text->at(i) == '(') {
      nestLevel--;
    } else if (text->at(i) == ')') {
      nestLevel++;
    }
  }
  return i;
}

std::string ProgramText::substrFromEnds(size_t startPos, size_t lenFromEnd) const {
  int length = text->length() - lenFromEnd - startPos;
  return text->substr(startPos, length);
}

std::string ProgramText::substrFromStart(size_t firstCharPos, size_t lastCharPos) const {
  int length = lastCharPos - firstCharPos + 1;
  return text->substr(firstCharPos, length);
}

char ProgramText::lastChar() const {
  return text->at(text->length() - 1);
}

const std::string ProgramText::toString() const {
  return *text;
}
