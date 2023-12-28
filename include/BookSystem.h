#ifndef HOMEWORK5_BOOKSTORE_BOOKSYSTEM_H
#define HOMEWORK5_BOOKSTORE_BOOKSYSTEM_H

#include <string>
#include "FileMap.h"
#include "FileReader.h"
#include "Log.h"

using std::string;
using std::ostream;

extern FinanceReport finance_report;

class BookSystem;

class BookInfo
{
 private:
  friend class BookSystem;
  const static int code_len = 20 + 5;
  const static int name_len = 60 + 5;
  char code[code_len];
  char name[name_len], author[name_len], keyword[name_len];
  long long stock;
  long double price;
 public:
  BookInfo(): code{}, name{}, author{}, keyword{}, stock(), price(){};
  explicit BookInfo(const string &code_);
  friend bool operator < (const BookInfo &, const BookInfo &);
  friend bool operator != (const BookInfo &, const BookInfo &);
  friend bool operator == (const BookInfo &, const BookInfo &);
  friend ostream &operator << (ostream &, const BookInfo &);
};

class BookSystem
{
 private:
  int num;
  FileReader<BookInfo, 1> book;
  FileMap code_map, name_map, author_map, keyword_map;
  int Find(const string &code);

 public:
  static const int npos = -1;
  BookSystem();
  ~BookSystem();
  int Create(const string &code);
  int Select(const string &code);
  void Show(const string &code, const string &name,
            const string &author, const string &keyword);
  void Buy(const string &code, int quantity);
  void Modify(int id, const string &ISBN, const string &name,
              const string &author, const string &keyword, long double price);
  void Import(int id, int quantity, long double total_cost);
};


#endif //HOMEWORK5_BOOKSTORE_BOOKSYSTEM_H
