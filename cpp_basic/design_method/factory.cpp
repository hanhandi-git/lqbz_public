#include <iostream>
#include <memory>

class Product {
public:
    virtual void use() = 0;
    virtual ~Product() = default;
};

class ConcreteProductA : public Product {
public:
    void use() override {
        std::cout << "use product A" << std::endl;
    }
};

class ConcreteProductB : public Product {
public:
    void use() override {
        std::cout << "use product B" << std::endl;
    }
};

class Factory {
public:
    virtual std::unique_ptr<Product> createProduct() = 0;
    virtual ~Factory() = default;
};

class FactoryA : public Factory {
public:
    std::unique_ptr<Product> createProduct() override {
        return std::make_unique<ConcreteProductA>();
    }
};

class FactoryB : public Factory {
public:
    std::unique_ptr<Product> createProduct() override {
        return std::make_unique<ConcreteProductB>();
    }
};

int main()
{
    std::unique_ptr<Factory> factory = std::make_unique<FactoryA>();
    auto product = factory->createProduct();
    product->use();

    factory = std::make_unique<FactoryB>();
    product = factory->createProduct();
    product->use();
    return 0;
}

// g++ -std=c++14 factory.cpp -o test