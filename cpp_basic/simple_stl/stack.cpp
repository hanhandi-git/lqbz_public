#include <iostream>
#include <stdexcept>

template<typename T>
class Stack {
private:
    struct Node {
        T data;
        Node* next;
        Node(const T& value) : data(value), next(nullptr) {}
    };
    
    Node* top_;
    size_t size_;

public:
    Stack() : top_(nullptr), size_(0) {}
    
    ~Stack() {
        while (!empty()) {
            pop();
        }
    }

    // 拷贝构造函数
    Stack(const Stack& other) : top_(nullptr), size_(0) {
        if (!other.empty()) {
            Node* current = other.top_;
            Node* prev = nullptr;
            while (current != nullptr) {
                Node* new_node = new Node(current->data);
                if (prev == nullptr) {
                    top_ = new_node;
                } else {
                    prev->next = new_node;
                }
                prev = new_node;
                current = current->next;
                ++size_;
            }
        }
    }

    // 移动构造函数
    Stack(Stack&& other) noexcept : top_(other.top_), size_(other.size_) {
        other.top_ = nullptr;
        other.size_ = 0;
    }

    // 拷贝赋值运算符
    Stack& operator=(const Stack& other) {
        if (this != &other) {
            Stack temp(other);
            std::swap(top_, temp.top_);
            std::swap(size_, temp.size_);
        }
        return *this;
    }

    // 移动赋值运算符
    Stack& operator=(Stack&& other) noexcept {
        if (this != &other) {
            while (!empty()) {
                pop();
            }
            top_ = other.top_;
            size_ = other.size_;
            other.top_ = nullptr;
            other.size_ = 0;
        }
        return *this;
    }

    void push(const T& value) {
        Node* new_node = new Node(value);
        new_node->next = top_;
        top_ = new_node;
        ++size_;
    }

    void pop() {
        if (empty()) {
            throw std::runtime_error("Stack is empty");
        }
        Node* temp = top_;
        top_ = top_->next;
        
        temp->data.~T();         // 1. 显式调用析构函数
        ::operator delete(temp); // 2. 释放内存
        --size_;
    }

    T& top() {
        if (empty()) {
            throw std::runtime_error("Stack is empty");
        }
        return top_->data;
    }

    const T& top() const {
        if (empty()) {
            throw std::runtime_error("Stack is empty");
        }
        return top_->data;
    }

    bool empty() const {
        return size_ == 0;
    }

    size_t size() const {
        return size_;
    }

    // 添加 emplace 操作
    template<typename... Args>
    void emplace(Args&&... args) {
        // 分配节点内存
        Node* new_node = static_cast<Node*>(::operator new(sizeof(Node)));
        try {
            // 使用 placement new 在已分配的内存上构造数据
            new (&new_node->data) T(std::forward<Args>(args)...);
            new_node->next = top_;
            top_ = new_node;
            ++size_;
        } catch (...) {
            // 如果构造失败，释放节点内存
            ::operator delete(new_node);
            throw;
        }
    }
};

// 测试代码
int main() {
    Stack<int> s;
    s.push(1);
    s.push(2);
    s.push(3);

    std::cout << "Stack top: " << s.top() << std::endl;
    std::cout << "Stack size: " << s.size() << std::endl;

    s.pop();
    std::cout << "After pop, top: " << s.top() << std::endl;

    Stack<int> s2 = s;  // 测试拷贝构造
    std::cout << "s2 top: " << s2.top() << std::endl;

    Stack<int> s3 = std::move(s);  // 测试移动构造
    std::cout << "s3 top: " << s3.top() << std::endl;

    return 0;
} 