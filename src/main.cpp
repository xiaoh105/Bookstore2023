#include <iostream>
#include <FileReader.h>

using std::cin, std::cout, std::endl;

class Str
{
public:
  char ch[10];
  Str()
  {
    for (int i = 0; i < 10; ++i) ch[i] = rand() % 26 + 'a';
  }
  friend bool operator < (Str a, Str b)
  {
    for (int i = 0; i < 10; ++i)
      if (a.ch[i] != b.ch[i]) return a.ch[i] < b.ch[i];
    return false;
  }
  friend std::ostream &operator << (std::ostream &out, Str a)
  {
    for (int i = 0; i < 10; ++i) out << a.ch[i];
    out << endl;
  }
};

Str a[100];

int main()
{
  FileReader<Str, 0> file("FuckBookstore");
  /*for (int i = 1; i <= 90; ++i) cout << a[i];
  for (int i = 1; i <= 90; ++i)
  {
    file.AskId();
    file.Write(i, a[i]);
  }*/
  for (int i = 1; i <= 90; ++i) file.Get(i, a[i]);
  for (int i = 1; i <= 90; ++i)
    for (int j = 1; j <= 89; ++j)
    {
      Str u, v;
      file.Get(j, u), file.Get(j + 1, v);
      if (u < v)
      {
        file.Write(j, v), file.Write(j + 1, u);
      }
    }
  for (int i = 1; i <= 90; ++i)
  {
    Str tmp;
    file.Get(i, tmp);
    cout << tmp;
  }
}