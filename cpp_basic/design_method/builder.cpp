#include <iostream>
#include <string>

class Product {
public:
    std::string partA;
    std::string partB;

    void show() {
        std::cout << "Product Parts: " << partA << ", " << partB << std::endl;
    }
};

class Builder {
public:
    virtual void buildPartA() = 0;
    virtual void buildPartB() = 0;
    virtual Product getResult() = 0;
    virtual ~Builder() = default;
};

class ConcreteBuilder : public Builder {
private:
    Product product;

public:
    void buildPartA() override {
        product.partA = "Part A built";
    }

    void buildPartB() override {
        product.partB = "Part B built";
    }

    Product getResult() override {
        return product;
    }
};

class Director {
public:
    void construct(Builder& builder) {
        builder.buildPartA();
        builder.buildPartB();
    }
};

int main() {
    ConcreteBuilder builder;
    Director director;

    director.construct(builder);
    Product product = builder.getResult();
    product.show();

    return 0;
}
