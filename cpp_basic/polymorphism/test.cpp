#include <iostream>

class Base {
public:
    Base() {
        init();  // 调用虚函数
    }

    virtual void init() {
        std::cout << "Base::init" << std::endl;
    }
};

class Derived :public Base {
public:
    void init() override {
        std::cout << "Derived::init" << std::endl;
    }
};

int main() {
    Derived d;
    return 0;
}