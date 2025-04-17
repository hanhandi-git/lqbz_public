#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <string>
#include <list>
#include <unordered_map>
#include <mutex>

template<typename K, typename V>
class MappedLRUCache {
private:
    struct CacheEntry {
        K key;
        V value;
        size_t timestamp;
    };

    std::mutex mtx;
    std::list<CacheEntry> lru_list;
    std::unordered_map<K, typename std::list<CacheEntry>::iterator> cache_map;
    void* mapped_memory;
    size_t max_size;
    int fd;
    const char* filename;

public:
    MappedLRUCache(const char* fname, size_t size) : max_size(size), filename(fname) {
        // 打开或创建映射文件
        fd = open(filename, O_RDWR | O_CREAT, 0666);
        if (fd == -1) {
            throw std::runtime_error("无法打开或创建文件");
        }

        // 设置文件大小
        ftruncate(fd, max_size * sizeof(CacheEntry));

        // 创建内存映射
        mapped_memory = mmap(nullptr, max_size * sizeof(CacheEntry),
                           PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        if (mapped_memory == MAP_FAILED) {
            close(fd);
            throw std::runtime_error("内存映射失败");
        }

        // 加载已存在的缓存数据
        loadExistingData();
    }

    ~MappedLRUCache() {
        munmap(mapped_memory, max_size * sizeof(CacheEntry));
        close(fd);
    }

    void put(const K& key, const V& value) {
        std::lock_guard<std::mutex> lock(mtx);

        auto it = cache_map.find(key);
        if (it != cache_map.end()) {
            // 更新现有项
            lru_list.erase(it->second);
        } else if (cache_map.size() >= max_size) {
            // 移除最久未使用的项
            auto last = lru_list.back();
            cache_map.erase(last.key);
            lru_list.pop_back();
        }

        // 添加新项到前面
        lru_list.push_front({key, value, time(nullptr)});
        cache_map[key] = lru_list.begin();

        // 同步到映射内存
        syncToMappedMemory();
    }

    bool get(const K& key, V& value) {
        std::lock_guard<std::mutex> lock(mtx);

        auto it = cache_map.find(key);
        if (it == cache_map.end()) {
            return false;
        }

        // 移动到列表前端
        lru_list.splice(lru_list.begin(), lru_list, it->second);
        value = it->second->value;
        return true;
    }

private:
    void loadExistingData() {
        CacheEntry* entries = static_cast<CacheEntry*>(mapped_memory);
        for (size_t i = 0; i < max_size; ++i) {
            if (entries[i].timestamp != 0) {
                lru_list.push_back(entries[i]);
                cache_map[entries[i].key] = std::prev(lru_list.end());
            }
        }
    }

    void syncToMappedMemory() {
        CacheEntry* entries = static_cast<CacheEntry*>(mapped_memory);
        size_t i = 0;
        for (const auto& entry : lru_list) {
            entries[i++] = entry;
        }
        msync(mapped_memory, max_size * sizeof(CacheEntry), MS_SYNC);
    }
}; 