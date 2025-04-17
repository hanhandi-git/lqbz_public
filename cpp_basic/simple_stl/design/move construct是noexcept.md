为什么这里的move construct需要使用noexcept呢？
使用 noexcept 在移动构造函数中有几个重要原因：
**1. 标准容器的强异常保证**
```cpp
std::vector<Vector<int>> vec;
vec.push_back(Vector<int>());  // 如果需要扩容，std::vector 会选择移动而不是拷贝
```
- 当 std::vector 需要扩容时，它需要将元素移动到新的内存位置
- 如果移动构造函数标记为 noexcept，std::vector 会使用移动操作
- 如果没有标记 noexcept，为了保证强异常安全性，std::vector 会退化为使用拷贝操作
**2. 性能优化**
```cpp
Vector(Vector&& other) noexcept : size_(other.size_), 
                                 capacity_(other.capacity_), 
                                 data_(other.data_) {
    // 这些操作实际上不可能抛出异常
    other.size_ = 0;
    other.capacity_ = 0;
    other.data_ = nullptr;
}
```
- 移动操作通常是简单的指针和整数赋值，不会抛出异常
- 标记 noexcept 可以让编译器生成更优化的代码
**3. 语义保证**
```cpp
// 移动赋值运算符也应该是 noexcept
Vector& operator=(Vector&& other) noexcept {
    if (this != &other) {
        delete[] data_;  // 不会抛出异常
        // 简单的赋值操作
        size_ = other.size_;
        capacity_ = other.capacity_;
        data_ = other.data_;
        other.size_ = 0;
        other.capacity_ = 0;
        other.data_ = nullptr;
    }
    return *this;
}
```
- 移动操作应该总是成功的，因为它们只是资源的转移
- 如果移动操作可能失败，那么它就不是真正的"移动"
**4. 与标准库一致性**
```cpp
static_assert(std::is_nothrow_move_constructible<Vector<int>>::value,
              "Vector should be nothrow move constructible");
```
标准库容器的移动构造函数都是 noexcept 的 这是一个广泛接受的约定
**5. 示例：std::vector 的行为**
```cpp
std::vector<Vector<int>> containers;
containers.reserve(10);  // 预分配空间

Vector<int> v;
// 如果 Vector 的移动构造是 noexcept
containers.push_back(std::move(v));  // 使用移动构造

// 如果 Vector 的移动构造不是 noexcept
containers.push_back(std::move(v));  // 可能使用拷贝构造！
```
**6. 异常安全性考虑**
```cpp
void swap(Vector& other) noexcept {  // swap 也应该是 noexcept
    std::swap(size_, other.size_);
    std::swap(capacity_, other.capacity_);
    std::swap(data_, other.data_);
}
```
- 移动操作通常用于实现异常安全的代码
- 如果移动操作可能抛出异常，会使异常安全的实现变得更困难