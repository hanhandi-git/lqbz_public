# LRU Cache (最近最少使用缓存)

## 问题链接
[LeetCode 146. LRU Cache](https://leetcode.com/problems/lru-cache/)

## 问题描述
设计并实现一个LRU（最近最少使用）缓存机制。它应该支持以下操作：获取数据get和写入数据put。

get(key)：如果关键字(key)存在于缓存中，则获取关键字的值（总是正数），否则返回 -1。
put(key, value)：如果关键字已经存在，则变更其数据值；如果关键字不存在，则插入该组「关键字-值」。当缓存容量达到上限时，它应该在写入新数据之前删除最久未使用的数据值，从而为新的数据值留出空间。

## 解题思路
1. 使用哈希表和双向链表来实现LRU缓存。
2. 哈希表用于快速查找键是否存在，双向链表用于维护缓存项的顺序。
3. 最近使用的项放在链表头部，最久未使用的项在链表尾部。
4. 当需要删除最久未使用的项时，直接删除链表尾部的项。

## 代码实现
```cpp
class LRUCache {
private:
    int capacity;
    using Node = pair<int, int>;    // 缓存中的一个项 第一个 int 是键，第二个 int 是值。 存key是为了后续删除的时候快速删除映射关系
    using Cache = list<Node>;       // 存储所有缓存项的双向链表
    using CacheIter = Cache::iterator;  // 指向链表中特定元素的迭代器。
    Cache cache; 
    unordered_map<int, CacheIter> map; // 建立了一个从缓存项的键到其在链表中位置的映射

public:
    LRUCache(int capacity) : capacity(capacity) {}
    
    int get(int key) {
        auto iter = map.find(key);
        if (iter == map.end()) return -1;
        auto& node = iter->second;
        // splice 是 std::list 的一个成员函数，用于将元素从一个列表转移到另一个列表（或同一个列表的不同位置），
        // 而不需要实际复制或移动元素的内容。这个操作非常高效，因为它只需要调整一些指针，而不需要分配或释放内存。
        // void splice(const_iterator pos, list& other, const_iterator it);
        cache.splice(cache.begin(), cache, node);
        return node->second;
    }
    
    void put(int key, int value) {
        if (capacity <= 0) return;  // 如果容量为异常直接返回
        auto iter = map.find(key);
        if (iter == map.end()) {
            // 判断是否超容量
            if (cache.size() >= capacity) {
                // 删除队尾元素
                int last_key = cache.back().first;
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
```

## 复杂度分析
- 时间复杂度：get和put操作都是O(1)。
- 空间复杂度：O(capacity)，其中capacity是缓存的容量。

## 扩展：其他缓存替换策略
1. FIFO (First In First Out)：先进先出策略
2. LFU (Least Frequently Used)：最少使用频率策略
3. Random Replacement：随机替换策略

这些策略各有优缺点，适用于不同的场景。LRU是一种在实际应用中较为常用和有效的策略。



LFUCache（最少使用频率缓存）的实现细节
LFUCache 的核心思想是淘汰使用频率最低的元素。如果有多个元素使用频率相同，则淘汰最久未使用的元素。这个实现使用了几个关键的数据结构：
这个实现使用了几个关键的数据结构：
1. `std::unordered_map<int, std::pair<int, int>> cache;`
键：缓存项的 key
值：pair 的第一个元素是缓存项的 value，第二个元素是使用频率
2. `std::unordered_map<int, std::list<int>> freq_list;`
键：使用频率
值：具有相同使用频率的 key 列表
3. `std::unordered_map<int, std::list<int>::iterator> key_iter;`
键：缓存项的 key
值：该 key 在 freq_list 中对应列表的迭代器
4. int min_freq;
记录当前最小使用频率
主要操作的实现细节
1. get 操作
    如果 key 不存在，返回 -1
    如果 key 存在
        a. 获取当前频率，并从对应频率的列表中移除该 key
        b. 增加频率，并将 key 添加到新频率的列表中
        c. 更新 key_iter 中的迭代器
        d. 更新 cache 中的频率
        e. 如果原最小频率的列表为空，增加 min_freq
2. put 操作
    如果容量为 0，直接返回
    如果 key 已存在，更新值并调用 get 来更新频率
    如果 key 不存在：
        a. 如果缓存已满，删除使用频率最低的元素
        b. 将新元素添加到缓存中，频率设为 1
        c. 将新元素添加到频率为 1 的列表中
        d. 更新 key_iter
        e. 将 min_freq 设为 1

这个实现的关键点在于：
1. 使用 freq_list 来维护相同频率的 key，这样可以快速找到并删除最少使用的元素
2. 使用 key_iter 来快速定位和更新 freq_list 中的元素位置
3. 维护 min_freq 来快速找到最小频率
这种实现方式保证了 get 和 put 操作的时间复杂度为 O(1)，同时也能够准确地实现 LFU 的淘汰策略。