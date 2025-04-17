#include <iostream>
#include <stdexcept>

template<typename T>
class Queue {
private:
    struct Node {
        T data;
        Node* next;
        Node(const T& value) : data(value), next(nullptr) {}
    };
    
    Node* front_;
    Node* back_;
    size_t size_;

public:
    Queue() : front_(nullptr), back_(nullptr), size_(0) {}
    
    ~Queue() {
        while (!empty()) {
            pop();
        }
    }

    // 拷贝构造函数
    Queue(const Queue& other) : front_(nullptr), back_(nullptr), size_(0) {
        Node* current = other.front_;
        while (current != nullptr) {
            push(current->data);
            current = current->next;
        }
    }

    // 移动构造函数
    Queue(Queue&& other) noexcept : front_(other.front_), back_(other.back_), size_(other.size_) {
        other.front_ = nullptr;
        other.back_ = nullptr;
        other.size_ = 0;
    }

    // 拷贝赋值运算符
    Queue& operator=(const Queue& other) {
        if (this != &other) {
            Queue temp(other);
            std::swap(front_, temp.front_);
            std::swap(back_, temp.back_);
            std::swap(size_, temp.size_);
        }
        return *this;
    }

    // 移动赋值运算符
    Queue& operator=(Queue&& other) noexcept {
        if (this != &other) {
            while (!empty()) {
                pop();
            }
            front_ = other.front_;
            back_ = other.back_;
            size_ = other.size_;
            other.front_ = nullptr;
            other.back_ = nullptr;
            other.size_ = 0;
        }
        return *this;
    }

    void push(const T& value) {
        Node* new_node = new Node(value);
        if (empty()) {
            front_ = back_ = new_node;
        } else {
            back_->next = new_node;
            back_ = new_node;
        }
        ++size_;
    }

    void pop() {
        if (empty()) {
            throw std::runtime_error("Queue is empty");
        }
        Node* temp = front_;
        front_ = front_->next;
        delete temp;
        --size_;
        if (empty()) {
            back_ = nullptr;
        }
    }

    T& front() {
        if (empty()) {
            throw std::runtime_error("Queue is empty");
        }
        return front_->data;
    }

    const T& front() const {
        if (empty()) {
            throw std::runtime_error("Queue is empty");
        }
        return front_->data;
    }

    bool empty() const {
        return size_ == 0;
    }

    size_t size() const {
        return size_;
    }
};

// 测试代码
int main() {
    Queue<int> q;
    q.push(1);
    q.push(2);
    q.push(3);

    std::cout << "Queue front: " << q.front() << std::endl;
    std::cout << "Queue size: " << q.size() << std::endl;

    q.pop();
    std::cout << "After pop, front: " << q.front() << std::endl;

    Queue<int> q2 = q;  // 测试拷贝构造
    std::cout << "q2 front: " << q2.front() << std::endl;

    Queue<int> q3 = std::move(q);  // 测试移动构造
    std::cout << "q3 front: " << q3.front() << std::endl;

    return 0;
} 