#include <iostream>
#include <iomanip>
#include "Utils.h"
#include "Log.h"
using std::cout, std::endl;

FinanceLog::FinanceLog():num(), info("FinanceLog")
{
  info.GetInfo(1, num);
}

FinanceLog::~FinanceLog()
{
  info.WriteInfo(1, num);
}

void FinanceLog::Insert(long double val)
{
  num = info.AskId();
  info.Write(num, val);
}

void FinanceLog::Print(int cnt)
{
  long double income = 0.0, outcome = 0.0;
  if (cnt == 0)
  {
    cout << endl;
  }
  else if (cnt == -1)
  {
    for (int i = 1; i <= num; ++i)
    {
      long double tmp = info[i];
      tmp > 0 ? income += tmp : outcome += tmp;
    }
    cout << std::fixed << std::setprecision(2);
    cout << income << " " << std::abs(outcome) << endl;
  }
  else if (cnt <= num)
  {
    for (int i = num - cnt + 1; i <= num; ++i)
    {
      long double tmp = info[i];
      tmp > 0 ? income += tmp : outcome += tmp;
    }
    cout << std::fixed << std::setprecision(2);
    cout << income << " " << std::abs(outcome) << endl;
  }
  else { Invalid(); }
}

FinanceInfo::FinanceInfo(const string &name_, int count_, long double cost_):
name(), count(count_), cost(cost_)
{
  for (int i = 0; i < name_.size(); ++i) name[i] = name_[i];
  name[name_.size()] = '\0';
}

std::ostream &operator<<(std::ostream &os, const FinanceInfo &info)
{
  os << info.name << " " << info.count << " " << info.cost;
  return os;
}

FinanceReport::FinanceReport():
in(), out(), income("ReportIncome"), outcome("ReportOutcome")
{
  income.GetInfo(1, in);
  outcome.GetInfo(2, out);
}

FinanceReport::~FinanceReport()
{
  income.WriteInfo(1, in);
  outcome.WriteInfo(2, out);
}

void FinanceReport::Insert(const string &name_, int count_, long double cost_)
{
  if (cost_ > 0)
  {
    out = outcome.AskId();
    outcome.Write(out, FinanceInfo(name_, count_, cost_));
  }
  else
  {
    in = income.AskId();
    income.Write(in, FinanceInfo(name_, count_, std::abs(cost_)));
  }
}

void FinanceReport::Print()
{
  finance_log.Print(-1);
  cout << in << endl;
  for (int i = 1; i <= in; ++i)
  {
    auto tmp = income[i];
    cout << tmp << endl;
  }
  cout << out << endl;
  for (int i = 1; i <= out; ++i)
  {
    auto tmp = outcome[i];
    cout << tmp << endl;
  }
}
