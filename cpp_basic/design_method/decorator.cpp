#include <iostream>
#include <memory>

class Component {
public:
    virtual void operation() = 0;
    virtual ~Component() = default;
};

class ConcreteComponent : public Component {
public:
    void operation() override {
        std::cout << "Concrete Component operation" << std::endl;
    }
};

class Decorator : public Component {
protected:
    std::unique_ptr<Component> component;

public:
    explicit Decorator(std::unique_ptr<Component> comp) : component(std::move(comp)) {}
    void operation() override {
        if (component) {
            component->operation();
        }
    }
};

class ConcreteDecoratorA : public Decorator {
public:
    explicit ConcreteDecoratorA(std::unique_ptr<Component> comp) : Decorator(std::move(comp)) {}

    void operation() override {
        Decorator::operation();
        std::cout << "Concrete Decorator A operation" << std::endl;
    }
};

class ConcreteDecoratorB : public Decorator {
public:
    explicit ConcreteDecoratorB(std::unique_ptr<Component> comp) : Decorator(std::move(comp)) {}

    void operation() override {
        Decorator::operation();
        std::cout << "Concrete Decorator B operation" << std::endl;
    }
};

int main() {
    auto component = std::make_unique<ConcreteComponent>();
    auto decoratorA = std::make_unique<ConcreteDecoratorA>(std::move(component));
    auto decoratorB = std::make_unique<ConcreteDecoratorB>(std::move(decoratorA));

    decoratorB->operation();

    return 0;
}
