## 书店管理系统

#### By xiaoh

### 程序功能

用可视化的形式进行书店的书籍管理和用户管理。



### 开发平台

后端由C++编写，进行数据库和书籍、用户信息的管理。由Python的flask库进行与前端的沟通与部分可变内容的生成。

前端由html语言，基于bootstrap5进行编写。所用的模板引擎为Jinja2。



### 代码文件结构

本项目的文件结构如下图所示：

```tree
.
├── CMakeLists.txt
├── html
│   └── templates
│       ├── add_book.html
│       ├── buy.html
│       ├── employee.html
│       ├── finance.html
│       ├── import.html
│       ├── log.html
│       ├── login_main.html
│       ├── main.html
│       ├── modify.html
│       ├── search.html
│       └── user.html
├── include
│   ├── BookSystem.h
│   ├── Executor.h
│   ├── FileMap.h
│   ├── FileReader.h
│   ├── InstructionValidator.h
│   ├── Log.h
│   ├── UserSystem.h
│   └── Utils.h
└── src
    ├── backend
    │   ├── BookSystem.cpp
    │   ├── Executor.cpp
    │   ├── FileMap.cpp
    │   ├── InstructionValidator.cpp
    │   ├── Log.cpp
    │   ├── UserSystem.cpp
    │   ├── Utils.cpp
    │   └── main.cpp
    └── frontend
        └── main.py
```



``html/``目录下为项目的前端网页模板，``html/templates/``为网页模板，``html/static/``为bootstrap5的``.javascript``和`.css`文件的存放处。

`include/`下为项目所需的头文件，主要进行项目所需的类与函数的定义。

`src/`下为项目后端的源代码，其中`src/backend/`下为项目C++后端的源代码，`src/frontend/`下为项目通过Python进行前后端沟通的部分。



### 接口与实现逻辑

#### `FileReader`

`FileReader<T, info_len>`为实现文件读写的模板类，其中`T`为需要读写的类型，`info_len`为读写需要附带的整形信息的长度。

##### 实现逻辑

基于`fstream`进行文件的二进制读写部分。有以下两点需要注意的部分：

1.为加快文件读写的速度，类内进行了针对读写的优化。第一个$4KB$的块存储所有的整形信息。若`T`的大小小于$4KB$，则后续每一个块存储尽可能多的`T`类型变量，但需保证不存在超出$4KB$边界的变量；若`T`的大小大于等于$4KB$，则每次将一个`T`类型变量与$4KB$的块首对齐。

2.为减少文件读写的数量，类内加入了一个滚动缓存数组`cache`，缓存数组的大小至多是50个$4KB$的块，且至多有100个变量。每次读/写一个新元素时，将其载入缓存，并优先从缓存中读取。最新加入缓存的变量将覆盖最早加入的变量。在一个缓存被覆盖前，将被重新写入内存。同时，所有的整形信息都将存在缓存中，类构造时读入，析构时写出。

3.关于变量删除：删除时采用懒惰删除，每次删除时不擦除文件中的数据，而是将其下标记录在一个`.info`文件中，在类构造时从文件中读取空余索引并优先在写入新数据时分配被擦除的索引，类析构时将未被使用的索引写入文件。

##### 接口及成员说明

```c++
template <class T, int info_len = 1>
class FileReader
{
 private:
  // 文件系统中块的大小
  const static int block_size = 4096;
  // 类型的大小
  const static int size = sizeof(T);
  // 每个块中存多少个变量，用于size < 4KB的情况
  const static int val_per_block = block_size / size;
  // 每个变量存多少个块，用于size >= 4KB的情况
  const static int block_per_val = (size - 1) / block_size + 1;
  // 缓存的最大大小
  const static int max_cache_size = 50 * block_size, max_element = 100;
  // 缓存的大小
  const static int cache_size = min(max_element, max_cache_size / size);
  // 数字的数量
  int n;
  // 存储整形信息
  int info[info_len + 1];
  // 滚动数组的指针，用于指示覆写位置
  int ptr = 0;
  // 滚动数组的每个变量在文件中的索引
  int index[cache_size];
  // 每个索引是否在缓存中，以及在缓存具体位置的映射
  unordered_map<int, int> pos;
  // 缓存数组
  T cache[cache_size];
  // 写入文件和索引文件的文件流
  fstream file, file_info;
  // 写入文件的文件名与索引文件的文件名
  string file_name, file_info_name;
  // 空索引
  vector<int> empty_index;

  /// 获取索引为id的变量在文件中的具体位置，1-based
  int (*GetPos)(int id);
  // 处理size > 4KB的索引定位
  static int GetPosBig(int);
  // 处理size < 4KB的索引定位
  static int GetPosSmall(int);
  // 在文件并不存在时创建所需的文件
  void CreateFile();
  // 将索引为id的变量读入val（文件读写）
  void GetFile(int id, T &val);
  // 将val写入索引为id的变量（文件读写）
  void WriteFile(int id, T &val);
  // 删除索引为id的变量
  void DelFile(int id);

 public:
  // 无默认构造函数（必须要有文件名）
  FileReader() = delete;
  // 构造函数，文件名为file_name_,默认生成的索引文件为file_name_ + ".info"
  explicit FileReader(string file_name_);
  // 析构函数
  ~FileReader();
  // 将索引为id的整形信息写入val
  void GetInfo(int id, int &val) const;
  // 将val写入索引为id的整形变量
  void WriteInfo(int id, int val);
  // 将索引为id的变量写入val（缓存读写）
  void Get(int id, T &val);
  // 将val写入索引为id的变量（缓存读写）
  void Write(int id, const T &val);
  // 删除索引为id的变量
  void Del(int id);
  // 生成一个新变量并获取其索引信息
  int AskId();
  // 获取下标为id的变量，[]运算符版本
  T operator [](int id);
};
```



