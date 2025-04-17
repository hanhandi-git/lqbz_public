#include <iostream>
#include <vector>
#include <memory>
#include <atomic>

// === 示例1 ===
struct ExampleA {
    char a;                  // 1 字节
    int b;                  // 4 字节（通常需要 4 字节对齐）
    char c;                 // 1 字节
    double d;               // 8 字节
    int arr[3];             // 3 * 4 = 12 字节
    void (*funcPtr)();      // 函数指针，8 字节（64 位系统）
    static int staticVar;   // 静态变量，不占用结构体大小
    virtual void virtualFunc() {} // 虚函数，引入虚函数表指针（vptr），8 字节（64 位系统）
};

// === 示例2 ===
struct ExampleB {
    char a;                  // 1 字节
    char c;                  // 1 字节
    // 填充 2 字节以满足 int 的对齐要求
    int b;                  // 4 字节
    double d;               // 8 字节
    int arr[3];             // 12 字节
    void (*funcPtr)();      // 8 字节
    virtual void virtualFunc() {} // 虚函数引入虚函数表指针（vptr），8 字节
    static int staticVar;   // 静态变量，不占用结构体大小
};

int ExampleA::staticVar = 0; // 静态变量的定义
int ExampleB::staticVar = 0; // 静态变量的定义


// === 示例3 ===
class ComplexExample {
public:
    // 成员变量
    char a;                          // 1 字节
    int b;                           // 4 字节
    std::vector<int> vec;           // 动态数组，可能会引入额外的内存 这里占用24字节（64 位系统）
    std::shared_ptr<int> ptr;        // 智能指针，16字节（64 位系统）
    std::atomic<int> atomicVar;      // 原子变量，4 字节（通常需要 4 字节对齐）
    double d;                       // 8 字节
    void (*funcPtr)();              // 函数指针，8 字节
    virtual void virtualFunc() {}    // 虚函数引入虚函数表指针（vptr），8 字节
    virtual void virtualFunc2() {}    // 多个虚函数，只占用一个虚函数表指针（vptr），8 字节

    // 构造函数 
    ComplexExample() : a(0), b(0), d(0.0), atomicVar(0) {}

    // 方法
    void addValue(int value) {
        vec.push_back(value);
    }
};
// char a (1字节)
// - 需要填充3字节来对齐int

// int b (4字节)
// - 需要填充4字节来对齐vector(因为vector是8字节对齐)

// std::vector<int> vec (24字节，在64位系统中vector包含3个指针，每个8字节)
// - 已经8字节对齐，不需要填充

// std::shared_ptr<int> ptr (16字节，shared_ptr包含两个指针)
// - 已经8字节对齐，不需要填充

// std::atomic<int> atomicVar (4字节)
// - 需要填充4字节来对齐double

// double d (8字节)
// - 已经8字节对齐，不需要填充

// void (*funcPtr)() (8字节)
// - 已经8字节对齐，不需要填充

// 计算总和：
// 8(vptr) + 1(a) + 3(padding) + 4(b) + 24(vec) + 16(ptr) + 4(atomicVar) + 4(padding) + 8(d) + 8(funcPtr) = 80字节


int main() {
    std::cout << "Size of std::vector<int>: " << sizeof(std::vector<int>) << " bytes" << std::endl;
    std::cout << "Size of std::shared_ptr<int>: " << sizeof(std::shared_ptr<int>) << " bytes" << std::endl;
    std::cout << "Size of std::atomic<int>: " << sizeof(std::atomic<int>) << " bytes" << std::endl;
    std::cout << "Size of ExampleA: " << sizeof(ExampleA) << " bytes" << std::endl;
    std::cout << "Size of ExampleB: " << sizeof(ExampleB) << " bytes" << std::endl;

    ComplexExample example;

    std::cout << "Size of ComplexExample: " << sizeof(ComplexExample) << " bytes" << std::endl;
    return 0;
}