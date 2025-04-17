#include <iostream>
#include <new>

class MyClass {
    int value;
    std::string str;
public:
    MyClass() : value(0), str("default") {
        std::cout << "Constructor called" << std::endl;
    }
    
    MyClass(int v, const std::string& s) : value(v), str(s) {
        std::cout << "Parameterized constructor called" << std::endl;
    }
    
    ~MyClass() {
        std::cout << "Destructor called" << std::endl;
    }
    
    void print() const {
        std::cout << "Value: " << value << ", String: " << str << std::endl;
    }
};

// 简单的内存池示例
class SimpleMemoryPool {
    char* memory;
    size_t size;
    
public:
    SimpleMemoryPool(size_t s) : size(s) {
        memory = new char[size];
    }
    
    ~SimpleMemoryPool() {
        delete[] memory;
    }
    
    void* allocate(size_t bytes) {
        if (bytes <= size) {
            return memory;
        }
        return nullptr;
    }
};

int main() {
    // 1. 基本使用示例
    std::cout << "=== Basic Usage ===" << std::endl;
    char buffer[sizeof(MyClass)];
    MyClass* obj1 = new (buffer) MyClass(42, "test");
    obj1->print();
    obj1->~MyClass();  // 显式调用析构函数
    
    // 2. 内存池示例
    std::cout << "\n=== Memory Pool Usage ===" << std::endl;
    SimpleMemoryPool pool(sizeof(MyClass) * 2);
    void* memory = pool.allocate(sizeof(MyClass));
    
    MyClass* obj2 = new (memory) MyClass(100, "pool");
    obj2->print();
    obj2->~MyClass();
    
    // 3. 数组示例
    std::cout << "\n=== Array Usage ===" << std::endl;
    alignas(MyClass) char array_buffer[sizeof(MyClass) * 3];
    MyClass* objects[3];
    
    for (int i = 0; i < 3; ++i) {
        objects[i] = new (array_buffer + i * sizeof(MyClass)) 
                        MyClass(i, "array" + std::to_string(i));
        objects[i]->print();
    }
    
    // 反向析构数组中的对象
    for (int i = 2; i >= 0; --i) {
        objects[i]->~MyClass();
    }
    
    return 0;
}
