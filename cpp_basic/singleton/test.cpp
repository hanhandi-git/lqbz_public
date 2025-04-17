#include <iostream>

class Singleton {
public:
    static Singleton& getInstance() {
        static Singleton st;
        return st;
    }

    void showMessage() {
        std::cout << "address:" << this << std::endl;
    }

private:
    Singleton() = default;
    ~Singleton() = default;
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;
};

int main()
{
    Singleton& s1 = Singleton::getInstance();
    Singleton& s2 = Singleton::getInstance();
    s1.showMessage();
    s2.showMessage();
    return 0;
}
// g++ -std=c++11 test.cpp -o test