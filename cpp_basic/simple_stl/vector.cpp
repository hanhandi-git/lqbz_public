#include <iostream>
#include <stdexcept>
#include <utility>
#include <cstddef>
template<typename T>

class Vector {
public:
    Vector() : size_(0), capacity_(0), data_(nullptr) {}
    // copy construct
    Vector(const Vector& other) : size_(other.size_), capacity_(other.capacity_), data_(new T[capacity_]) {
        for (size_t i = 0; i < size_; i++) {
            data_[i] = other.data_[i];
        }
    }
    
    // move construct
    Vector(Vector&& other) noexcept : size_(other.size_), capacity_(other.capacity_), data_(other.data_) {
        other.size_ = 0;
        other.capacity_ = 0;
        other.data_ = nullptr;
    }
    // 为什么operator 操作返回的都是引用呢？
    // 如果不返回引用的话会有如下几个弊端
    // 会创建不必要的临时对象，性能会下降
    // 无法支持连续赋值操作
    // copy =
    Vector& operator=(const Vector& other) {
        if (this != &other) {
            delete[] data_;
            size_ = other.size_;
            capacity_ = other.capacity_;
            data_ = new T[capacity_];
            for (size_t i = 0; i < size_; i++) {
                data_[i] = other.data_[i];
            }
        }
        return *this;
    }
    // move =
    Vector& operator=(Vector&& other) noexcept {
        if (this != &other) {
            delete[] data_;
            size_ = other.size_;
            capacity_ = other.capacity_;
            data_ = other.data_;
            other.size_ = 0;
            other.capacity_ = 0;
            other.data_ = nullptr;
        }
        return *this;
    }

    ~Vector() {
        if (data_ != nullptr) {
            delete[] data_;
        }
    }

    // 这里使用万能引用去捕获左值 右值
    template<typename U>
    void push_back(U&& value) {
        if (size_ == capacity_) {
            reserve(capacity_ == 0 ? 1 : capacity_ * 2);
        }
        data_[size_++] = std::forward<U>(value);
    }

    template<typename ... Args>
    void emplace_back(Args&& ... args) {
        if (size_ == capacity_) {
            reserve(capacity_ == 0 ? 1 : capacity_ * 2);
        }
        new (&data_[size_++]) T(std::forward<Args>(args)...);
    }

    void pop_back() {
        if (size_ == 0) {
            throw std::out_of_range("Vector is empty");
        }
        --size_;
    }

    T& operator[](size_t index) {
        if (index >= size_) {
            throw std::out_of_range("Index out of range");
        }
        return data_[index];
    }

    const T& operator[](size_t index) const {
        if (index >= size_) {
            throw std::out_of_range("Index out of range");
        }
        return data_[index];
    }

    size_t size() const {
        return size_;
    }

    size_t capacity() const {
        return capacity_;
    }

    bool empty() const {
        return size_ == 0;
    }

    void clear() {
        size_ = 0;
    }

    void resize(size_t new_size) {
        if (new_size > size_) {
            reserve(new_size);
            for (size_t i = size_; i < new_size; i++) {
                new (&data_[i]) T();
            }
        } else {    // 否则析构掉多余的元素
            for (size_t i = new_size; i < size_; i++) {
                data_[i].~T();
            }
        }
        size_ = new_size;
    }

    void reserve(size_t new_capacity) {
        if (new_capacity > capacity_) {
            T* new_data = new T[new_capacity];
            for(size_t i = 0; i < size_; i++) {
                new_data[i] = data_[i];
            }
            delete[] data_;
            data_ = new_data;
            capacity_ = new_capacity;
        }
    }

    void swap(Vector& other) noexcept {
        std::swap(size_, other.size_);
        std::swap(capacity_, other.capacity_);
        std::swap(data_, other.data_);
    }

    // 将 check_alignment 移到 public 部分
    bool check_alignment() const {
        return reinterpret_cast<std::uintptr_t>(data_) % alignof(T) == 0;
    }

private:
    size_t size_;
    size_t capacity_;
    T* data_;

    // 添加一个友元函数用于打印
    template<typename U>
    friend std::ostream& operator<<(std::ostream& os, const Vector<U>& vec);

    // 添加一个静态断言来检查对齐要求
    static_assert(alignof(T) <= alignof(std::max_align_t), 
                 "Alignment requirement too strict");
};

// 添加打印函数
template<typename T>
std::ostream& operator<<(std::ostream& os, const Vector<T>& vec) {
    os << "[ ";
    for (size_t i = 0; i < vec.size_; ++i) {
        os << vec.data_[i];
        if (i != vec.size_ - 1) {
            os << ", ";
        }
    }
    os << " ]";
    return os;
}


