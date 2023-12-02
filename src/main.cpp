#include <iostream>
#include <cassert>
#include "FileMap.h"
#include "Utils.h"

using std::cin, std::cout, std::endl;
using std::string;

int main()
{
  std::ios::sync_with_stdio(false);
  FileMap mp("FuckBookStore");
  int n;
  cin >> n;
  for (int i = 1; i <= n; ++i) {
    std::string op, index;
    int value;
    cin >> op >> index;
    if (op == "insert")
    {
      cin >> value;
      mp.Insert(Key(GetHash(index), value));
    }
    else if (op == "delete")
    {
      cin >> value;
      mp.Remove(Key(GetHash(index), value));
    }
    else if (op == "find")
    {
      auto ret = mp.Find(GetHash(index));
      if (ret.empty())
      {
        cout << "null" << endl;
      }
      else
      {
        for (const auto &val: ret) cout << val << " ";
        cout << endl;
      }
    }
    else assert(false);
  }
}
/*
13
insert aaa 1
insert aaa 2
find aaa
insert aaa 3
delete aaa 2
insert aaa 4
find aaa
insert aaa 5
find aaa
delete aaa 5
insert aaa 5
insert aaa 6
find aaa
 */