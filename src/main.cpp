#include <iostream>
#include <string>
#include <cassert>
#include "FileMap.h"
#include "Utils.h"

using std::cin, std::cout, std::endl;
using std::string;

int main()
{
  std::ios::sync_with_stdio(false);
  FileMap mp("MemoryTest");
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