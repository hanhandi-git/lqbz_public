#include <iostream>
#include <list>
#include <unordered_map>
#include <functional>

class LRUCache {
private:
    using Key = int;
    using Value = int;
    using Node = std::pair<Key, Value>;
    using Cache = std::list<Node>;
    using CacheIter = Cache::iterator;
    
    int capacity;
    Cache cache;
    std::unordered_map<Key, CacheIter> map;

public:
    LRUCache(int capacity) : capacity(capacity) {}
    
    Value get(Key key) {
        auto iter = map.find(key);
        if (iter == map.end()) return -1;
        auto& node = iter->second; // 找到node的iter
        // 将其移动到链表头部
        cache.splice(cache.begin(), cache, node);
        return node->second;
    }
    
    void put(Key key, Value value) {
        if (capacity <= 0) return;  // 如果容量为异常直接返回
        auto iter = map.find(key);
        if (iter == map.end()) {
            // 判断是否超容量
            if (cache.size() >= capacity) {
                // 删除队尾元素
                Key last_key = cache.back().first;
                cache.pop_back();
                map.erase(last_key);
            }
            // 插入到头部
            cache.push_front({key, value});
            // 添加到映射关系
            map[key] = cache.begin();
        } else {
            // 更新
            auto& node = iter->second;
            node->second = value;
            cache.splice(cache.begin(), cache, node);
        }
    }
};

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
        // 原有的测试用例
        runTest("TestLRUCache", [this]() {
            LRUCache cache(2);
            
            cache.put(1, 1);
            cache.put(2, 2);
            if (cache.get(1) != 1) return false;  // 返回 1
            
            cache.put(3, 3);  // 该操作会使得关键字 2 作废
            if (cache.get(2) != -1) return false;  // 返回 -1 (未找到)
            
            cache.put(4, 4);  // 该操作会使得关键字 1 作废
            if (cache.get(1) != -1) return false;  // 返回 -1 (未找到)
            if (cache.get(3) != 3) return false;  // 返回 3
            if (cache.get(4) != 4) return false;  // 返回 4
            
            return true;
        });

        runTest("TestLRUCacheCapacityOne", [this]() {
            LRUCache cache(1);
            
            cache.put(2, 1);
            if (cache.get(2) != 1) return false;  // 返回 1
            
            cache.put(3, 2);
            if (cache.get(2) != -1) return false;  // 返回 -1 (未找到)
            if (cache.get(3) != 2) return false;  // 返回 2
            
            return true;
        });

        runTest("TestLRUCacheUpdateExistingKey", [this]() {
            LRUCache cache(2);
            
            cache.put(1, 1);
            cache.put(2, 2);
            if (cache.get(1) != 1) return false;  // 返回 1
            
            cache.put(2, 3);  // 更新已存在的键
            if (cache.get(2) != 3) return false;  // 返回 3
            
            cache.put(4, 4);  // 该操作会使得关键字 1 作废
            if (cache.get(1) != -1) return false;  // 返回 -1 (未找到)
            if (cache.get(2) != 3) return false;  // 返回 3
            
            return true;
        });

        // 新增的测试用例
        runTest("TestLRUCacheFrequentAccess", [this]() {
            LRUCache cache(3);
            
            cache.put(1, 1);
            cache.put(2, 2);
            cache.put(3, 3);
            cache.get(1);  // 访问1，使其成为最近使用的
            cache.put(4, 4);  // 这应该会淘汰2
            
            if (cache.get(2) != -1) return false;  // 2应该被淘汰
            if (cache.get(1) != 1) return false;  // 1应该还在
            if (cache.get(3) != 3) return false;  // 3应该还在
            if (cache.get(4) != 4) return false;  // 4应该在
            
            return true;
        });

        runTest("TestLRUCacheOverwriteExisting", [this]() {
            LRUCache cache(2);
            
            cache.put(1, 1);
            cache.put(2, 2);
            cache.put(1, 10);  // 覆盖已存在的键
            cache.put(3, 3);  // 这应该会淘汰2
            
            if (cache.get(1) != 10) return false;  // 1的值应该被更新为10
            if (cache.get(2) != -1) return false;  // 2应该被淘汰
            if (cache.get(3) != 3) return false;  // 3应该在
            
            return true;
        });

        runTest("TestLRUCacheEmptyCache", [this]() {
            LRUCache cache(0);
            
            cache.put(1, 1);
            if (cache.get(1) != -1) return false;  // 缓存容量为0，所有操作都应该失败
            
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
