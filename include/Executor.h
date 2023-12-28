#ifndef HOMEWORK5_BOOKSTORE_EXECUTOR_H
#define HOMEWORK5_BOOKSTORE_EXECUTOR_H

#include <stack>
#include "BookSystem.h"
#include "UserSystem.h"
#include "Log.h"

using std::stack;
using std::string;

extern FinanceReport finance_report;

class Executor
{
 private:
  static const int npos = -1;
  stack<Privilege> p_stack;
  stack<string> name_stack;
  stack<int> book_stack;
  stack<string> id_stack;
  unordered_map<string, int> id_map;
  void PrintInfo();
 public:
  Executor();
  ~Executor() = default;
  Privilege GetPrivilege();
  string GetName();
  void listen();
};

extern Executor executor;

#endif //HOMEWORK5_BOOKSTORE_EXECUTOR_H
