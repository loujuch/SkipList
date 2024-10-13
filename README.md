# Skip List

## 模板简介

```c++
template <typename Key, typename Value, typename KeyOfValue, typename Compare, typename floor_number_type MaxFloorNumber = 32>
class SkipList
```

|模板参数|解释|
|---|---|
|Key|键类型|
|Value|值类型，值类型中应该包含键类型|
|KeyOfValue|键提取器，从值对象中提取出键类型的仿函数|
|Compare|比较器，对键类型进行比较的仿函数|
|MaxFloorNumber|最高层数，默认为32|

## 类方法简介

### 构造函数

|函数定义|函数用途|参数|
|----|----|----|
|SkipList(double p = 0.5)|构造一个空的SkipList对象|p: 每层增长概率，默认为0.5|
|template \<typename Iterator\><br>SkipList(Iterator first, Iterator last, double p = 0.5)|通过迭代器拷贝其他容器内容的构造函数|first: 起始迭代器<br>last: 终点迭代器<br>p: 每层增长概率，默认为0.5|
|SkipList(std::initializer_list<value_type> ilist, double p = 0.5)|通过初始化列表初始化的构造函数|ilist: 初始化列表<br>p: 每层增长概率，默认为0.5|
|SkipList(const self &sl)|拷贝构造函数|sl: 拷贝目标|
|SkipList(self &&sl)|移动构造函数|sl: 移动目标|

...