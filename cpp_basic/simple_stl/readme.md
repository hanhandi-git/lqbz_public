# Simple STL Implementation

本项目实现了几个简单的STL容器，包括：

## 1. Vector (vector.cpp)
动态数组实现，提供以下接口：
- 构造函数（默认构造、拷贝构造、移动构造）
- 赋值运算符（拷贝赋值、移动赋值）
- operator[]：随机访问
- resize：调整大小
- reserve：预分配空间
- push_back：尾部插入
- emplace_back：原地构造
- pop_back：尾部删除

UML类图：
```mermaid
classDiagram
    class Vector<T> {
        -size_: size_t
        -capacity_: size_t
        -data_: T*
        
        +Vector()
        +Vector(const Vector& other)
        +Vector(Vector&& other) noexcept
        +operator=(const Vector& other): Vector&
        +operator=(Vector&& other) noexcept: Vector&
        +~Vector()
        
        +push_back<U>(U&& value): void
        +pop_back(): void
        +emplace_back<Args>(Args&&... args): void
        +operator[](size_t index): T&
        +operator[](size_t index) const: const T&
        
        +size() const: size_t
        +capacity() const: size_t
        +empty() const: bool
        +clear(): void
        +resize(size_t new_size): void
        +reserve(size_t new_capacity): void
        +swap(Vector& other) noexcept: void
    }
```
测试输出：
```
v: 1 2 3
v2: 1 2 3
v3: 1 2 3
v4: 1 2 3
v5: 1 2 3
v5: 1 2 3 0 0
v5: 1 2 3
v5:
```

## 2. Queue (queue.cpp)
链表实现的队列，提供以下接口：
- 构造函数（默认构造、拷贝构造、移动构造）
- 赋值运算符（拷贝赋值、移动赋值）
- push：入队
- pop：出队
- front：访问队首元素
- empty：判断是否为空
- size：获取大小

测试输出：
```
Queue front: 1
Queue size: 3
After pop, front: 2
q2 front: 2
q3 front: 2
```

## 3. Stack (stack.cpp)
链表实现的栈，提供以下接口：
- 构造函数（默认构造、拷贝构造、移动构造）
- 赋值运算符（拷贝赋值、移动赋值）
- push：入栈
- pop：出栈
- top：访问栈顶元素
- empty：判断是否为空
- size：获取大小

测试输出：
```
Stack top: 3
Stack size: 3
After pop, top: 2
s2 top: 2
s3 top: 2
```

## 4. HashTable (hashtable.cpp)
开放地址法实现的哈希表，提供以下接口：
- 构造函数（默认构造、拷贝构造、移动构造）
- 赋值运算符（拷贝赋值、移动赋值）
- insert：插入键值对
- erase：删除键值对
- operator[]：访问或插入值
- contains：检查键是否存在
- size/empty：获取大小/判断是否为空
- clear：清空哈希表

测试输出：
```
apple: 1
banana: 2
orange: 3
Contains apple: true
Contains grape: false
After erasing banana, contains banana: false
ht2 apple: 1
ht3 apple: 1
Size of ht3: 2
```

## 5. DoublyLinkedList (doubly_linked_list.cpp)
双向链表实现，提供以下接口：
- 构造函数（默认构造、拷贝构造、移动构造）
- 赋值运算符（拷贝赋值、移动赋值）
- push_front/push_back：首尾插入
- pop_front/pop_back：首尾删除
- insert：指定位置插入
- erase：指定位置删除
- front/back：访问首尾元素
- begin/end：迭代器支持

测试输出：
```
List contents: 0 1 2
After insert: 0 5 1 2
After erase: 0 1 2
Copy constructed list: 0 1 2
Move constructed list: 0 1 2
Front: 0
Back: 2
```

## 使用方法
每个容器都在独立的cpp文件中实现，包含对应的测试代码。使用提供的build.sh脚本编译所有容器：

```bash
chmod +x build.sh  # 使脚本可执行（如果需要）
./build.sh         # 运行构建脚本
```

然后可以分别运行生成的测试程序：
```bash
./vector_test
./queue_test
./stack_test
./hashtable_test
./doubly_linked_list_test
```

## 实现特点
1. 所有容器都是模板类，支持任意数据类型
2. 实现了完整的构造函数、赋值运算符
3. 支持移动语义，提高性能
4. 包含异常处理
5. 内存管理安全，无内存泄漏
6. 提供了完整的单元测试