// 在 main 函数中添加测试
void test_alignment() {
    struct alignas(16) AlignedType {
        double d;
        int i;
        
        // 添加构造函数
        AlignedType(double d_ = 0.0, int i_ = 0) : d(d_), i(i_) {}
    };

    Vector<AlignedType> vec;
    vec.reserve(10);
    vec.emplace_back(1.0, 1);

    // 直接检查内部数组的对齐
    std::cout << "Alignment required: " << alignof(AlignedType) << std::endl;
    
    // 检查是否正确对齐
    if (vec.check_alignment()) {
        std::cout << "Memory is correctly aligned" << std::endl;
    } else {
        std::cout << "Memory alignment error!" << std::endl;
    }
    
    // 打印实际的对齐值
    AlignedType* ptr = &vec[0];
    std::cout << "Actual alignment offset: " << 
        (reinterpret_cast<std::uintptr_t>(ptr) & (alignof(AlignedType) - 1)) 
        << " (should be 0)" << std::endl;
}

int main() {
    std::cout << "\n=== Vector 完整功能测试 ===\n\n";
    
    // 1. 构造函数测试
    std::cout << "1. 构造函数测试\n";
    Vector<int> v;
    std::cout << "   默认构造: v = " << v << std::endl;
    std::cout << "   size = " << v.size() << ", capacity = " << v.capacity() << std::endl;
    
    // 2. push_back 测试
    std::cout << "\n2. push_back 测试\n";
    std::cout << "   添加元素 1, 2, 3\n";
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);
    std::cout << "   v = " << v << std::endl;
    std::cout << "   size = " << v.size() << ", capacity = " << v.capacity() << std::endl;
    
    // 3. emplace_back 测试
    std::cout << "\n3. emplace_back 测试\n";
    v.emplace_back(4);
    std::cout << "   emplace_back(4): v = " << v << std::endl;
    
    // 4. operator[] 测试
    std::cout << "\n4. operator[] 测试\n";
    std::cout << "   v[2] = " << v[2] << std::endl;
    v[2] = 30;
    std::cout << "   修改 v[2] = 30 后: v = " << v << std::endl;
    
    // 5. 拷贝构造测试
    std::cout << "\n5. 拷贝构造测试\n";
    Vector<int> v2(v);
    std::cout << "   v2(v): v2 = " << v2 << std::endl;
    
    // 6. 移动构造测试
    std::cout << "\n6. 移动构造测试\n";
    Vector<int> v3(std::move(v));
    std::cout << "   v3(std::move(v)): v3 = " << v3 << std::endl;
    std::cout << "   移动后 v = " << v << std::endl;
    
    // 7. 拷贝赋值测试
    std::cout << "\n7. 拷贝赋值测试\n";
    Vector<int> v4;
    v4 = v2;
    std::cout << "   v4 = v2: v4 = " << v4 << std::endl;
    
    // 8. 移动赋值测试
    std::cout << "\n8. 移动赋值测试\n";
    Vector<int> v5;
    v5 = std::move(v3);
    std::cout << "   v5 = std::move(v3): v5 = " << v5 << std::endl;
    std::cout << "   移动后 v3 = " << v3 << std::endl;
    
    // 9. pop_back 测试
    std::cout << "\n9. pop_back 测试\n";
    v5.pop_back();
    std::cout << "   pop_back 后: v5 = " << v5 << std::endl;
    
    // 10. resize 测试
    std::cout << "\n10. resize 测试\n";
    std::cout << "    扩大前: v5 = " << v5 << std::endl;
    v5.resize(6);
    std::cout << "    resize(6) 后: v5 = " << v5 << std::endl;
    v5.resize(2);
    std::cout << "    resize(2) 后: v5 = " << v5 << std::endl;
    
    // 11. reserve 测试
    std::cout << "\n11. reserve 测试\n";
    std::cout << "    reserve前 capacity = " << v5.capacity() << std::endl;
    v5.reserve(10);
    std::cout << "    reserve(10)后 capacity = " << v5.capacity() << std::endl;
    
    // 12. empty 测试
    std::cout << "\n12. empty 测试\n";
    std::cout << "    v5.empty() = " << std::boolalpha << v5.empty() << std::endl;
    
    // 13. clear 测试
    std::cout << "\n13. clear 测试\n";
    v5.clear();
    std::cout << "    clear后: v5 = " << v5 << std::endl;
    std::cout << "    v5.empty() = " << v5.empty() << std::endl;
    
    // 14. swap 测试
    std::cout << "\n14. swap 测试\n";
    Vector<int> v6;
    v6.push_back(100);
    v6.push_back(200);
    std::cout << "    交换前: v4 = " << v4 << ", v6 = " << v6 << std::endl;
    v4.swap(v6);
    std::cout << "    交换后: v4 = " << v4 << ", v6 = " << v6 << std::endl;
    
    // 15. 异常处理测试
    std::cout << "\n15. 异常处理测试\n";
    try {
        std::cout << "    尝试访问越界元素 v4[10]\n";
        std::cout << v4[10] << std::endl;
    } catch (const std::out_of_range& e) {
        std::cout << "    捕获到异常: " << e.what() << std::endl;
    }
    
    try {
        std::cout << "    尝试对空vector执行pop_back()\n";
        Vector<int> empty_vec;
        empty_vec.pop_back();
    } catch (const std::out_of_range& e) {
        std::cout << "    捕获到异常: " << e.what() << std::endl;
    }
    std::cout << "对齐测试" << std::endl;
    test_alignment();
    return 0;
}