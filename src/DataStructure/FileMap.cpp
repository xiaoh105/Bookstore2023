#include <algorithm>
#include <iostream>
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
    InsertInternal(tmp_key[cur_node.size], parent[cur], new_id);
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
  Node l_node, r_node, parent_node;
  file.Get(parent[cur], parent_node);
  if (lp >= 0)
  {
    file.Get(ls, l_node);
    if (l_node.size > (M + 1) >> 1)
    {
      for (int i = cur_node.size - 1; i >= 0; --i)
        cur_node.key[i + 1] = cur_node.key[i];
      cur_node.key[0] = l_node.key[l_node.size - 1];
      parent_node.key[lp] = cur_node.key[0];
      cur_node.ptr[cur_node.size + 1] = cur_node.ptr[cur_node.size];
      cur_node.ptr[cur_node.size] = 0;
      ++cur_node.size;
      l_node.ptr[l_node.size - 1] = l_node.ptr[l_node.size];
      l_node.ptr[l_node.size] = 0;
      --l_node.size;
      file.Write(cur, cur_node);
      file.Write(ls, l_node);
      file.Write(parent[cur], parent_node);
      return;
    }
  }
  else if (rp >= 0)
  {
    file.Get(rs, r_node);
    if (r_node.size > (M + 1) >> 1)
    {
      cur_node.key[cur_node.size] = r_node.key[0];
      for (int i = 1; i < r_node.size; ++i)
        r_node.key[i - 1] = r_node.key[i];
      parent_node.key[rp - 1] = r_node.key[0];
      cur_node.ptr[cur_node.size + 1] = cur_node.ptr[cur_node.size];
      cur_node.ptr[cur_node.size] = 0;
      ++cur_node.size;
      r_node.ptr[r_node.size - 1] = r_node.ptr[r_node.size];
      r_node.ptr[r_node.size] = 0;
      --r_node.size;
      file.Write(cur, cur_node);
      file.Write(rs, r_node);
      file.Write(parent[cur], parent_node);
      return;
    }
  }
  if (lp >= 0)
  {
    for (int i = 0; i < cur_node.size; ++i)
      l_node.key[i + l_node.size] = cur_node.key[i];
    l_node.ptr[l_node.size] = 0;
    l_node.size += cur_node.size;
    l_node.ptr[l_node.size] = cur_node.ptr[cur_node.size];
    file.Write(ls, l_node);
    RemoveInternal(cur_node.key[0], parent[cur], cur);
    file.Del(cur);
  }
  else
  {
    for (int i = 0; i < r_node.size; ++i)
      cur_node.key[i + cur_node.size] = r_node.key[i];
    cur_node.ptr[cur_node.size] = 0;
    cur_node.size += r_node.size;
    cur_node.ptr[cur_node.size] = r_node.ptr[r_node.size];
    file.Write(cur, cur_node);
    RemoveInternal(r_node.key[0], parent[cur], rs);
    file.Del(rs);
  }
}

