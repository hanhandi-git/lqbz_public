// test_memory_layout.cpp
#include "memory_layout_analyzer.hpp"
#include <vector>
#include <memory>
#include <atomic>

class ComplexExample {
public:
    char a;                          
    int b;                           
    std::vector<int> vec;           
    std::shared_ptr<int> ptr;        
    std::atomic<int> atomicVar;      
    double d;                       
    void (*funcPtr)();              
    virtual void virtualFunc() {}    
    virtual void virtualFunc2() {}    
};

int main() {
    MemoryLayoutAnalyzer analyzer("ComplexExample");
    
    // 使用宏添加成员信息
    analyzer.ADD_MEMBER(ComplexExample, a);
    analyzer.ADD_MEMBER(ComplexExample, b);
    analyzer.ADD_MEMBER(ComplexExample, vec);
    analyzer.ADD_MEMBER(ComplexExample, ptr);
    analyzer.ADD_MEMBER(ComplexExample, atomicVar);
    analyzer.ADD_MEMBER(ComplexExample, d);
    analyzer.ADD_MEMBER(ComplexExample, funcPtr);

    analyzer.analyze();
    return 0;
}