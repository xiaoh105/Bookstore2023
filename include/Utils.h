#ifndef HOMEWORK5_BOOKSTORE_UTILS_H
#define HOMEWORK5_BOOKSTORE_UTILS_H
#include <string>

using std::string;

constexpr unsigned long long pow = 1314527;

unsigned long long GetHash(const string &s);

int StrCount(const string &s, const string &pattern);

#endif //HOMEWORK5_BOOKSTORE_UTILS_H
