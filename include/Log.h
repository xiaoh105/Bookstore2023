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

extern FinanceLog finance_log;

class FinanceReport;

class FinanceInfo
{
 private:
  friend class FinanceReport;
  char name[50];
  int count;
  long double cost;

 public:
  FinanceInfo() = default;
  FinanceInfo(const string &name_, int count_, long double cost_);
  friend std::ostream &operator<<(std::ostream &os, const FinanceInfo &info);
};

class FinanceReport
{
 private:
  int in, out;
  FileReader<FinanceInfo, 1> income;
  FileReader<FinanceInfo, 1> outcome;

 public:
  FinanceReport();
  ~FinanceReport();
  /// Insert a finance info, cost_ > 0 for outcome
  void Insert(const string &name_, int count_, long double cost_);
  void Print();
};

#endif //HOMEWORK5_BOOKSTORE_LOG_H
