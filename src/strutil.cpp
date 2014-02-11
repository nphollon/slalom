#include "strutil.hpp"

string substrFromStart(const string& text, size_t firstCharPos, size_t lastCharPos) {
  int length = lastCharPos - firstCharPos + 1;
  return text.substr(firstCharPos, length);
}

string substrFromEnds(const string& text, size_t startPos, size_t lenFromEnd) {
  int length = text.length() - lenFromEnd - startPos;
  return text.substr(startPos, length);
}

char lastChar(const string& text) {
  return text.at(text.length() - 1);
}

char firstChar(const string& text) {
  return text.at(0);
}
