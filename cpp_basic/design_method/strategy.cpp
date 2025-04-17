#include <iostream>
#include <memory>

class Strategy {
public:
    virtual void execute() = 0;
    virtual ~Strategy() = default;
};

class ConcreteStrategyA : public Strategy {
public:
    void execute() override {
        std::cout << "Executing Strategy A" << std::endl;
    }
};

class ConcreteStrategyB : public Strategy {
public:
    void execute() override {
        std::cout << "Executing Strategy B" << std::endl;
    }
};

class Context {
private:
    std::unique_ptr<Strategy> strategy;

public:
    void setStrategy(std::unique_ptr<Strategy> newStrategy) {
        strategy = std::move(newStrategy);
    }

    void executeStrategy() {
        if (strategy) {
            strategy->execute();
        }
    }
};

int main() {
    Context context;
    context.setStrategy(std::make_unique<ConcreteStrategyA>());
    context.executeStrategy();

    context.setStrategy(std::make_unique<ConcreteStrategyB>());
    context.executeStrategy();

    return 0;
}
