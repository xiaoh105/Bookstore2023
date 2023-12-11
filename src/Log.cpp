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
    cout << "+ " << income << " - " << std::abs(outcome) << endl;
  }
  else if (cnt <= num)
  {
    for (int i = num - cnt + 1; i <= num; ++i)
    {
      long double tmp = info[i];
      tmp > 0 ? income += tmp : outcome += tmp;
    }
    cout << std::fixed << std::setprecision(2);
    cout << "+ " << income << " - " << std::abs(outcome) << endl;
  }
  else { Invalid(); }
}

