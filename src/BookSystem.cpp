#include <cstring>
#include <iostream>
#include <iomanip>
#include "BookSystem.h"
#include "Utils.h"

using std::cout, std::endl;

BookInfo::BookInfo(const string &code_):
code(), name(), author(), keyword(), stock(), price()
{
  for (int i = 0; i < code_.size(); ++i)
    code[i] = code_[i];
  code[code_.size()] = '\0';
}

bool operator < (const BookInfo &x, const BookInfo &y)
{
  return std::strcmp(x.code, y.code) < 0;
}

bool operator == (const BookInfo &x, const BookInfo &y)
{
  return std::strcmp(x.code, y.code) == 0;
}

bool operator != (const BookInfo &x, const BookInfo &y)
{
  return std::strcmp(x.code, y.code) != 0;
}

ostream operator << (ostream &out, const BookInfo &val)
{
  out << std::fixed << std::setprecision(2);
  out << val.code << "\t" << val.name << "\t" << val.author << "\t"
  << val.keyword << "\t" << val.price << "\t" << val.stock << endl;
}

BookSystem::BookSystem():
book("BookInfo"), code_map("BookCode"), name_map("BookName"),
author_map("BookAuthor"), keyword_map("BookKeyword"), num()
{
  book.GetInfo(1, num);
}

BookSystem::~BookSystem()
{
  book.WriteInfo(1, num);
}

void BookSystem::Show()
{
  vector <BookInfo> ret;
  for (int i = 1; i <= num; ++i)
  {
    BookInfo tmp{};
    book.Get(i, tmp);
    ret.push_back(tmp);
  }
  for (const auto &i: ret) cout << i;
  if (ret.empty()) cout << endl;
}

void BookSystem::ShowCode(const string &code)
{
  unsigned long long index = GetHash(code);
  auto ret = code_map.Find(index);
  for (const auto &i: ret)
  {
    cout << book[i];
  }
  if (ret.empty()) cout << endl;
}

void BookSystem::ShowAuthor(const std::string &author)
{
  unsigned long long index = GetHash(author);
  auto ret = author_map.Find(index);
  for (const auto &i: ret)
  {
    cout << book[i];
  }
  if (ret.empty()) cout << endl;
}

void BookSystem::ShowKeyword(const std::string &keyword)
{
  unsigned long long index = GetHash(keyword);
  auto ret = keyword_map.Find(index);
  for (const auto &i: ret)
  {
    cout << book[i];
  }
  if (ret.empty()) cout << endl;
}

void BookSystem::ShowName(const std::string &name)
{
  unsigned long long index = GetHash(name);
  auto ret = name_map.Find(index);
  for (const auto &i: ret)
  {
    cout << book[i];
  }
  if (ret.empty()) cout << endl;
}

int BookSystem::Find(const string &code)
{
  unsigned long long index = GetHash(code);
  auto ret = name_map.Find(index);
  if (ret.empty())
  {
    int id = book.AskId();
    BookInfo new_book(code);
    book.Write(id, new_book);
    return id;
  }
  else { return ret[0]; }
}
