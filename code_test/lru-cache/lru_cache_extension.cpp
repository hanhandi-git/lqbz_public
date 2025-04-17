#include <iostream>
#include <list>
#include <unordered_map>
#include <queue>
#include <random>
#include <functional>

// 1. FIFO (First In First Out) Cache
class FIFOCache {
private:
    int capacity;
    using Key = int;
    using Value = int;
    using Node = std::pair<Key, Value>;
    using Cache = std::queue<Node>;     //  先进先出用queue
    Cache cache;
    std::unordered_map<Key, Value> map;   // 这里不存迭代器了，直接存value
    // 为什么 LRUCache 在 map 中存储迭代器，而 FIFOCache 直接存储值
    // LRUCache（最近最少使用）：
    // * 需要频繁地将最近访问的元素移动到链表的头部。
    // * 使用双向链表（std::list）来存储元素，因为需要在任意位置快速插入和删除。
    // * 在 map 中存储迭代器是为了能够快速定位到链表中的元素，并进行 O(1) 时间复杂度的移动操作。
    // * 使用 splice 操作可以高效地将元素移动到链表头部，而不需要重新分配内存。
    // FIFOCache（先进先出）：
    // * 只需要在队列的一端添加元素，在另一端删除元素。
    // * 使用队列（std::queue）来存储元素，因为只需要在两端操作。
    // * 不需要在中间位置进行操作，所以不需要快速定位到特元素。
    // * 在 map 中直接存储值就足够了，因为不需要移动元素。
public:
    FIFOCache(int capacity) : capacity(capacity) {}
    
    Value get(Key key) {
        if (map.find(key) == map.end()) return -1;
        return map[key];
    }
    
    void put(Key key, Value value) {
        if (map.find(key) == map.end()) {
            if (cache.size() == capacity) {
                map.erase(cache.front().first);
                cache.pop();
            }
            cache.push({key, value});
        }
        map[key] = value;
    }
};

// 2. LFU (Least Frequently Used) Cache
class LFUCache {
private:
    int capacity;
    using Key = int;
    using Frequency = int;
    using Value = int;
    using Node = std::pair<Value, Frequency>;   // node = {value, frequency}
    using KeyList = std::list<Key>;
    std::unordered_map<Key, Node> key_2_node_map;  // key -> node
    std::unordered_map<Frequency, KeyList> freq_2_keylist_map; // frequency -> list of keys
    std::unordered_map<Key, KeyList::iterator> key_2_keylist_iter_map; // key -> iterator in freq_list
    int min_freq;

private:
    void increaseFrequency(Key key) {
        // std::cout << "Increasing frequency for key: " << key << std::endl;
        auto node_it = key_2_node_map.find(key);
        if (node_it == key_2_node_map.end()) {
            // std::cout << "Error: Key not found in key_2_node_map" << std::endl;
            return;
        }
        Node& node = node_it->second;
        Frequency oldFreq = node.second;
        Frequency newFreq = oldFreq + 1;
        // std::cout << "Old frequency: " << oldFreq << ", New frequency: " << newFreq << std::endl;

        // 从旧频率列表中移除
        auto freq_it = freq_2_keylist_map.find(oldFreq);
        if (freq_it != freq_2_keylist_map.end()) {
            auto key_it = key_2_keylist_iter_map.find(key);
            if (key_it != key_2_keylist_iter_map.end()) {
                freq_it->second.erase(key_it->second);
                if (freq_it->second.empty()) {
                    freq_2_keylist_map.erase(oldFreq);
                    if (oldFreq == min_freq) {
                        min_freq = newFreq;
                        // std::cout << "Updated min_freq to: " << min_freq << std::endl;
                    }
                }
            } else {
                // std::cout << "Warning: Key not found in key_2_keylist_iter_map" << std::endl;
            }
        } else {
            // std::cout << "Warning: Old frequency not found in freq_2_keylist_map" << std::endl;
        }

        // 添加到新频率列表
        // 为什么把 key 添加到对应 list 的末尾 这是为了维护 LFU（最不经常使用）缓存的淘汰顺序。在同一频率下，我们需要一种方式来决定哪个元素应该先被淘汰。
        // 通过将新增加频率的元素添加到列表末尾，我们实现了一个简单的 LRU（最近最少使用）策略在同频率元素中的应用。
        // 列表前端的元素是该频率下最早被添加或更新的，因此在需要淘汰时应该首先考虑这些元素。
        // 列表后端的元素是该频率下最近被添加或更新的，应该最后被考虑淘汰。
        freq_2_keylist_map[newFreq].push_back(key);
        // 这行代码的目的是获取指向新插入元素的迭代器
        // freq_2_keylist_map[newFreq].end() 返回一个指向列表末尾之后的迭代器
        // 对这个迭代器使用 -- 操作，我们得到一个指向列表最后一个元素的迭代器
        // 由于我们刚刚使用 push_back 将元素添加到列表末尾，这个迭代器正好指向我们刚插入的元素
        key_2_keylist_iter_map[key] = --freq_2_keylist_map[newFreq].end();

        // 更新节点频率
        node.second = newFreq;
        // std::cout << "Frequency increased successfully" << std::endl;
    }

public:
    LFUCache(int capacity) : capacity(std::max(0, capacity)), min_freq(0) {
        // std::cout << "LFUCache created with capacity: " << this->capacity << std::endl;
    }
    
