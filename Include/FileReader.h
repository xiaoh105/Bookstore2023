#ifndef BOOKSTORE_FILEREADER_H
#define BOOKSTORE_FILEREADER_H

#include <fstream>
#include <vector>

using std::fstream;
using std::string;
using std::vector;

template <class T, int info_len = 1>
class FileReader
{
 private:
  const static int block_size = 4096;
  const static int size = sizeof(T);
  const static int val_per_block = block_size / size;
  const static int block_per_val = size / block_size;
  int n;
  int info[info_len + 1];
  fstream file, file_info;
  string file_name, file_info_name;
  vector<int> empty_index;
  /// Get the index of the id-th element, 1-based.
  int (*GetPos)(int id);
  static int GetPosBig(int);
  static int GetPosSmall(int);
  void CreateFile();

 public:
  FileReader(string file_name_);
  ~FileReader();
  void GetInfo(int id, int &val);
  void WriteInfo(int id, int val);
  void Get(int, T &);
  void Write(int, T &);
  void Del(int);
  int AskId();
};

template <class T, int info_len>
void FileReader<T, info_len>::CreateFile()
{
  file.open(file_name, std::ios::out);
  file.close();
  file.open(file_name);
}

template <class T, int info_len>
FileReader<T, info_len>::FileReader(string file_name_):info()
{
  file_name = std::move(file_name_);
  file_info_name = file_name + ".info";
  file.open(file_name);
  file_info.open(file_info_name);
  if (!file) { CreateFile(); }
  else
  {
    file.seekg(0);
    for (int i = 1; i <= info_len; ++i)
    {
      file.read(reinterpret_cast<char *>(&info[i]), sizeof(int));
    }
    file_info.seekg(0);
    int num;
    file_info.read(reinterpret_cast<char *>(&num), sizeof(int));
    for (int i = 1; i <= num; ++i)
    {
      int tmp;
      file_info.read(reinterpret_cast<char *>(&tmp), sizeof(int));
      empty_index.push_back(tmp);
    }
  }
  file_info.close();
  if (size > block_size) { GetPos = GetPosBig; }
  else { GetPos = GetPosSmall; }
  n = 0;
}

template <class T, int info_len>
int FileReader<T, info_len>::GetPosSmall(int id)
{
  int block = (id - 1) / val_per_block + 1;
  int pos = (id - 1) % val_per_block;
  return block * block_size + pos * size;
}

template <class T, int info_len>
int FileReader<T, info_len>::GetPosBig(int id)
{
  int block = (id - 1) * block_per_val + 1;
  return block * block_size;
}

template <class T, int info_len>
void FileReader<T, info_len>::GetInfo(int id, int &val)
{
  val = info[id];
}

template <class T, int info_len>
void FileReader<T, info_len>::WriteInfo(int id, int val)
{
  info[id] = val;
}

template <class T, int info_len>
void FileReader<T, info_len>::Get(int id, T &val)
{
  file.seekg(GetPos(id));
  file.read(reinterpret_cast<char *>(&val), size);
}

template <class T, int info_len>
void FileReader<T, info_len>::Write(int id, T &val)
{
  file.seekp(GetPos(id));
  file.write(reinterpret_cast<char *>(&val), size);
}

template <class T, int info_len>
void FileReader<T, info_len>::Del(int id)
{
  empty_index.push_back(id);
}

template <class T, int info_len>
int FileReader<T, info_len>::AskId()
{
  if (!empty_index.empty())
  {
    int ret = empty_index.back();
    empty_index.pop_back();
    return ret;
  }
  else { return ++n; }
}

template <class T, int info_len>
FileReader<T, info_len>::~FileReader()
{
  file_info.open(file_info_name);
  file.seekp(0);
  for (int i = 1; i <= info_len; ++i)
  {
    file.write(reinterpret_cast<char *>(&info[i]), sizeof(int));
  }
  file_info.seekp(0);
  int num = empty_index.size();
  file_info.write(reinterpret_cast<char *>(&num), sizeof(int));
  for (int i = 0; i < num; ++i)
  {
    file_info.write(reinterpret_cast<char *>(&empty_index[i]), sizeof(int));
  }
  file.close(), file_info.close();
}

#endif //BOOKSTORE_FILEREADER_H
