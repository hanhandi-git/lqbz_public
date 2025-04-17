#include <iostream>
#include <chrono>
#include <list>
#include <iomanip>
#include "cache_aware_list.hpp"

using namespace std;
using namespace std::chrono;

class ListTester {
private:
    static const size_t TEST_SIZE = 1000000;
    static const size_t ITERATIONS = 100;
    
public:
    void runTest() {
        cout << "开始链表性能测试...\n" << endl;
        
        auto std_time = testStdList();
        auto cache_time = testCacheAwareList();
        
        cout << "\n测试结果汇总:" << endl;
        cout << "----------------------------------------" << endl;
        cout << setw(20) << "实现方式" << setw(15) << "耗时(ms)" << endl;
        cout << "----------------------------------------" << endl;
        cout << setw(20) << "std::list" << setw(15) << std_time << endl;
        cout << setw(20) << "cache_aware_list" << setw(15) << cache_time << endl;
        cout << "----------------------------------------" << endl;
        
        if(std_time > 0) {
            double speedup = static_cast<double>(std_time) / cache_time;
            cout << "性能提升: " << fixed << setprecision(2) 
                 << speedup << "x" << endl;
        }
    }
    
private:
    double testStdList() {
        cout << "测试std::list..." << endl;
        
        auto start = high_resolution_clock::now();
        
        for(size_t iter = 0; iter < ITERATIONS; iter++) {
            list<int> lst;
            // 插入测试
            for(size_t i = 0; i < TEST_SIZE; i++) {
                lst.push_front(i);
            }
            // 删除测试
            while(!lst.empty()) {
                lst.pop_front();
            }
        }
        
        auto end = high_resolution_clock::now();
        return duration_cast<milliseconds>(end - start).count();
    }
    
    double testCacheAwareList() {
        cout << "测试cache_aware_list..." << endl;
        
        auto start = high_resolution_clock::now();
        
        for(size_t iter = 0; iter < ITERATIONS; iter++) {
            CacheAwareList<int> lst;
            // 插入测试
            for(size_t i = 0; i < TEST_SIZE; i++) {
                lst.push_front(i);
            }
            // 删除测试
            int value;
            while(lst.pop_front(value)) {}
        }
        
        auto end = high_resolution_clock::now();
        return duration_cast<milliseconds>(end - start).count();
    }
};

int main() {
    ListTester tester;
    tester.runTest();
    return 0;
} 