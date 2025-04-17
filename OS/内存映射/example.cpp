#include <iostream>
#include "lru_cache.cpp"

int main() {
    MappedLRUCache<std::string, int> cache("cache.dat", 1000);
    
    // 存储数据
    cache.put("key1", 100);
    cache.put("key2", 200);
    
    // 读取数据
    int value;
    if (cache.get("key1", value)) {
        std::cout << "找到值: " << value << std::endl;
    }
    
    return 0;
} 