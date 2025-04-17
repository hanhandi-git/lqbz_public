#include <iostream>
#include <chrono>
#include <unordered_map>
#include <random>
#include <iomanip>
#include "cache_aware_hash.hpp"

using namespace std;
using namespace std::chrono;

/**
 * @brief 测试哈希表的缓存优化效果
 */
class HashTester {
private:
    static const size_t TEST_SIZE = 1000000;
    static const size_t ITERATIONS = 100;
    vector<int> test_data;
    vector<int> lookup_data;
    
public:
    HashTester() {
        // 生成测试数据
        test_data.resize(TEST_SIZE);
        lookup_data.resize(TEST_SIZE);
        
        for(size_t i = 0; i < TEST_SIZE; i++) {
            test_data[i] = i;
        }
        
        // 随机打乱
        random_device rd;
        mt19937 gen(rd());
        shuffle(test_data.begin(), test_data.end(), gen);
        
        // 生成查找数据
        for(size_t i = 0; i < TEST_SIZE; i++) {
            lookup_data[i] = test_data[i % (TEST_SIZE/2)];  // 50%命中率
        }
        shuffle(lookup_data.begin(), lookup_data.end(), gen);
    }
    
    void runTest() {
        cout << "开始哈希表性能测试...\n" << endl;
        
        auto std_time = testStdHash();
        auto cache_time = testCacheAwareHash();
        auto batch_time = testBatchLookup();
        
        cout << "\n测试结果汇总:" << endl;
        cout << "----------------------------------------" << endl;
        cout << setw(20) << "实现方式" << setw(15) << "耗时(ms)" << endl;
        cout << "----------------------------------------" << endl;
        cout << setw(20) << "std::unordered_map" << setw(15) << std_time << endl;
        cout << setw(20) << "cache_aware_hash" << setw(15) << cache_time << endl;
        cout << setw(20) << "batch_lookup" << setw(15) << batch_time << endl;
        cout << "----------------------------------------" << endl;
        
        if(std_time > 0) {
            double speedup = static_cast<double>(std_time) / cache_time;
            cout << "缓存优化提升: " << fixed << setprecision(2) 
                 << speedup << "x" << endl;
            
            speedup = static_cast<double>(std_time) / batch_time;
            cout << "批量查找提升: " << fixed << setprecision(2) 
                 << speedup << "x" << endl;
        }
    }
    
private:
    double testStdHash() {
        cout << "测试std::unordered_map..." << endl;
        
        auto start = high_resolution_clock::now();
        
        for(size_t iter = 0; iter < ITERATIONS; iter++) {
            unordered_map<int, int> hash;
            // 插入测试
            for(size_t i = 0; i < TEST_SIZE; i++) {
                hash[test_data[i]] = test_data[i];
            }
            // 查找测试
            volatile int sum = 0;
            for(size_t i = 0; i < TEST_SIZE; i++) {
                auto it = hash.find(lookup_data[i]);
                if(it != hash.end()) {
                    sum += it->second;
                }
            }
        }
        
        auto end = high_resolution_clock::now();
        return duration_cast<milliseconds>(end - start).count();
    }
    
    double testCacheAwareHash() {
        cout << "测试cache_aware_hash..." << endl;
        
        auto start = high_resolution_clock::now();
        
        for(size_t iter = 0; iter < ITERATIONS; iter++) {
            CacheAwareHashTable<int, int> hash;
            // 插入测试
            for(size_t i = 0; i < TEST_SIZE; i++) {
                hash.insert(test_data[i], test_data[i]);
            }
            // 查找测试
            volatile int sum = 0;
            int value;
            for(size_t i = 0; i < TEST_SIZE; i++) {
                if(hash.find(lookup_data[i], value)) {
                    sum += value;
                }
            }
        }
        
        auto end = high_resolution_clock::now();
        return duration_cast<milliseconds>(end - start).count();
    }
    
    double testBatchLookup() {
        cout << "测试批量查找优化..." << endl;
        
        auto start = high_resolution_clock::now();
        
        for(size_t iter = 0; iter < ITERATIONS; iter++) {
            CacheAwareHashTable<int, int> hash;
            // 插入测试
            for(size_t i = 0; i < TEST_SIZE; i++) {
                hash.insert(test_data[i], test_data[i]);
            }
            // 批量查找测试
            volatile int sum = 0;
            auto results = hash.batchLookup(lookup_data);
            for(const auto& value : results) {
                sum += value;
            }
        }
        
        auto end = high_resolution_clock::now();
        return duration_cast<milliseconds>(end - start).count();
    }
};

int main() {
    HashTester tester;
    tester.runTest();
    return 0;
} 