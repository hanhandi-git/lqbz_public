#include <iostream>
#include <memory>
#include <vector>

class Observer {
public:
    virtual void update(const std::string& msg) = 0;
    virtual ~Observer() = default;
};

class ConcreteObserver : public Observer {
public:
    void update(const std::string& msg) override {
        std::cout << "receive update:" << msg << std::endl;
    }
};

class Subject {
private:
    std::vector<std::shared_ptr<Observer>> observers;

public:
    void attach(const std::shared_ptr<Observer>& observer) {
        observers.push_back(observer);
    }

    void notify(const std::string& msg) {
        for (auto& ob : observers) {
            ob->update(msg);
        }
    }
};

int main()
{
    auto ob1 = std::make_shared<ConcreteObserver>();
    auto ob2 = std::make_shared<ConcreteObserver>();

    Subject subject;
    subject.attach(ob1);
    subject.attach(ob2);
    subject.notify("hello");
    return 0;
}

// g++ -std=c++11 observer.cpp -o test
