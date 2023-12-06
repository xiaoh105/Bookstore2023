#include <cassert>
#include "UserSystem.h"
#include "Utils.h"

UserInfo::UserInfo(const string &userid_,const string &password_,
                   const string &username_, int privilege_):
userid(), password(), username(), privilege()
{
  for (int i = 0; i < userid_.size(); ++i) userid[i] = userid_[i];
  userid[userid_.size()] = 0;
  for (int i = 0; i < password_.size(); ++i) password[i] = password_[i];
  password[password_.size()] = 0;
  for (int i = 0; i < username_.size(); ++i) username[i] = username_[i];
  username[username_.size()] = 0;
  switch (privilege_)
  {
    case 1 : privilege = Privilege::customer; break;
    case 3 : privilege = Privilege::employee; break;
    case 7 : privilege = Privilege::root; break;
    default : assert(false);
  }
}

UserInfo::UserInfo(const string &userid_,const string &password_,
                   const string &username_, Privilege privilege_):
        userid(), password(), username(), privilege(privilege_)
{
  for (int i = 0; i < userid_.size(); ++i) userid[i] = userid_[i];
  userid[userid_.size()] = 0;
  for (int i = 0; i < password_.size(); ++i) password[i] = password_[i];
  password[password_.size()] = 0;
  for (int i = 0; i < username_.size(); ++i) username[i] = username_[i];
  username[username_.size()] = 0;
}

UserSystem::UserSystem(): user("UserInfo"), id_map("UserID"), num()
{
  user.GetInfo(1, num);
  Init();
}

UserSystem::~UserSystem()
{
  user.WriteInfo(1, num);
}

int UserSystem::Find(const string &userid)
{
  auto ret = id_map.Find(GetHash(userid));
  return ret.empty() ? npos : ret[0];
}

void UserSystem::Init()
{
  if (num == 0)
  {
    Insert("root", "sjtu", Privilege::root, "root");
  }
}

bool UserSystem::Login(const string &userid, const string &password,
                       Privilege privilege, Privilege &cur_privilege,
                       string &username)
{
  int id = Find(userid);
  if (id == npos) { Invalid(); return false; }
  auto info = user[id];
  if (info.privilege < privilege)
  {
    if (!password.empty() && password != info.password) { Invalid(); return false; }
  }
  else
  {
    if (password != info.password) { Invalid(); return false; }
  }
  cur_privilege = info.privilege;
  username = info.username;
  return true;
}

void UserSystem::Register(const string &userid, const string &password,
                          const string &username)
{
  if (Find(userid) != npos) { Invalid(); return; }
  Insert(userid, password, Privilege::customer, username);
}

void UserSystem::Insert(const string &userid, const string &password,
                        Privilege privilege, const string &username)
{
  ++num;
  int id = user.AskId();
  UserInfo info(userid, password, username, privilege);
  user.Write(id, info);
  id_map.Insert({GetHash(userid), id});
}

void UserSystem::Passwd(const string &userid, Privilege privilege,
                        const string &cur_password, const string &new_password)
{
  int id = Find(userid);
  if (id == npos) { Invalid(); return; }
  auto info = user[id];
  if (privilege == Privilege::root)
  {
    if (!cur_password.empty() && cur_password != info.password)
    {
      Invalid();
      return;
    }
  }
  else
  {
    if (cur_password != info.password) { Invalid(); return; }
  }
  for (int i = 0; i < new_password.length(); ++i)
    info.password[i] = new_password[i];
  info.password[new_password.size()] = 0;
  user.Write(id, info);
}

void UserSystem::UserAdd(const string &userid, const string &password,
                         int privilege, const string &username,
                         Privilege cur_privilege)
{
  if (static_cast<Privilege>(privilege) >= cur_privilege)
  {
    Invalid();
    return;
  }
  int id = Find(userid);
  if (id == npos) {Invalid(); return; }
  Insert(userid, password, static_cast<Privilege>(privilege), username);
}

void UserSystem::Delete(const string &userid)
{
  int id = Find(userid);
  if (id == npos) { Invalid(); return; }
  user.Del(id);
  id_map.Remove({GetHash(userid), id});
}