    Value get(Key key) {
        // std::cout << "Attempting to get key: " << key << std::endl;
        if (key_2_node_map.find(key) == key_2_node_map.end()) {
            // std::cout << "Key not found" << std::endl;
            return -1;
        }
        increaseFrequency(key);
        Value value = key_2_node_map[key].first;
        // std::cout << "Returning value: " << value << std::endl;
        return value;
    }
    
    void put(Key key, Value value) {
        // std::cout << "Attempting to put key: " << key << ", value: " << value << std::endl;
        if (capacity <= 0) {
            // std::cout << "Cache capacity is 0, cannot put" << std::endl;
            return;
        }
        if (key_2_node_map.find(key) != key_2_node_map.end()) {
            // std::cout << "Key already exists, updating value" << std::endl;
            key_2_node_map[key].first = value; // 更新value
            increaseFrequency(key);
            return;
        }
        if (key_2_node_map.size() == capacity) {
            // std::cout << "Cache is full, need to evict" << std::endl;
            if (freq_2_keylist_map[min_freq].empty()) {
                // std::cout << "Warning: Unexpected state - min_freq list is empty" << std::endl;
                return;
            }
            int evict = freq_2_keylist_map[min_freq].front();
            // std::cout << "Evicting key: " << evict << std::endl;
            freq_2_keylist_map[min_freq].pop_front();
            key_2_node_map.erase(evict);
            key_2_keylist_iter_map.erase(evict);
        }
        // std::cout << "Adding new key-value pair" << std::endl;
        key_2_node_map[key] = {value, 0};
        increaseFrequency(key);
        min_freq = 1;
        // std::cout << "Put operation completed successfully" << std::endl;
    }
};

// 3. Random Replacement Cache
class RandomCache {
private:
    int capacity;
    std::vector<std::pair<int, int>> cache;
    std::unordered_map<int, int> map;
    std::mt19937 gen;
    std::uniform_int_distribution<> dis;

public:
    RandomCache(int capacity) : capacity(capacity), gen(std::random_device{}()), dis(0, capacity - 1) {}
    
    int get(int key) {
        if (map.find(key) == map.end()) return -1;
        return cache[map[key]].second;
    }
    
    void put(int key, int value) {
        if (map.find(key) != map.end()) {
            cache[map[key]].second = value;
        } else {
            if (cache.size() == capacity) {
                int index = dis(gen);
                map.erase(cache[index].first);
                cache[index] = {key, value};
                map[key] = index;
            } else {
                map[key] = cache.size();
                cache.push_back({key, value});
            }
        }
    }
};

// Test Framework
class TestFramework {
private:
    int passed = 0;
    int total = 0;

