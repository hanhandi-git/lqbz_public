#include "my_enable_shared_from_this.hpp"
#include <iostream>

class Test : public my_enable_shared_from_this<Test> {
public:
    void doSomething() {
        std::shared_ptr<Test> self = shared_from_this();
        std::cout << "Using shared_from_this()" << std::endl;
    }
};

int main() {
    // 正确用法
    auto ptr = std::make_shared<Test>();
    ptr->doSomething();
    
    // 错误用法 - 会抛出异常
    try {
        Test t;
        t.doSomething();
    } catch (const std::bad_weak_ptr& e) {
        std::cout << "Exception: " << e.what() << std::endl;
    }
    
    return 0;
} 