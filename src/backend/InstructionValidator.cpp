#include <iostream>
#include "InstructionValidator.h"
#include "Utils.h"

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
basic_regex regex_report(report, optimize);

string GetOptional(const string &s)
{
  return s + '?';
}

string GetMultiple(const string &s)
{
  return s + "+";
}

string GetInstruction(const std::initializer_list<string> &s,
                      bool suffix_space)
{
  string ret = " *";
  for (const auto &str: s)
  {
    ret += str + " +";
  }
  if (suffix_space)
  {
    ret.pop_back();
    ret.push_back('*');
  }
  else
  {
    ret.pop_back(), ret.pop_back();
  }
  return ret;
}

bool GetQuit(const string &s)
{
  return std::regex_match(s, regex_quit);
}

bool GetSu(const string &s, string &userid, string &password)
{
  auto s2 = s + ' ';
  smatch match;
  if (!regex_match(s2, match, regex_su)) return false;
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
  return true;
}

bool GetUserAdd(const string &s, string &userid, string &password,
                int &privilege, string &username)
{
  smatch match;
  regex_match(s, match, regex_useradd);
  if (!regex_match(s, match, regex_useradd)) return false;
  userid = match[1], password = match[2];
  privilege = std::stoi(match[3]), username = match[4];
  return true;
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
  string s2 = s + ' ';
  smatch match;
  if (!regex_match(s2, match, regex_show)) return false;
  ISBN = match[2], name = match[3], author = match[4], keyword = match[5];
  if (keyword.length() > 60) return false;
  return true;
}

bool GetBuy(const string &s, string &ISBN, int &quantity)
{
  smatch match;
  if (!regex_match(s, match, regex_buy)) return false;
  ISBN = match[1];
  string tmp = match[2];
  if (std::stoll(tmp) > INT32_MAX) return false;
  quantity = std::stoi(tmp);
  return true;
}

bool GetImport(const string &s, int &quantity, long double &total_cost)
{
  smatch match;
  if (!regex_match(s, match, regex_import)) return false;
  string tmp1 = match[1], tmp2 = match[2];
  if (std::stoll(tmp1) > INT32_MAX) return false;
  if (tmp2.size() > 13) return false;
  quantity = std::stoi(tmp1), total_cost = std::stold(tmp2);
  return true;
}

bool GetShowFinance(const string &s, int &count)
{
  auto s2 = s + ' ';
  smatch match;
  if (!regex_match(s2, match, regex_show_finance)) return false;
  string tmp = match[1];
  if (tmp.empty())
  {
    count = -1;
    return true;
  }
  if (std::stoll(tmp) > INT32_MAX) return false;
  count = std::stoi(tmp);
  return true;
}

bool GetModify(const string &s, string &ISBN, string &name, string &author,
               string &keyword, long double &price)
{
  string s2 = s + ' ';
  smatch match;
  if (!regex_match(s2, match, regex_modify)) return false;
  ISBN = match[2], name = match[3], author = match[4];
  keyword = match[5];
  string tmp = match[6];
  if (keyword.length() > 60) return false;
  if (StrCount(s, " -ISBN=") >= 2) return false;
  if (StrCount(s, " -name=") >= 2) return false;
  if (StrCount(s, " -author=") >= 2) return false;
  if (StrCount(s, " -keyword=") >= 2) return false;
  if (StrCount(s, " -price=") >= 2) return false;
  if (tmp.size() > 13) return false;
  price = tmp.empty()? -1:std::stold(tmp);
  return true;
}

bool GetReport(const string &s, bool &is_finance)
{
  smatch match;
  if (!regex_match(s, match, regex_report)) return false;
  is_finance = match[1] == "finance";
  return true;
}