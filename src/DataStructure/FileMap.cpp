#include <algorithm>
#include "FileMap.h"

using std::string;

bool operator < (const Key &x, const Key &y)
{
  return x.index != y.index? x.index < y.index:x.value < y.value;
}

bool operator != (const Key &x, const Key &y)
{
  return x.index != y.index || x.value != y.value;
}

int Node::lower_bound(const Key &val) const
{
  return static_cast<int>(std::lower_bound(key, key + size, val) - key);
}

int Node::upper_bound(const Key &val) const
{
  return static_cast<int>(std::upper_bound(key, key + size, val) - key);
}

FileMap::FileMap(string name): file(std::move(name)), root()
{
  file.GetInfo(1, root);
}

FileMap::~FileMap()
{
  file.WriteInfo(1, root);
}

void FileMap::Insert(const Key &key)
{
  if (!root)
  {
    root = file.AskId();
    Node rt;
    rt.is_leaf = true;
    rt.key[0] = key;
    rt.size = 1;
    file.Write(root, rt);
    return;
  }
  int cur = root;
  Node cur_node;
  file.Get(cur, cur_node);
  parent.clear();
  while (!cur_node.is_leaf)
  {
    int tmp = cur;
    cur = cur_node.ptr[cur_node.lower_bound(key)];
    parent[cur] = tmp;
    file.Get(cur, cur_node);
  }
  int pos = cur_node.lower_bound(key);
  if (cur_node.size < M)
  {
    for (int i = cur_node.size - 1; i >= pos; --i)
      cur_node.key[i + 1] = cur_node.key[i];
    cur_node.key[pos] = key;
    cur_node.ptr[cur_node.size + 1] = cur_node.ptr[cur_node.size];
    cur_node.ptr[cur_node.size] = 0;
    ++cur_node.size;
    file.Write(cur, cur_node);
    return;
  }
  Key tmp[M + 1];
  for (int i = 0; i < pos; ++i) tmp[i] = cur_node.key[i];
  tmp[pos] = key;
  for (int i = pos; i < cur_node.size; ++i) tmp[i + 1] = cur_node.key[i];
  int new_id = file.AskId();
  Node new_node;
  new_node.is_leaf = true;
  cur_node.size = (M + 1) >> 1;
  new_node.size = (M + 1) - cur_node.size;
  for (int i = 0; i < new_node.size; ++i)
    new_node.key[i] = tmp[cur_node.size + i];
  if (pos < cur_node.size)
    for (int i = pos; i < cur_node.size; ++i) cur_node.key[i] = tmp[i];
  cur_node.ptr[cur_node.size] = new_id;
  new_node.ptr[new_node.size] = cur_node.ptr[M];
  cur_node.ptr[M] = 0;
  file.Write(cur, cur_node);
  file.Write(new_id, new_node);
  if (cur == root)
  {
    int new_root_id = file.AskId();
    Node new_root;
    new_root.size = 1;
    new_root.key[0] = new_node.key[0];
    new_root.ptr[0] = cur, new_root.ptr[1] = new_id;
    root = new_root_id;
    file.Write(new_root_id, new_root);
  }
  else
  {
    InsertInternal(new_node.key[0], parent[cur], new_id);
  }
}

void FileMap::InsertInternal(const Key &key, int cur, int ch)
{
  Node cur_node;
  file.Get(cur, cur_node);
  int pos = cur_node.lower_bound(key);
  if (cur_node.size < M)
  {
    for (int i = cur_node.size - 1; i >= pos; --i)
      cur_node.key[i + 1] = cur_node.key[i];
    cur_node.key[pos] = key;
    for (int i = cur_node.size; i > pos; --i)
      cur_node.ptr[i + 1] = cur_node.ptr[i];
    cur_node.ptr[pos + 1] = ch;
    ++cur_node.size;
    file.Write(cur, cur_node);
    return;
  }
  Key tmp_key[M + 1];
  int tmp_ptr[M + 2];
  for (int i = 0; i < pos; ++i) tmp_key[i] = cur_node.key[i];
  tmp_key[pos] = key;
  for (int i = pos; i < cur_node.size; ++i) tmp_key[i + 1] = cur_node.key[i];
  for (int i = 0; i <= pos; ++i) tmp_ptr[i] = cur_node.ptr[i];
  tmp_ptr[pos + 1] = ch;
  for (int i = pos + 1; i <= cur_node.size; ++i)
    tmp_ptr[i + 1] = cur_node.ptr[i];
  int new_id = file.AskId();
  Node new_node;
  cur_node.size = (M + 1) >> 1;
  new_node.size = M - cur_node.size;
  if (pos < cur_node.size)
  {
    for (int i = pos; i < cur_node.size; ++i)
      cur_node.key[i] = tmp_key[i];
    for (int i = pos + 1; i <= cur_node.size; ++i)
      cur_node.ptr[i] = tmp_ptr[i];
  }
  for (int i = 0; i < new_node.size; ++i)
    new_node.key[i] = tmp_key[i + 1 + cur_node.size];
  for (int i = 0; i <= new_node.size; ++i)
    new_node.ptr[i] = tmp_ptr[i + 1 + cur_node.size];
  file.Write(cur, cur_node);
  file.Write(new_id, new_node);
  if (cur == root)
  {
    int new_root_id = file.AskId();
    Node new_root;
    new_root.size = 1;
    new_root.key[0] = tmp_key[cur_node.size];
    new_root.ptr[0] = cur, new_root.ptr[1] = new_id;
    root = new_root_id;
    file.Write(new_root_id, new_root);
  }
  else
  {
    InsertInternal(tmp_key[cur_node.size], parent[cur], cur);
  }
}

void FileMap::UpdateParent(const Key &key, int cur)
{
  int fa = parent[cur];
  while (cur != root)
  {
    Node parent_node;
    file.Get(fa, parent_node);
    int pos;
    for (int i = 0; i <= parent_node.size; ++i)
      if (parent_node.ptr[i] == cur)
      {
        pos = i;
        break;
      }
    if (pos)
    {
      parent_node.key[pos - 1] = key;
      file.Write(fa, parent_node);
      return;
    }
    cur = fa, fa = parent[cur];
    file.Get(fa, parent_node);
  }
}

void FileMap::Remove(const Key &key)
{
  if (!root) return;
  int cur = root;
  Node cur_node;
  file.Get(cur, cur_node);
  parent.clear();
  int lp, rp, ls, rs;
  while (!cur_node.is_leaf)
  {
    int tmp = cur;
    int pos = cur_node.upper_bound(key);
    lp = pos - 1;
    rp = (pos == cur_node.size)? -1:pos + 1;
    if (lp >= 0) ls = cur_node.ptr[lp];
    if (rp >= 0) rs = cur_node.ptr[rp];
    cur = cur_node.ptr[pos];
    parent[cur] = tmp;
    file.Get(cur, cur_node);
  }
  int pos = cur_node.lower_bound(key);
  if (pos == cur_node.size || cur_node.key[pos] != key) return;
  for (int i = pos + 1; i < cur_node.size; ++i)
    cur_node.key[i - 1] = cur_node.key[i];
  cur_node.ptr[cur_node.size - 1] = cur_node.ptr[cur_node.size];
  cur_node.ptr[cur_node.size] = 0;
  --cur_node.size;
  file.Write(cur, cur_node);
  if (pos == 0) UpdateParent(cur_node.key[0], cur);
  if (cur == root) return;
  if (cur_node.size >= (M + 1) >> 1) return;
  Node left, right;
  if (lp >= 0)
  {
    file.Get(ls, left);
    if (left.size > (M + 1) >> 1)
    {

      return;
    }
  }
}