void FileMap::RemoveInternal(const Key &key, int cur, int ch)
{
  Node cur_node;
  file.Get(cur, cur_node);
  if (cur == root && cur_node.size == 1)
  {
    if (cur_node.ptr[0] == ch)
    {
      root = cur_node.ptr[1];
      file.Del(cur);
    }
    else
    {
      root = cur_node.ptr[0];
      file.Del(cur);
    }
    return;
  }
  int pos = cur_node.lower_bound(key);
  for (int i = pos + 1; i < cur_node.size; ++i)
    cur_node.key[i - 1] = cur_node.key[i];
  for (int i = pos + 2; i <= cur_node.size; ++i)
    cur_node.ptr[i - 1] = cur_node.ptr[i];
  --cur_node.size;
  file.Write(cur, cur_node);
  if (cur_node.size >= M >> 1) return;
  if (cur == root) return;
  int ls, rs, lp, rp;
  Node parent_node;
  file.Get(parent[cur], parent_node);
  for (int i = 0; i <= parent_node.size; ++i)
    if (parent_node.ptr[i] == cur)
    {
      lp = i - 1;
      rp = (i == parent_node.size)? -1:i + 1;
      if (lp >= 0) ls = parent_node.ptr[lp];
      if (rp >= 0) rs = parent_node.ptr[rp];
    }
  Node left_node, right_node;
  if (lp >= 0)
  {
    file.Get(ls, left_node);
    if (left_node.size > M >> 1)
    {
      for (int i = cur_node.size - 1; i >= 0; --i)
        cur_node.key[i + 1] = cur_node.key[i];
      for (int i = cur_node.size; i >= 0; --i)
        cur_node.ptr[i + 1] = cur_node.ptr[i];
      cur_node.key[0] = parent_node.key[lp];
      parent_node.key[lp] = left_node.key[left_node.size - 1];
      cur_node.ptr[0] = left_node.ptr[left_node.size];
      ++cur_node.size, --left_node.size;
      file.Write(cur, cur_node);
      file.Write(ls, left_node);
      file.Write(parent[cur], parent_node);
      return;
    }
  }
  else if (rp >= 0)
  {
    file.Get(rs, right_node);
    if (right_node.size > M >> 1)
    {
      cur_node.ptr[cur_node.size + 1] = right_node.ptr[0];
      cur_node.key[cur_node.size] = parent_node.key[rp - 1];
      parent_node.key[rp - 1] = right_node.key[0];
      for (int i = 1; i < right_node.size; ++i)
        right_node.key[i - 1] = right_node.key[i];
      for (int i = 1; i <= right_node.size; ++i)
        right_node.ptr[i - 1] = right_node.ptr[i];
      ++cur_node.size, --right_node.size;
      file.Write(cur, cur_node);
      file.Write(rs, right_node);
      file.Write(parent[cur], parent_node);
      return;
    }
  }
  if (lp >= 0)
  {
    for (int i = 0; i < cur_node.size; ++i)
      left_node.key[i + left_node.size + 1] = cur_node.key[i];
    for (int i = 0; i <= cur_node.size; ++i)
      left_node.ptr[i + left_node.size + 1] = cur_node.ptr[i];
    left_node.key[left_node.size] = parent_node.key[lp];
    left_node.size += cur_node.size + 1;
    file.Write(ls, left_node);
    RemoveInternal(parent_node.key[lp], parent[cur], cur);
    file.Del(cur);
  }
  else
  {
    for (int i = 0; i < right_node.size; ++i)
      cur_node.key[i + cur_node.size + 1] = right_node.key[i];
    for (int i = 0; i <= right_node.size; ++i)
      cur_node.ptr[i + cur_node.size + 1] = right_node.ptr[i];
    cur_node.key[cur_node.size] = parent_node.key[rp - 1];
    cur_node.size += right_node.size + 1;
    file.Write(cur, cur_node);
    RemoveInternal(parent_node.key[rp - 1], parent[cur], rs);
    file.Del(rs);
  }
}

vector<int> FileMap::Find(unsigned long long index)
{
  vector<int> ret;
  Key key(index, INT32_MIN);
  if (!root) return ret;
  int cur = root;
  Node cur_node;
  file.Get(cur, cur_node);
  while (!cur_node.is_leaf)
  {
    int pos = cur_node.upper_bound(key);
    cur = cur_node.ptr[pos];
    file.Get(cur, cur_node);
  }
  int pos = cur_node.lower_bound(key);
  bool flag = true;
  while (flag)
  {
    for (int i = pos; i < cur_node.size; ++i)
      if (cur_node.key[i].index == index)
      {
        ret.push_back(cur_node.key[i].value);
      }
      else
      {
        flag = false;
        break;
      }
    if (flag)
    {
      pos = 0;
      int next_node = cur_node.ptr[cur_node.size];
      if (next_node)
      {
        file.Get(next_node, cur_node);
      }
      else
      {
        flag = false;
      }
    }
  }
  return ret;
}

void FileMap::Print(int cur)
{
  Node cur_node;
  file.Get(cur, cur_node);
  std::cout << "Node #" << cur << std::endl;
  if (cur_node.is_leaf) std::cout << "This is a leaf." << std::endl;
  for (int i = 0; i < cur_node.size; ++i)
    std::cout << "(" <<  cur_node.key[i].index << "," << cur_node.key[i].value << ") " << " ";
  std::cout << std::endl;
  for (int i = 0; i <= cur_node.size; ++i)
    std::cout << cur_node.ptr[i] << " ";
  std::cout << std::endl;
  if (!cur_node.is_leaf)
  {
    for (int i = 0; i <= cur_node.size; ++i) Print(cur_node.ptr[i]);
  }
}