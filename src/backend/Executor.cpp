#include <iostream>
#include "InstructionValidator.h"
#include "Executor.h"
#include "Utils.h"
#include "BookSystem.h"
#include "UserSystem.h"
#include "Log.h"

using std::cin, std::cout, std::endl;

BookSystem book;
UserSystem user;
FinanceLog finance_log;
FinanceReport finance_report;
Executor executor;

Executor::Executor():name_stack(), book_stack(), id_map(), id_stack()
{
  p_stack.push(Privilege::tourist);
}

Privilege Executor::GetPrivilege()
{
  return p_stack.top();
}

void Executor::PrintInfo()
{
  cout << id_stack.top() << " " << name_stack.top()
       << " " << (int)GetPrivilege() << endl;
}

void Executor::listen()
{
  bool flag = true;
  while (flag)
  {
    string command;
    getline(cin, command);
    if (cin.eof()) { flag = false; }
    string op = GetWord(command);
    if (op.empty())
    {
      continue;
    }
    else if (op == "quit" || op == "exit")
    {
      if (GetQuit(command)) { flag = false; }
      else { Invalid(); }
    }
    else if (op == "su")
    {
      string userid, password, username;
      Privilege cur_privilege;
      if (GetSu(command, userid, password))
      {
        if (user.Login(userid, password, GetPrivilege(),
                       cur_privilege, username))
        {
          name_stack.push(username);
          p_stack.push(cur_privilege);
          book_stack.push(-1);
          id_stack.push(userid);
          ++id_map[userid];
          cout << "succeed " << static_cast<int>(GetPrivilege()) << " "
               << name_stack.top() << endl;
        }
      }
      else { Invalid(); }
    }
    else if (op == "logout")
    {
      if (GetLogout(command))
      {
        if (name_stack.empty()) { Invalid(); }
        else
        {
          name_stack.pop();
          p_stack.pop();
          --id_map[id_stack.top()];
          if (id_map[id_stack.top()] == 0) id_map.erase(id_stack.top());
          id_stack.pop();
          book_stack.pop();
        }
      }
      else { Invalid(); }
    }
    else if (op == "register")
    {
      string userid, password, username;
      Privilege cur_privilege;
      if (GetRegister(command, userid, password, username) && user.Register(userid, password, username))
      {
        cout << "succeed 1 " << username << endl;
        user.Login(userid, password, GetPrivilege(), cur_privilege, username);
        name_stack.push(username);
        p_stack.push(cur_privilege);
        book_stack.push(-1);
        id_stack.push(userid);
        ++id_map[userid];
      }
      else { Invalid(); }
    }
    else if (op == "passwd")
    {
      string userid, cur_password, new_password;
      if (GetPrivilege() >= Privilege::customer &&
          GetPassword(command, userid, cur_password, new_password))
      {
        if (user.Passwd(userid, GetPrivilege(), cur_password, new_password))
          cout << "succeed" << endl;
        else Invalid();
      }
      else { Invalid(); }
    }
    else if (op == "useradd")
    {
      string userid, password, username;
      int privilege;
      if (GetPrivilege() >= Privilege::employee &&
          GetUserAdd(command, userid, password, privilege, username))
      {
        user.UserAdd(userid, password, privilege, userid, GetPrivilege());
      }
      else
      {
        Invalid();
      }
    }
    else if (op == "delete")
    {
      string userid;
      if (GetPrivilege() >= Privilege::root &&
          GetDelete(command, userid))
      {
        if (id_map[userid]) { Invalid(); }
        else
        {
          user.Delete(userid);
        }
      }
      else
      {
        Invalid();
      }
    }
    else if (op == "show")
    {
      string ISBN, name, author, keyword;
      int count;
      if (GetPrivilege() >= Privilege::tourist &&
          GetShow(command, ISBN, name, author, keyword))
      {
        book.Show(ISBN, name, author, keyword);
      }
      else if (GetPrivilege() >= Privilege::root &&
               GetShowFinance(command, count))
      {
        finance_log.Print(count);
      }
      else
      {
        Invalid();
      }
    }
    else if (op == "buy")
    {
      string ISBN;
      int quantity;
      if (GetPrivilege() >= Privilege::customer &&
          GetBuy(command, ISBN, quantity))
      {
        book.Buy(ISBN, quantity);
      }
      else
      {
        Invalid();
      }
    }
    else if (op == "select")
    {
      string ISBN;
      if (GetPrivilege() >= Privilege::employee &&
          GetSelect(command, ISBN))
      {
        int id = book.Select(ISBN);
        book_stack.pop();
        book_stack.push(id);
      }
      else
      {
        Invalid();
      }
    }
    else if (op == "modify")
    {
      string ISBN, name, author, keyword;
      long double price;
      if (GetPrivilege() >= Privilege::employee &&
          GetModify(command, ISBN, name, author, keyword, price))
      {
        if (book_stack.top() == npos) { Invalid(); }
        else
        {
          int id = book_stack.top();
          book.Modify(id, ISBN, name, author, keyword, price);
        }
      }
      else
      {
        Invalid();
      }
    }
    else if (op == "import")
    {
      int quantity;
      long double total_cost;
      if (GetPrivilege() >= Privilege::employee &&
          GetImport(command, quantity, total_cost))
      {
        int id = book_stack.top();
        if (id == npos) { Invalid(); }
        else
        {
          book.Import(id, quantity, total_cost);
        }
      }
      else { Invalid(); }
    }
    else if (op == "info")
    {
      PrintInfo();
    }
    else if (op == "report")
    {
      bool is_finance;
      if (GetReport(command, is_finance))
      {
        if (is_finance) { finance_report.Print(); }
      }
    }
    else
    {
      Invalid();
    }
  }
}

