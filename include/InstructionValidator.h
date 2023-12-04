#ifndef HOMEWORK5_BOOKSTORE_INSTRUCTIONVALIDATOR_H
#define HOMEWORK5_BOOKSTORE_INSTRUCTIONVALIDATOR_H

#include <string>
#include <initializer_list>

using std::string;

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
  // Notice: No check for length of keywords
  const string keywords = "((?:[!#-{}~]{1,60}\\x7c)+[!#-{}~]{1,60})";
  // Notice: No check for maximum of quantity
  const string quantity = "(\\d{1,10})";
  const string price = "(\\d{1,10}\\x2e\\d{2})";
  // Notice: No check for maximum of count
  const string count = "(\\d{1,10})";

  string operator !(const string &s);
  string GetInstruction(const std::initializer_list<string> &s);
  const string quit = GetInstruction({"quit|exit"});
  const string su = GetInstruction({"su", user_id, !password});
  const string logout = GetInstruction({"logout"});
  const string reg = GetInstruction({"register", user_id, password, username});
  const string passwd =
          GetInstruction({"passwd", user_id, !current_password, new_password});
  const string useradd =
          GetInstruction({"useradd", user_id, password, privilege, username});
  const string del = GetInstruction({"delete", user_id});
}

#endif //HOMEWORK5_BOOKSTORE_INSTRUCTIONVALIDATOR_H
