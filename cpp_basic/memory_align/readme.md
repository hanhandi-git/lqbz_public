为什么内存对齐很重要
1.硬件要求
```cpp
struct alignas(16) SSEData {  // 使用 SSE 指令需要 16 字节对齐
    float data[4];
};
```
2.性能优化
```cpp
struct CacheLine {
    alignas(64) char data[64];  // 缓存行对齐可以提高性能
};
```
3.避免未定义行为
```cpp
// placement new 在未对齐的内存上构造对象会导致未定义行为
template<typename ... Args>
void emplace_back(Args&& ... args) {
    if (size_ == capacity_) {
        reserve(capacity_ == 0 ? 1 : capacity_ * 2);
    }
    // data_ 已经正确对齐，所以这里的 placement new 是安全的
    new (&data_[size_++]) T(std::forward<Args>(args)...);
}
```
验证内存对齐的测试代码
```cpp
// 在 main 函数中添加测试
void test_alignment() {
    struct alignas(16) AlignedType {
        double d;
        int i;
    };

    Vector<AlignedType> vec;
    vec.reserve(10);

    // 验证对齐
    AlignedType* ptr = &vec[0];
    std::cout << "Alignment required: " << alignof(AlignedType) << std::endl;
    std::cout << "Actual alignment: " << 
        (reinterpret_cast<std::uintptr_t>(ptr) & (alignof(AlignedType) - 1)) 
        << std::endl;
}
```
更安全的实现方式
如果要更严格地控制内存对齐，可以这样修改:
```cpp
template<typename T>
class Vector {
    static constexpr size_t alignment = alignof(T);
    
    void* allocate(size_t n) {
        if (n == 0) return nullptr;
        return operator new(n * sizeof(T), std::align_val_t{alignment});
    }
    
    void deallocate(void* p) {
        if (p) {
            operator delete(p, std::align_val_t{alignment});
        }
    }
    
    void reserve(size_t new_capacity) {
        if (new_capacity > capacity_) {
            T* new_data = static_cast<T*>(allocate(new_capacity));
            // ... 其余代码不变
        }
    }
};
```