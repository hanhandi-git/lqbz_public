#include <iostream>
#include <stdexcept>

template<typename T>
class DoublyLinkedList {
private:
    struct Node {
        T data;
        Node* prev;
        Node* next;
        
        Node(const T& value) : data(value), prev(nullptr), next(nullptr) {}
        Node(T&& value) : data(std::move(value)), prev(nullptr), next(nullptr) {}
    };

    Node* head_;
    Node* tail_;
    size_t size_;

public:
    // 迭代器类
    class Iterator {
    private:
        Node* current_;
        
    public:
        Iterator(Node* node) : current_(node) {}
        
        T& operator*() { return current_->data; }
        const T& operator*() const { return current_->data; }
        
        Iterator& operator++() {
            current_ = current_->next;
            return *this;
        }
        
        Iterator& operator--() {
            current_ = current_->prev;
            return *this;
        }
        
        bool operator==(const Iterator& other) const {
            return current_ == other.current_;
        }
        
        bool operator!=(const Iterator& other) const {
            return !(*this == other);
        }

        friend class DoublyLinkedList;
    };

    // 构造函数
    DoublyLinkedList() : head_(nullptr), tail_(nullptr), size_(0) {}
    
    // 析构函数
    ~DoublyLinkedList() {
        clear();
    }
    
    // 拷贝构造函数
    DoublyLinkedList(const DoublyLinkedList& other) : head_(nullptr), tail_(nullptr), size_(0) {
        for (Node* curr = other.head_; curr != nullptr; curr = curr->next) {
            push_back(curr->data);
        }
    }
    
    // 移动构造函数
    DoublyLinkedList(DoublyLinkedList&& other) noexcept
        : head_(other.head_), tail_(other.tail_), size_(other.size_) {
        other.head_ = nullptr;
        other.tail_ = nullptr;
        other.size_ = 0;
    }
    
    // 拷贝赋值运算符
    DoublyLinkedList& operator=(const DoublyLinkedList& other) {
        if (this != &other) {
            DoublyLinkedList temp(other);
            std::swap(head_, temp.head_);
            std::swap(tail_, temp.tail_);
            std::swap(size_, temp.size_);
        }
        return *this;
    }
    
    // 移动赋值运算符
    DoublyLinkedList& operator=(DoublyLinkedList&& other) noexcept {
        if (this != &other) {
            clear();
            head_ = other.head_;
            tail_ = other.tail_;
            size_ = other.size_;
            other.head_ = nullptr;
            other.tail_ = nullptr;
            other.size_ = 0;
        }
        return *this;
    }

    // 在头部插入元素
    void push_front(const T& value) {
        Node* new_node = new Node(value);
        if (empty()) {
            head_ = tail_ = new_node;
        } else {
            new_node->next = head_;
            head_->prev = new_node;
            head_ = new_node;
        }
        ++size_;
    }

    // 在尾部插入元素
    void push_back(const T& value) {
        Node* new_node = new Node(value);
        if (empty()) {
            head_ = tail_ = new_node;
        } else {
            new_node->prev = tail_;
            tail_->next = new_node;
            tail_ = new_node;
        }
        ++size_;
    }

    // 删除头部元素
    void pop_front() {
        if (empty()) {
            throw std::runtime_error("List is empty");
        }
        Node* temp = head_;
        head_ = head_->next;
        if (head_) {
            head_->prev = nullptr;
        } else {
            tail_ = nullptr;
        }
        delete temp;
        --size_;
    }

    // 删除尾部元素
    void pop_back() {
        if (empty()) {
            throw std::runtime_error("List is empty");
        }
        Node* temp = tail_;
        tail_ = tail_->prev;
        if (tail_) {
            tail_->next = nullptr;
        } else {
            head_ = nullptr;
        }
        delete temp;
        --size_;
    }

    // 在指定位置插入元素
    Iterator insert(Iterator pos, const T& value) {
        if (pos.current_ == nullptr) {
            push_back(value);
            return Iterator(tail_);
        }
        if (pos.current_ == head_) {
            push_front(value);
            return Iterator(head_);
        }
        
        Node* new_node = new Node(value);
        new_node->prev = pos.current_->prev;
        new_node->next = pos.current_;
        pos.current_->prev->next = new_node;
        pos.current_->prev = new_node;
        ++size_;
        return Iterator(new_node);
    }

    // 删除指定位置的元素
    Iterator erase(Iterator pos) {
        if (pos.current_ == nullptr) {
            throw std::runtime_error("Invalid iterator");
        }
        
        Node* next = pos.current_->next;
        if (pos.current_ == head_) {
            pop_front();
        } else if (pos.current_ == tail_) {
            pop_back();
        } else {
            pos.current_->prev->next = pos.current_->next;
            pos.current_->next->prev = pos.current_->prev;
            delete pos.current_;
            --size_;
        }
        return Iterator(next);
    }

    // 访问首元素
    T& front() {
        if (empty()) {
            throw std::runtime_error("List is empty");
        }
        return head_->data;
    }

    // 访问尾元素
    T& back() {
        if (empty()) {
            throw std::runtime_error("List is empty");
        }
        return tail_->data;
    }

    // 清空链表
    void clear() {
        while (!empty()) {
            pop_front();
        }
    }

    // 获取大小
    size_t size() const {
        return size_;
    }

    // 判断是否为空
    bool empty() const {
        return size_ == 0;
    }

    // 迭代器相关方法
    Iterator begin() { return Iterator(head_); }
    Iterator end() { return Iterator(nullptr); }
};

// 测试代码
int main() {
    DoublyLinkedList<int> list;

    // 测试push_back和push_front
    list.push_back(1);
    list.push_back(2);
    list.push_front(0);

    std::cout << "List contents: ";
    for (auto it = list.begin(); it != list.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    // 测试insert
    auto it = list.begin();
    ++it;  // 移动到1的位置
    list.insert(it, 5);

    std::cout << "After insert: ";
    for (const auto& value : list) {
        std::cout << value << " ";
    }
    std::cout << std::endl;

    // 测试erase
    it = list.begin();
    ++it;  // 移动到要删除的位置
    list.erase(it);

    std::cout << "After erase: ";
    for (const auto& value : list) {
        std::cout << value << " ";
    }
    std::cout << std::endl;

    // 测试拷贝构造
    DoublyLinkedList<int> list2 = list;
    std::cout << "Copy constructed list: ";
    for (const auto& value : list2) {
        std::cout << value << " ";
    }
    std::cout << std::endl;

    // 测试移动构造
    DoublyLinkedList<int> list3 = std::move(list);
    std::cout << "Move constructed list: ";
    for (const auto& value : list3) {
        std::cout << value << " ";
    }
    std::cout << std::endl;

    // 测试front和back
    std::cout << "Front: " << list3.front() << std::endl;
    std::cout << "Back: " << list3.back() << std::endl;

    return 0;
} 