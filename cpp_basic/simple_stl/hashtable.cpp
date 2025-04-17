#include <iostream>
#include <stdexcept>
#include <functional>

template<typename K, typename V>
class HashTable {
private:
    // 哈希表中的节点
    struct Node {
        K key;
        V value;
        bool occupied;  // 标记该位置是否被占用
        bool deleted;   // 标记该位置是否被删除（用于开放地址法）

        Node() : occupied(false), deleted(false) {}
    };

    Node* table_;
    size_t capacity_;
    size_t size_;
    static constexpr double LOAD_FACTOR_THRESHOLD = 0.75;  // 负载因子阈值

    // 计算哈希值
    size_t hash(const K& key) const {
        return std::hash<K>{}(key) % capacity_;
    }

    // 查找key应该在的位置
    size_t findPos(const K& key) const {
        size_t pos = hash(key);
        size_t originalPos = pos;
        
        while (table_[pos].occupied) {
            if (table_[pos].key == key && !table_[pos].deleted) {
                return pos;
            }
            pos = (pos + 1) % capacity_;
            if (pos == originalPos) {
                throw std::runtime_error("HashTable is full");
            }
        }
        return pos;
    }

    // 重新哈希
    void rehash() {
        size_t oldCapacity = capacity_;
        Node* oldTable = table_;

        capacity_ *= 2;
        table_ = new Node[capacity_];
        size_ = 0;

        for (size_t i = 0; i < oldCapacity; ++i) {
            if (oldTable[i].occupied && !oldTable[i].deleted) {
                insert(oldTable[i].key, oldTable[i].value);
            }
        }

        delete[] oldTable;
    }

public:
    HashTable(size_t initial_capacity = 16) 
        : capacity_(initial_capacity), size_(0) {
        table_ = new Node[capacity_];
    }

    ~HashTable() {
        delete[] table_;
    }

    // 拷贝构造函数
    HashTable(const HashTable& other) 
        : capacity_(other.capacity_), size_(other.size_) {
        table_ = new Node[capacity_];
        for (size_t i = 0; i < capacity_; ++i) {
            table_[i] = other.table_[i];
        }
    }

    // 移动构造函数
    HashTable(HashTable&& other) noexcept
        : table_(other.table_), capacity_(other.capacity_), size_(other.size_) {
        other.table_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
    }

    // 拷贝赋值运算符
    HashTable& operator=(const HashTable& other) {
        if (this != &other) {
            delete[] table_;
            capacity_ = other.capacity_;
            size_ = other.size_;
            table_ = new Node[capacity_];
            for (size_t i = 0; i < capacity_; ++i) {
                table_[i] = other.table_[i];
            }
        }
        return *this;
    }

    // 移动赋值运算符
    HashTable& operator=(HashTable&& other) noexcept {
        if (this != &other) {
            delete[] table_;
            table_ = other.table_;
            capacity_ = other.capacity_;
            size_ = other.size_;
            other.table_ = nullptr;
            other.size_ = 0;
            other.capacity_ = 0;
        }
        return *this;
    }

    // 插入键值对
    void insert(const K& key, const V& value) {
        if (static_cast<double>(size_ + 1) / capacity_ > LOAD_FACTOR_THRESHOLD) {
            rehash();
        }

        size_t pos = findPos(key);
        if (!table_[pos].occupied || table_[pos].deleted) {
            ++size_;
        }
        table_[pos].key = key;
        table_[pos].value = value;
        table_[pos].occupied = true;
        table_[pos].deleted = false;
    }

    // 删除键值对
    void erase(const K& key) {
        size_t pos = findPos(key);
        if (table_[pos].occupied && !table_[pos].deleted && table_[pos].key == key) {
            table_[pos].deleted = true;
            --size_;
        }
    }

    // 查找值
    V& operator[](const K& key) {
        size_t pos = findPos(key);
        if (!table_[pos].occupied || table_[pos].deleted) {
            insert(key, V());
            pos = findPos(key);
        }
        return table_[pos].value;
    }

    // 判断键是否存在
    bool contains(const K& key) const {
        try {
            size_t pos = findPos(key);
            return table_[pos].occupied && !table_[pos].deleted && table_[pos].key == key;
        } catch (const std::runtime_error&) {
            return false;
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

    // 清空哈希表
    void clear() {
        delete[] table_;
        table_ = new Node[capacity_];
        size_ = 0;
    }
};

// 测试代码
int main() {
    HashTable<std::string, int> ht;

    // 测试插入
    ht.insert("apple", 1);
    ht.insert("banana", 2);
    ht.insert("orange", 3);

    // 测试查找
    std::cout << "apple: " << ht["apple"] << std::endl;
    std::cout << "banana: " << ht["banana"] << std::endl;
    std::cout << "orange: " << ht["orange"] << std::endl;

    // 测试contains
    std::cout << "Contains apple: " << std::boolalpha << ht.contains("apple") << std::endl;
    std::cout << "Contains grape: " << ht.contains("grape") << std::endl;

    // 测试删除
    ht.erase("banana");
    std::cout << "After erasing banana, contains banana: " << ht.contains("banana") << std::endl;

    // 测试拷贝构造
    HashTable<std::string, int> ht2 = ht;
    std::cout << "ht2 apple: " << ht2["apple"] << std::endl;

    // 测试移动构造
    HashTable<std::string, int> ht3 = std::move(ht);
    std::cout << "ht3 apple: " << ht3["apple"] << std::endl;

    // 测试大小
    std::cout << "Size of ht3: " << ht3.size() << std::endl;

    return 0;
} 