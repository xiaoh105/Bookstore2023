#include <iostream>
#include <Utils.h>


unsigned long long GetHash(const string &s)
{
  unsigned long long ret = 0;
  for (const auto &i: s)
    ret = ret * pow + i;
  return ret;
}

unsigned long long CalcHash(const unsigned long long *h,
                            const unsigned long long *p, int l, int r)
{
  if (l == 0) { return h[r]; }
  else { return h[r] - h[l - 1] * p[r - l + 1]; }
}

int StrCount(const string &s, const string &pattern)
{
  if (pattern.length() > s.length()) return 0;
  unsigned long long h[s.length() + 5], h_pattern;
  unsigned long long p[s.length() + 5];
  h[0] = s[0], h_pattern = pattern[0], p[0] = 1;
  for (int i = 1; i < s.length(); ++i)
  {
    h[i] = h[i - 1] * pow + s[i];
    p[i] = p[i - 1] * pow;
  }
  for (int i = 1; i < pattern.length(); ++i)
    h_pattern = h_pattern * pow + pattern[i];
  int ret = 0;
  for (int i = 0; i + pattern.length() <= s.length(); ++i)
    if (CalcHash(h, p, i, i + pattern.length() - 1) == h_pattern)
      ++ret;
  return ret;
}

void Invalid()
{
  std::cout << "Invalid" << std::endl;
}

vector<string> SplitKeyword(const string &s)
{
  vector<string> ret;
  string tmp;
  for (const auto &i: s)
    if (i != '|')
    {
      tmp += i;
    }
    else
    {
      ret.push_back(std::move(tmp));
      tmp = "";
    }
  if (!tmp.empty()) ret.push_back(tmp);
  return std::move(ret);
}