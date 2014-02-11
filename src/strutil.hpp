#ifndef H_STRUTIL
#define H_STRUTIL

#include <string>

using namespace std;

string substrFromEnds(const string&, size_t, size_t);
string substrFromStart(const string&, size_t, size_t);
char lastChar(const string&);
char firstChar(const string&);

#endif
