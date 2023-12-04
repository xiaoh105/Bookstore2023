#include "InstructionValidator.h"

using namespace regex;
using std::regex_match;
using std::smatch;
using std::basic_regex;

basic_regex regex_quit(quit, optimize);
basic_regex regex_su(su, optimize);
basic_regex regex_logout(logout, optimize);
basic_regex regex_reg(reg, optimize);
basic_regex regex_passwd(passwd, optimize);
basic_regex regex_useradd(useradd, optimize);
basic_regex regex_del(del, optimize);
basic_regex regex_show(show, optimize);
basic_regex regex_buy(buy, optimize);
basic_regex regex_select(sel, optimize);
basic_regex regex_modify(modify, optimize);
basic_regex regex_import(import, optimize);
basic_regex regex_show_finance(show_finance, optimize);
basic_regex regex_log(log, optimize);

string GetOptional(const string &s)
{
  return s + "?";
}

string GetMultiple(const string &s)
{
  return s + "+";
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
  return ret;
}

bool GetQuit(const string &s)
{
  return std::regex_match(s, regex_quit);
}

bool GetSu(const string &s, string &userid, string &password)
{
  smatch match;
  if (!regex_match(s, match, regex_su)) return false;
  userid = match[1], password = match[2];
  return true;
}

bool GetLogout(const string &s)
{
  return std::regex_match(s, regex_logout);
}

bool GetRegister(const string &s, string &userid,
                 string &password, string &username)
{
  smatch match;
  if (!regex_match(s, match, regex_reg)) return false;
  userid = match[1], password = match[2], username = match[3];
  return true;
}

bool GetPassword(const string &s, string &userid,
                 string &current_password, string &new_password)
{
  smatch match;
  if (!regex_match(s, match, regex_passwd)) return false;
  userid = match[1], current_password = match[2], new_password = match[3];
}

bool GetUserAdd(const string &s, string &userid, string &password,
                string &privilege, string &username)
{
  smatch match;
  if (!regex_match(s, match, regex_useradd)) return false;
  userid = match[1], password = match[2];
  privilege = match[3], username = match[4];
}

bool GetDelete(const string &s, string &userid)
{
  smatch match;
  if (!regex_match(s, match, regex_del)) return false;
  userid = match[1];
  return true;
}

bool GetSelect(const string &s, string &ISBN)
{
  smatch match;
  if (!regex_match(s, match, regex_select)) return false;
  ISBN = match[1];
  return true;
}

bool GetLog(const string &s)
{
  return regex_match(s, regex_log);
}

bool GetShow(const string &s, string &ISBN, string &name,
             string &author, string &keyword)
{
  smatch match;
  if (!regex_match(s, match, regex_show)) return false;
  ISBN = match[1], name = match[2], author = match[3], keyword = match[4];
  if (keyword.length() > 60) return false;
  return true;
}

bool GetBuy(const string &s, string &ISBN, string &quantity)
{
  smatch match;
  if (!regex_match(s, match, regex_buy)) return false;
  ISBN = match[1], quantity = match[2];
  
}