    void runTest(const std::string& testName, std::function<bool()> test) {
        total++;
        if (test()) {
            std::cout << "PASS: " << testName << std::endl;
            passed++;
        } else {
            std::cout << "FAIL: " << testName << std::endl;
        }
    }

public:
    void runAllTests() {
        // FIFO Cache Tests
        runTest("TestFIFOCache", [this]() {
            FIFOCache cache(2);
            cache.put(1, 1);
            cache.put(2, 2);
            if (cache.get(1) != 1) return false;
            cache.put(3, 3);
            if (cache.get(1) != -1) return false;  // 1 应该被淘汰
            if (cache.get(2) != 2) return false;   // 2 应该还在
            if (cache.get(3) != 3) return false;   // 3 刚刚加入，应该在
            return true;
        });

        // LFU Cache Tests
        runTest("TestLFUCache", [this]() {
            LFUCache cache(2);
            cache.put(1, 1);
            cache.put(2, 2);
            if (cache.get(1) != 1) return false;
            cache.put(3, 3);
            if (cache.get(2) != -1) return false;
            if (cache.get(3) != 3) return false;
            cache.put(4, 4);
            if (cache.get(1) != -1) return false;
            if (cache.get(3) != 3) return false;
            if (cache.get(4) != 4) return false;
            return true;
        });

        // 新增的 LFU Cache 测试用例 
        // 测试容量为1的LFU缓存
        runTest("TestLFUCacheCapacityOne", [this]() {
            LFUCache cache(1);
            cache.put(2, 1);
            if (cache.get(2) != 1) return false;
            cache.put(3, 2);
            if (cache.get(2) != -1) return false;
            if (cache.get(3) != 2) return false;
            return true;
        });

        // 测试当所有元素频率相同时的行为
        runTest("TestLFUCacheFrequencyTie", [this]() {
            LFUCache cache(3);
            cache.put(1, 1);
            cache.put(2, 2);
            cache.put(3, 3);
            cache.get(1);
            cache.get(2);
            cache.get(3);
            cache.put(4, 4);  // This should evict key 1
            if (cache.get(1) != -1) return false;
            if (cache.get(2) != 2) return false;
            if (cache.get(3) != 3) return false;
            if (cache.get(4) != 4) return false;
            return true;
        });

        // 测试更新已存在键值对时的行为
        runTest("TestLFUCacheUpdateExisting", [this]() {
            LFUCache cache(2);
            cache.put(1, 1);
            cache.put(2, 2);
            cache.get(1);
            cache.put(3, 3);  // This should evict key 2
            cache.get(2);
            cache.put(4, 4);  // This should evict key 3
            if (cache.get(1) != 1) return false;
            if (cache.get(2) != -1) return false;
            if (cache.get(3) != -1) return false;
            if (cache.get(4) != 4) return false;
            return true;
        });

        // 测试容量为0的边界情况
        runTest("TestLFUCacheZeroCapacity", [this]() {
            LFUCache cache(0);
            cache.put(0, 0);
            if (cache.get(0) != -1) return false;
            return true;
        });

        // 测试不同访问频率下的淘汰顺序
        runTest("TestLFUCacheFrequencyOrder", [this]() {
            LFUCache cache(3);
            cache.put(1, 1);
            cache.put(2, 2);
            cache.put(3, 3);
            cache.get(1);
            cache.get(2);
            cache.get(2);
            cache.put(4, 4);  // This should evict key 3
            if (cache.get(1) != 1) return false;
            if (cache.get(2) != 2) return false;
            if (cache.get(3) != -1) return false;
            if (cache.get(4) != 4) return false;
            return true;
        });

        // Random Replacement Cache Tests
        runTest("TestRandomCache", [this]() {
            RandomCache cache(2);
            cache.put(1, 1);
            cache.put(2, 2);
            if (cache.get(1) == -1 && cache.get(2) == -1) return false;
            cache.put(3, 3);
            if (cache.get(1) != -1 && cache.get(2) != -1 && cache.get(3) != -1) return false;
            return true;
        });

        std::cout << "\n测试结果: " << passed << " 通过, " 
                  << (total - passed) << " 失败, " 
                  << total << " 总计" << std::endl;
    }
};

int main() {
    TestFramework testFramework;
    testFramework.runAllTests();
    return 0;
}