#### `FileMap`

`FileMap`为实现关键词查找的类，内部实现为B+树。

##### 接口与成员说明

类`Key`为存储键与值的类，并针对查找的顺序对其进行了运算符的重载。键为`unsigned long long`,实际由字符串通过哈希生成，值为`int`。

类`Node`存储B+树的每个节点。

类`FileMap`存储整颗B+树及其相关的信息。

```C++
class FileMap
{
 private:
  // B+树节点个数
  constexpr static int M = 150;
  // 存储文件名
  string file_name;
  FileReader<Node, 1> file;
  // 存储一个节点的父亲节点，在访问节点时动态记录
  unordered_map<int, int> parent;
  // 向内部节点cur的位置插入key
  void InsertInternal(const Key &key, int cur, int ch);
  // 将内部位置为cur的key键值删除
  void RemoveInternal(const Key &key, int cur, int ch);
  // 向上更新父亲信息
  void UpdateParent(const Key &key, int cur);

 public:
  int root;
  FileMap() = delete;
  // name为B+树文件名
  explicit FileMap(string name);
  ~FileMap();
  void Insert(const Key &key);
  void Remove(const Key &key);
  // 返回所有键为index对应的值，以vector<int>的形式返回
  vector<int> Find(unsigned long long index);
  // 返回树上的所有值，以vector<int>的形式返回
  vector<int> FindAll();
};
```



#### ``InstructionValidator``

`InstructionValidator`通过C++的`basic_regex`验证命令行输入的合法性，并从中抽取关键词。

所有形如`bool GetXxx(string command);`的函数均为判断`command`的合法性，若返回值为`true`，则将抽取的关键词通过引用传递的变量返回。

`string GetOptional(const string &s);`负责将某个捕获组/非捕获组改造为可选的捕获组（通过在其后加入`?`）。

`string GetMultiple(const string &s);`负责将某个捕获组/非捕获组改造为可重复的捕获组（通过在其后加入`+`）。

`string GetInstruction(const std::initializer_list<string> &s,bool suffix_space = true);`负责将零散的词素组装成完整的正则表达式（通过在捕获组间加`[space]+`接受任意多个空格。



#### `Utils`

`Utils`中包含项目中可能需要的工具类函数，便于统一调配。

##### 接口说明

```C++
// 字符串哈希所需的指数幂常数
constexpr unsigned long long pow = 1314527;
// 字符串哈希，通过自然溢出进行
unsigned long long GetHash(const string &s);
// 在字符串s中寻找pattern出现的次数，内部实现为字符串哈希
int StrCount(const string &s, const string &pattern);
// 输出Invalid信息
void Invalid();
// 将|分隔的字符串关键字信息进行分离，通过vector<string>的形式返回
vector<string> SplitKeyword(const string &s);
// 获取字符串s中的第一个单词
string GetWord(const string &s);
```



#### `BookSystem`

``BookInfo``类中存储每本书籍的详细信息。

`BookSystem`类中对所有书构成的数据库进行查询和维护工作，但不维护选中信息。

##### 实现逻辑

建立所有可能被查询关键词到书本信息在数据库中索引的映射，查询时先基于给定关键词查询索引，再从数据库中基于索引获取书本信息并返回。

##### 接口与成员说明

```C++
class BookSystem
{
 private:
  // 书本数量
  int num;
  // 书本信息的文件读写
  FileReader<BookInfo, 1> book;
  // 不同关键词到索引的映射
  FileMap code_map, name_map, author_map, keyword_map;
  // 找到ISBN为code的书本索引
  int Find(const string &code);
  
 public:、
  // 未找到的返回值
  static const int npos = -1;
  // 构造函数
  BookSystem();
  // 析构函数
  ~BookSystem();
  // 创建ISBN为code的书本信息
  int Create(const string &code);
  // 返回选中ISBN为code的书本的索引
  int Select(const string &code);
  // 基于关键词输出所有满足条件的书本信息
  void Show(const string &code, const string &name,
            const string &author, const string &keyword);
  // 购买ISBN为code的书籍并更新信息，quantity为购买数量
  void Buy(const string &code, int quantity);
  // 基于给定的索引id修改书本信息
  void Modify(int id, const string &ISBN, const string &name,
              const string &author, const string &keyword, long double price);
  // 基于给定的索引id进货书本
  void Import(int id, int quantity, long double total_cost);
};
```



#### `UserSystem`

枚举类`Privilege`为4种权限信息。

``UserInfo``类储存每个用户的信息。

``UserSystem``类对所有的用户信息进行维护和查询，但不维护登录栈与登录信息。

##### 实现逻辑

具体实现与书本系统类似，在此不多赘述。注意`UserSystem`类中需要根据传入的权限信息处理操作是否符合权限要求。

##### 接口与成员说明

`void init();`函数负责在没有用户时生成默认的最高权限的`root`用户。

其余接口形式和逻辑与书本系统类似，在此不多赘述。



#### `Log`

`FinanceInfo/EmployeeInfo/LogInfo`类分别存储财务报表/员工工作情况/日志的每个条目的信息。

`FinanceReport/EmployeeReport/Log`类分别维护和输出财务情况/员工工作情况/日志信息。

其中`FinanceReport`和`Log`均为线性地将日志信息存入文件，在需要时顺序读取并输出。

`EmployeeReport`与员工/书籍系统类似，基于用户名查询索引，并将工作情况写入与员工对应的工作情况。
