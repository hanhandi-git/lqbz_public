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
            Node* cur = other.top_;
            Node* dummy = nullptr;
            while (cur != nullptr) {
                Node* new_node = new Node(cur->data);
                new_node->next = dummy;
                dummy = new_node;
                cur = cur->next;
            }
            top_ = dummy;  // 将dummy赋值给top_
            size_ = other.size_;
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
            // 先清空当前栈
            while (!empty()) {
                pop();
            }
            // 复制其他栈
            Node* cur = other.top_;
            Node* dummy = nullptr;
            while (cur != nullptr) {
                Node* new_node = new Node(cur->data);
                new_node->next = dummy;
                dummy = new_node;
                cur = cur->next;
            }
            top_ = dummy;
            size_ = other.size_;
        }
        return *this;
    }

    // 移动赋值运算符
    Stack& operator=(Stack&& other) noexcept {
        if (this != &other) {
            // 先清空当前栈
            while (!empty()) {
                pop();
            }
            // 移动其他栈
            top_ = other.top_;
            size_ = other.size_;
            other.top_ = nullptr;
            other.size_ = 0;
        }
        return *this;
    }

    // push
    void push(const T& value) {
        Node* new_node = new Node(value);
        new_node->next = top_;
        top_ = new_node;
        ++size_;
    }

    // pop
    void pop() {
        if (empty()) {
            throw std::runtime_error("Stack is empty");
        }
        Node* temp = top_;
        top_ = top_->next;
        delete temp;
        --size_;
    }

    // 添加 emplace 操作
    template<typename... Args>
    void emplace(Args&&... args) {
        Node* new_node = new Node(T(std::forward<Args>(args)...));
        new_node->next = top_;
        top_ = new_node;
        ++size_;
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

    void clear() {
        while (!empty()) {
            pop();
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

    // 测试 emplace
    s3.emplace(4);
    std::cout << "After emplace, s3 top: " << s3.top() << std::endl;

    return 0;
} 