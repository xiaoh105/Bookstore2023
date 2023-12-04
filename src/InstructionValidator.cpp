#include "InstructionValidator.h"

using namespace regex;

string operator !(const string &s)
{
  return s + "?";
}

string GetInstruction(const std::initializer_list<string> &s)
{
  string ret = "\\s*";
  for (const auto &str: s)
  {
    ret += str + "\\s+";
  }
  ret.pop_back();
  ret.push_back('*');
}