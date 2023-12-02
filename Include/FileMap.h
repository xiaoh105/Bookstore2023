#ifndef HOMEWORK5_BOOKSTORE_FILEMAP_H
#define HOMEWORK5_BOOKSTORE_FILEMAP_H

#include <vector>
#include "FileReader.h"

using std::vector;
using std::string;

class Node;
class FileMap;

class Key
{
 private:
  friend class Node;
  friend class FileMap;
  unsigned long long index;
  int value;

 public:
  Key() = default;
  Key(unsigned long long index_, int value_): index(index_), value(value_){}
  friend bool operator < (const Key &, const Key &);
  friend bool operator != (const Key &, const Key &);
};

class Node
{
 private:
  constexpr static int M = 200;
  friend class FileMap;
  Key key[M]{};
  int ptr[M + 1]{}, size{};
  bool is_leaf = false;

 public:
  Node() = default;
  int lower_bound(const Key &val) const;
  int upper_bound(const Key &val) const;
};

class FileMap
{
 private:
  constexpr static int M = 200;
  string file_name;
  FileReader<Node, 1> file;
  unordered_map<int, int> parent;
  void InsertInternal(const Key &key, int cur, int ch);
  void RemoveInternal(const Key &key, int cur, int ch);
  void UpdateParent(const Key &key, int cur);

 public:
  int root;
  FileMap() = delete;
  explicit FileMap(string);
  ~FileMap();
  void Insert(const Key &key);
  void Remove(const Key &key);
  vector<int> Find(unsigned long long index);
  void Print(int cur);
};

#endif //HOMEWORK5_BOOKSTORE_FILEMAP_H
