#include <iostream>
#include <chrono>
#include <vector>
#include <random>
#include <iomanip>

using namespace std;
using namespace std::chrono;

/**
 * @brief 测试不同内存访问模式的性能差异
 */
class MemoryAccessTester {
private:
    vector<int> data;
    const size_t size;
    const size_t iterations;
    
public:
    MemoryAccessTester(size_t s = 1024*1024, size_t iter = 1000) 
        : size(s), iterations(iter) {
        data.resize(size);
        // 初始化数据
        for(size_t i = 0; i < size; i++) {
            data[i] = i;
        }
    }
    
    void runAllTests() {
        cout << "开始内存访问测试...\n" << endl;
        
        auto l1_time = testL1CacheAccess();
        auto seq_time = testSequentialAccess();
        auto rand_time = testRandomAccess();
        
        cout << "\n测试结果汇总:" << endl;
        cout << "----------------------------------------" << endl;
        cout << setw(20) << "访问类型" << setw(15) << "平均时间(ns)" << endl;
        cout << "----------------------------------------" << endl;
        cout << setw(20) << "L1缓存访问" << setw(15) << l1_time << endl;
        cout << setw(20) << "顺序访问" << setw(15) << seq_time << endl;
        cout << setw(20) << "随机访问" << setw(15) << rand_time << endl;
        cout << "----------------------------------------" << endl;
    }
    
private:
    double testL1CacheAccess() {
        cout << "测试L1缓存访问..." << endl;
        
        const int L1_CACHE_SIZE = 16384; // 16K个整数 = 64KB
        auto start = high_resolution_clock::now();
        volatile int sum = 0;
        
        for(size_t i = 0; i < iterations; i++) {
            // 0x3FFF = 16383，保证访问范围在L1缓存大小内
            sum += data[i & (L1_CACHE_SIZE-1)];  // 等价于 i & 0x3FFF
        }
        
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<nanoseconds>(end - start).count();
        
        return static_cast<double>(duration) / iterations;
    }
    
    double testSequentialAccess() {
        cout << "测试顺序访问..." << endl;
        
        auto start = high_resolution_clock::now();
        volatile int sum = 0;
        
        for(size_t i = 0; i < iterations; i++) {
            sum += data[i % size];
        }
        
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<nanoseconds>(end - start).count();
        
        return static_cast<double>(duration) / iterations;
    }
    
    double testRandomAccess() {
        cout << "测试随机访问..." << endl;
        
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> dis(0, size - 1);
        
        auto start = high_resolution_clock::now();
        volatile int sum = 0;
        
        for(size_t i = 0; i < iterations; i++) {
            sum += data[dis(gen)];
        }
        
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<nanoseconds>(end - start).count();
        
        return static_cast<double>(duration) / iterations;
    }
};

int main() {
    MemoryAccessTester tester;
    tester.runAllTests();
    return 0;
} 