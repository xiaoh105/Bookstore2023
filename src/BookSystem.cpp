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

ostream &operator << (ostream &out, const BookInfo &val)
{
  out << std::fixed << std::setprecision(2);
  out << val.code << "\t" << val.name << "\t" << val.author << "\t"
  << val.keyword << "\t" << val.price << "\t" << val.stock << endl;
  return out;
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

void BookSystem::Show(const string &code, const string &name,
          const string &author, const string &keyword)
{
  vector<BookInfo> ret;
  vector<int> tmp;
  if (!code.empty())
  {
    tmp = code_map.Find(GetHash(code));
  }
  else if (!name.empty())
  {
    tmp = name_map.Find(GetHash(name));
  }
  else if (!author.empty())
  {
    tmp = author_map.Find(GetHash(author));
  }
  else if(!keyword.empty())
  {
    tmp = keyword_map.Find(GetHash(keyword));
  }
  else
  {
    tmp = name_map.FindAll();
  }
  for (const auto &i: tmp) ret.push_back(book[i]);
  std::sort(ret.begin(), ret.end());
  for (const auto &i: ret) cout << i;
  if (ret.empty()) cout << endl;
}

int BookSystem::Find(const string &code)
{
  auto ret = code_map.Find(GetHash(code));
  return ret.empty()? npos:ret[0];
}

int BookSystem::Select(const std::string &code)
{
  int ret = Find(code);
  return ret == npos ? Create(code) : ret;
}

void BookSystem::Buy(const string &code, int quantity)
{
  if (quantity == 0) { Invalid(); return; }
  int id = Find(code);
  if (id == npos) { Invalid(); return; }
  auto info = book[id];
  info.stock -= quantity;
  if (info.stock < 0) { Invalid(); return; }
  book.Write(id, info);
  cout << std::fixed << std::setprecision(2);
  cout << quantity * info.price << endl;
  finance_log.Insert(quantity * info.price);
}

void BookSystem::Modify(int id, const string &ISBN,
                        const string &name, const string &author,
                        const string &keyword, long double price)
{
  auto info = book[id];
  if (ISBN == info.code) { Invalid(); return; }
  if (!ISBN.empty())
  {
    if (Find(ISBN) != npos) { Invalid(); return; }
    code_map.Remove({GetHash(info.code), id});
    for (int i = 0; i < ISBN.size(); ++i)
      info.code[i] = ISBN[i];
    info.code[ISBN.size()] = 0;
    code_map.Insert({GetHash(info.code), id});
  }
  if (!name.empty())
  {
    name_map.Remove({GetHash(info.name), id});
    for (int i = 0; i < name.size(); ++i)
      info.name[i] = name[i];
    info.name[name.size()] = 0;
    name_map.Insert({GetHash(info.name), id});
  }
  if (!author.empty())
  {
    author_map.Remove({GetHash(info.author), id});
    for (int i = 0; i < author.size(); ++i)
      info.author[i] = author[i];
    info.author[author.size()] = 0;
    author_map.Insert({GetHash(info.author), id});
  }
  if (!keyword.empty())
  {
    for (const auto &i: SplitKeyword(info.keyword))
      keyword_map.Remove({GetHash(i), id});
    for (int i = 0; i < keyword.size(); ++i)
      info.keyword[i] = keyword[i];
    info.keyword[keyword.size()] = 0;
    unordered_map<string, bool> check;
    for (const auto &i: SplitKeyword(info.keyword))
    {
      if (check[i]) { Invalid(); return; }
      keyword_map.Insert({GetHash(i), id});
    }
  }
  if (price >= 0) info.price = price;
  book.Write(id, info);
}

void BookSystem::Import(int id, int quantity, long double total_cost)
{
  if (quantity == 0) { Invalid(); return; }
  if (total_cost < 1e-3) { Invalid(); return; }
  auto info = book[id];
  info.stock += quantity;
  book.Write(id, info);
  finance_log.Insert(-total_cost);
}

int BookSystem::Create(const string &code)
{
  num = book.AskId();
  BookInfo new_book(code);
  book.Write(num, new_book);
  code_map.Insert({GetHash(code), num});
  return num;
}