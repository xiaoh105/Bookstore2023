#include <Utils.h>

unsigned long long GetHash(const string &s)
{
  unsigned long long ret = 0;
  for (const auto &i: s)
    ret = ret * pow + i;
  return ret;
}