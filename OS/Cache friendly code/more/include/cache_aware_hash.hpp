#pragma once
#include <vector>
#include <array>
#include <algorithm>
#include <functional>

using std::vector;
using std::array;

template<typename K, typename V>
class CacheAwareHashTable {
private:
    static const size_t BUCKET_COUNT = 1 << 16;
    static const size_t CACHE_LINE_SIZE = 64;
    static const size_t CHUNK_SIZE = 8;  // 每个chunk存储多个键值对
    
    struct alignas(CACHE_LINE_SIZE) Chunk {
        array<K, CHUNK_SIZE> keys;
        array<V, CHUNK_SIZE> values;
        array<bool, CHUNK_SIZE> occupied;
        Chunk* next;
        
        Chunk() : next(nullptr) {
            occupied.fill(false);
        }
    };
    
    // 确保哈希桶数组按缓存行对齐
    vector<Chunk*> buckets;
    size_t size_;
    
    size_t hash(const K& key) const {
        return std::hash<K>{}(key) & (BUCKET_COUNT - 1);
    }
    
public:
    CacheAwareHashTable() : buckets(BUCKET_COUNT), size_(0) {
        for(auto& bucket : buckets) {
            bucket = new Chunk();
        }
    }
    
    ~CacheAwareHashTable() {
        for(auto bucket : buckets) {
            while(bucket) {
                Chunk* next = bucket->next;
                delete bucket;
                bucket = next;
            }
        }
    }
    
    void insert(const K& key, const V& value) {
        size_t h = hash(key);
        Chunk* chunk = buckets[h];
        
        // 查找可用槽位
        while(chunk) {
            for(size_t i = 0; i < CHUNK_SIZE; i++) {
                if(!chunk->occupied[i]) {
                    chunk->keys[i] = key;
                    chunk->values[i] = value;
                    chunk->occupied[i] = true;
                    size_++;
                    return;
                }
            }
            if(!chunk->next) {
                chunk->next = new Chunk();
            }
            chunk = chunk->next;
        }
    }
    
    bool find(const K& key, V& value) const {
        size_t h = hash(key);
        Chunk* chunk = buckets[h];
        
        while(chunk) {
            for(size_t i = 0; i < CHUNK_SIZE; i++) {
                if(chunk->occupied[i] && chunk->keys[i] == key) {
                    value = chunk->values[i];
                    return true;
                }
            }
            chunk = chunk->next;
        }
        return false;
    }
    
    // 批量查找优化
    vector<V> batchLookup(const vector<K>& keys) {
        vector<V> results(keys.size());
        // 对查询键值进行排序以优化访问模式
        vector<std::pair<size_t, size_t>> sorted_indices;
        sorted_indices.reserve(keys.size());
        
        for(size_t i = 0; i < keys.size(); i++) {
            sorted_indices.emplace_back(hash(keys[i]), i);
        }
        std::sort(sorted_indices.begin(), sorted_indices.end());
        
        // 批量查找
        for(const auto& p : sorted_indices) {
            V value;
            if(find(keys[p.second], value)) {
                results[p.second] = value;
            }
        }
        return results;
    }
    
    size_t size() const { return size_; }
}; 