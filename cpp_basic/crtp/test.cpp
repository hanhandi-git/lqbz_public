#include <iostream>
#include <string>
#include <chrono>
#include <vector>

// 1. 对象计数器示例
template<typename Derived>
class ObjectCounter {
    static int count;  // 改为类外初始化
protected:
    ObjectCounter() { count++; }
    ~ObjectCounter() { count--; }
public:
    static int getCount() { return count; }
};

// 在类外初始化静态成员
template<typename Derived>
int ObjectCounter<Derived>::count = 0;

class Widget : public ObjectCounter<Widget> {
public:
    Widget() = default;
    ~Widget() = default;
};

// 2. 静态接口实现示例
template<typename Derived>
class Printable {
public:
    void print() const {
        static_cast<const Derived*>(this)->printImpl();
    }
    
    std::string toString() const {
        return static_cast<const Derived*>(this)->toStringImpl();
    }
};

class Employee : public Printable<Employee> {
    std::string name;
    int id;
public:
    Employee(const std::string& n, int i) : name(n), id(i) {}
    
    void printImpl() const {
        std::cout << "Employee: " << name << ", ID: " << id << std::endl;
    }
    
    std::string toStringImpl() const {
        return "Employee(" + name + ", " + std::to_string(id) + ")";
    }
};

// 3. 性能比较示例
// 3.1 CRTP版本
template<typename Derived>
class ShapeCRTP {
public:
    double area() const {
        return static_cast<const Derived*>(this)->areaImpl();
    }
};

class CircleCRTP : public ShapeCRTP<CircleCRTP> {
    double radius;
public:
    CircleCRTP(double r) : radius(r) {}
    double areaImpl() const { return 3.14159 * radius * radius; }
};

// 3.2 虚函数版本
class ShapeVirtual {
public:
    virtual double area() const = 0;
    virtual ~ShapeVirtual() = default;
};

class CircleVirtual : public ShapeVirtual {
    double radius;
public:
    CircleVirtual(double r) : radius(r) {}
    double area() const override { return 3.14159 * radius * radius; }
};

// 测试函数
void testObjectCounter() {
    std::cout << "\n=== Testing Object Counter ===\n";
    std::cout << "Initial count: " << Widget::getCount() << std::endl;
    
    {
        Widget w1;
        std::cout << "After creating w1: " << Widget::getCount() << std::endl;
        Widget w2;
        std::cout << "After creating w2: " << Widget::getCount() << std::endl;
    }
    
    std::cout << "After destroying widgets: " << Widget::getCount() << std::endl;
}

void testPrintable() {
    std::cout << "\n=== Testing Printable Interface ===\n";
    Employee emp("John Doe", 12345);
    emp.print();
    std::cout << "ToString: " << emp.toString() << std::endl;
}

void testPerformance() {
    std::cout << "\n=== Performance Comparison ===\n";
    const int iterations = 10000000;
    volatile double radius = 5.0;  // 使用volatile防止编译器过度优化
    
    // CRTP测试
    auto start = std::chrono::high_resolution_clock::now();
    CircleCRTP circleCRTP(radius);
    volatile double sumCRTP = 0;  // 使用volatile防止编译器优化掉计算
    for (int i = 0; i < iterations; ++i) {
        sumCRTP += circleCRTP.area();
    }
    auto endCRTP = std::chrono::high_resolution_clock::now();
    auto durationCRTP = std::chrono::duration_cast<std::chrono::microseconds>(endCRTP - start);
    
    // 虚函数测试
    start = std::chrono::high_resolution_clock::now();
    CircleVirtual circleVirtual(radius);
    ShapeVirtual* shapePtr = &circleVirtual;  // 使用指针而不是引用
    volatile double sumVirtual = 0;  // 使用volatile防止编译器优化掉计算
    for (int i = 0; i < iterations; ++i) {
        sumVirtual += shapePtr->area();
    }
    auto endVirtual = std::chrono::high_resolution_clock::now();
    auto durationVirtual = std::chrono::duration_cast<std::chrono::microseconds>(endVirtual - start);
    
    std::cout << "CRTP version took: " << durationCRTP.count() << " microseconds\n";
    std::cout << "Virtual version took: " << durationVirtual.count() << " microseconds\n";
    std::cout << "Performance ratio: " << static_cast<double>(durationVirtual.count()) / durationCRTP.count() << "x\n";
    
    // 打印结果防止编译器优化掉整个计算
    std::cout << "Sums (for verification): " << sumCRTP << ", " << sumVirtual << std::endl;
}

int main() {
    // 运行所有测试
    testObjectCounter();
    testPrintable();
    testPerformance();
    
    return 0;
} 