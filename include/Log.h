#ifndef HOMEWORK5_BOOKSTORE_LOG_H
#define HOMEWORK5_BOOKSTORE_LOG_H

#include "FileReader.h"

class FinanceLog
{
 private:
  const static int size = 200;
  int num;
  FileReader<long double, 1> info;

 public:
  FinanceLog();
  ~FinanceLog();
  void Insert(long double val);
  void Print(int cnt);
};


#endif //HOMEWORK5_BOOKSTORE_LOG_H
