#ifndef HOMEWORK5_BOOKSTORE_USERSYSTEM_H
#define HOMEWORK5_BOOKSTORE_USERSYSTEM_H

#include <string>
#include "FileReader.h"
#include "FileMap.h"
#include "Log.h"

using std::string;

extern EmployeeReport employee_report;

class UserSystem;

enum class Privilege { tourist = 0, customer = 1, employee = 3, root = 7 };

class UserInfo
{
 private:
  static const int name_len = 30 + 5;
  static const int password_len = 30 + 5;
  friend class UserSystem;
  char userid[name_len], password[name_len];
  char username[name_len];
  Privilege privilege;

 public:
  UserInfo() = default;
  UserInfo(const string &userid_, const string &password_,
           const string &username_, int privilege_);
  UserInfo(const string &userid_, const string &password_,
           const string &username_, Privilege privilege_);
};

class UserSystem
{
 private:
  const static int npos = -1;
  int num;
  FileReader<UserInfo, 1> user;
  FileMap id_map;
  int Find(const string &userid);
  void Insert(const string &userid, const string &password,
              Privilege privilege, const string &username);

 public:
  UserSystem();
  ~UserSystem();
  void Init();
  bool Login(const string &userid, const string &password, Privilege privilege,
             Privilege &cur_privilege, string &username);
  bool Register(const string &userid, const string &password,
                const string &username);
  bool Passwd(const string &userid, Privilege privilege,
              const string &cur_password, const string &new_password);
  void UserAdd(const string &userid, const string &password,
               int privilege, const string &username, Privilege cur_privilege);
  void Delete(const string &userid);
};

#endif //HOMEWORK5_BOOKSTORE_USERSYSTEM_H
