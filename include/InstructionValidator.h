#ifndef HOMEWORK5_BOOKSTORE_INSTRUCTIONVALIDATOR_H
#define HOMEWORK5_BOOKSTORE_INSTRUCTIONVALIDATOR_H

#include <string>
#include <initializer_list>
#include <regex>

using std::string;
using std::basic_regex;
using std::regex_constants::optimize;

string GetOptional(const string &s);
string GetMultiple(const string &s);
string GetInstruction(const std::initializer_list<string> &s,
                      bool suffix_space = true);

namespace regex
{
  const string user_id = "([A-Za-z0-9_]{1,30})";
  const string password = "([A-Za-z0-9_]{1,30})";
  const string current_password = "([A-Za-z0-9_]{1,30})";
  const string new_password = "([A-Za-z0-9_]{1,30})";
  const string privilege = "(1|3|7)";
  const string username = "([!-~]{1,30})";
  const string ISBN = "([!-~]{1,20})";
  const string book_name = "([!#-~]{1,60})";
  const string author = "([!#-~]{1,60})";
  const string keyword = "([!#-{}~]{1,60})";
// Notice: No check for length of keywords
  const string keywords = "((?:[!#-{}~]{1,60}\\x7c)*[!#-{}~]{1,60})";
// Notice: No check for maximum of quantity
  const string quantity = "(\\d{1,10})";
  const string price = "(\\d{1,10}\\x2e\\d{2})";
  const string total_cost = "(\\d{1,10}\\x2e\\d{2})";
// Notice: No check for maximum of count
  const string count = "(\\d{1,10})";
  const string book_info_show =
          "(-ISBN=" + ISBN + "\\s+|-name=" + book_name +
          "\\s+|-author=" + author + "\\s+|-keyword=" + keyword + "\\s+)";
  const string book_info_price =
          "(-ISBN=" + ISBN + "\\s+|-name=" + book_name +
          "\\s+|-author=" + author + "\\s+|-keyword=" + keywords +
          "\\s+|-price=" + price + "\\s+)";

  const string quit = GetInstruction({"quit|exit"});
  const string su = GetInstruction({"su", user_id, GetOptional(password)});
  const string logout = GetInstruction({"logout"});
  const string reg = GetInstruction({"register", user_id, password, username});
  const string passwd =
          GetInstruction({"passwd", user_id,
                          GetOptional(current_password), new_password});
  const string useradd =
          GetInstruction({"useradd", user_id, password, privilege, username});
  const string del = GetInstruction({"delete", user_id});

  const string show =
          GetInstruction({"show", GetOptional(book_info_show)}, false);
  const string buy = GetInstruction({"buy", ISBN, quantity});
  const string sel = GetInstruction({"select", ISBN});
  const string modify =
          GetInstruction({"modify", GetMultiple(book_info_price)}, false);
  const string import = GetInstruction({"import", quantity, total_cost});

  const string show_finance =
          GetInstruction({"show", "finance", GetOptional(count)});
  const string log = GetInstruction({"log"});
}

bool GetQuit(const string &s);
bool GetSu(const string &s, string &userid, string &password);
bool GetLogout(const string &s);
bool GetRegister(const string &s, string &userid,
                 string &password, string &username);
bool GetPassword(const string &s, string &userid,
                 string &current_password, string &new_password);
bool GetUserAdd(const string &s, string &userid, string &password,
                string &privilege, string &username);
bool GetDelete(const string &s, string &userid);
bool GetShow(const string &s, string &ISBN, string &name,
             string &author, string &keyword);
bool GetBuy(const string &s, string &ISBN, int &quantity);
bool GetSelect(const string &s, string &ISBN);
// Notice: No check for repeated keyword
bool GetModify(const string &s, string &ISBN, string &name, string &author,
               string &keyword, long double &price);
bool GetImport(const string &s, int &quantity, long double &total_cost);
bool GetShowFinance(const string &s, int &count);
bool GetLog(const string &s);

#endif //HOMEWORK5_BOOKSTORE_INSTRUCTIONVALIDATOR_H
