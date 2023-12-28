#ifndef HOMEWORK5_BOOKSTORE_LOG_H
#define HOMEWORK5_BOOKSTORE_LOG_H

#include "FileReader.h"
#include "FileMap.h"

class FinanceLog
{
 private:
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
  char user[50];
  int count;
  long double cost;

 public:
  FinanceInfo() = default;
  FinanceInfo(const string &name_, int count_, long double cost_, const string &user_);
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
  void Insert(const string &name_, int count_, long double cost_, const string &user);
  void Print();
};

class EmployeeReport;

class EmployeeInfo
{
private:
  friend class EmployeeReport;
  char name[50];
  int modify;
  int import;
  long double cost;
public:
  EmployeeInfo() = default;
  explicit EmployeeInfo(const string &name_);
  friend std::ostream &operator<<(std::ostream &os, const EmployeeInfo &info);
};

class EmployeeReport
{
 private:
  int num;
  FileReader<EmployeeInfo, 1> info;
  FileMap mp;

 public:
  EmployeeReport();
  ~EmployeeReport();
  void Insert(const string &name);
  void RecordModify(const string &name);
  void RecordImport(const string &name, long double cost);
  void Print();
};

#endif //HOMEWORK5_BOOKSTORE_LOG_H
