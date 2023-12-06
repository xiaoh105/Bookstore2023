#include <iostream>
#include "InstructionValidator.h"
#include "Executor.h"
#include "Utils.h"
#include "BookSystem.h"
#include "UserSystem.h"

using std::cin;

BookSystem book;
UserSystem user;

Executor::Executor():name_stack(), book_stack(), id_map(), id_stack()
{
  p_stack.push(Privilege::tourist);
}

Privilege Executor::GetPrivilege()
{
  return p_stack.top();
}

void Executor::listen()
{
  bool flag = true;
  while (flag)
  {
    string command;
    getline(cin, command);
    string op = GetWord(command);
    if (command.empty())
    {
      continue;
    }
    else if (command == "quit" || command == "exit")
    {
      if (GetQuit(command)) { flag = false; }
      else { Invalid(); }
    }
    else if (command == "su")
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
          book_stack.push(npos);
          id_stack.push(userid);
          ++id_map[userid];
        }
      }
      else { Invalid(); }
    }
    else if (command == "logout")
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
        }
      }
      else { Invalid(); }
    }
    else if (command == "register")
    {
      string userid, password, username;
      if (GetRegister(command, userid, password, username))
      {
        user.Register(userid, password, username);
      }
      else { Invalid(); }
    }
    else if (command == "passwd")
    {
      string userid, cur_password, new_password;
      if (GetPassword(command, userid, cur_password, new_password))
      {
        user.Passwd(userid, GetPrivilege(), cur_password, new_password);
      }
      else { Invalid(); }
    }
    else if (command == "useradd")
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
    else if (command == "delete")
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
  }
}

