#ifndef H_PARSE
#define H_PARSE

#include <string>
#include "node.hpp"

using namespace std;

const Node* parse(const string&);

class ParseError {};
class ParenthesesDoNotMatch : public ParseError {};
class MissingWhitespace : public ParseError {};

#endif
