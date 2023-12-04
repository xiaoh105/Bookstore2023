#ifndef HOMEWORK5_BOOKSTORE_BOOKSYSTEM_H
#define HOMEWORK5_BOOKSTORE_BOOKSYSTEM_H

#include <string>
#include <FileMap.h>
#include <FileReader.h>

using std::string;
using std::ostream;

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
  BookInfo() = default;
  explicit BookInfo(const string &code_);
  friend bool operator < (const BookInfo &, const BookInfo &);
  friend bool operator != (const BookInfo &, const BookInfo &);
  friend bool operator == (const BookInfo &, const BookInfo &);
  friend ostream operator << (ostream &, const BookInfo &);
};

class BookSystem
{
 private:
  int num;
  FileReader<BookInfo, 1> book;
  FileMap<> code_map, name_map, author_map, keyword_map;
 public:
  BookSystem();
  ~BookSystem();
  void Show();
  void ShowCode(const string &code);
  void ShowName(const string &name);
  void ShowAuthor(const string &author);
  void ShowKeyword(const string &keyword);
  int Find(const string &code);
  
};


#endif //HOMEWORK5_BOOKSTORE_BOOKSYSTEM_H
