#include <iostream>
#include <chrono>
#include <map>
#include <random>
#include <iomanip>
#include "cache_aware_btree.hpp"

using namespace std;
using namespace std::chrono;

/**
 * @brief 测试B+树的缓存优化效果
 */
class TreeTester {
private:
    static const size_t TEST_SIZE = 1000000;
    static const size_t ITERATIONS = 100;
    vector<int> test_data;
    
public:
    TreeTester() {
        // 生成测试数据
        test_data.resize(TEST_SIZE);
        for(size_t i = 0; i < TEST_SIZE; i++) {
            test_data[i] = i;
        }
        
        // 随机打乱
        random_device rd;
        mt19937 gen(rd());
        shuffle(test_data.begin(), test_data.end(), gen);
    }
    
    void runTest() {
        cout << "开始树结构性能测试...\n" << endl;
        
        auto std_time = testStdMap();
        auto cache_time = testCacheAwareTree();
        
        cout << "\n测试结果汇总:" << endl;
        cout << "----------------------------------------" << endl;
        cout << setw(20) << "实现方式" << setw(15) << "耗时(ms)" << endl;
        cout << "----------------------------------------" << endl;
        cout << setw(20) << "std::map" << setw(15) << std_time << endl;
        cout << setw(20) << "cache_aware_tree" << setw(15) << cache_time << endl;
        cout << "----------------------------------------" << endl;
        
        if(std_time > 0) {
            double speedup = static_cast<double>(std_time) / cache_time;
            cout << "性能提升: " << fixed << setprecision(2) 
                 << speedup << "x" << endl;
        }
    }
    
private:
    double testStdMap() {
        cout << "测试std::map..." << endl;
        
        auto start = high_resolution_clock::now();
        
        for(size_t iter = 0; iter < ITERATIONS; iter++) {
            map<int, int> tree;
            // 插入测试
            for(size_t i = 0; i < TEST_SIZE; i++) {
                tree[test_data[i]] = test_data[i];
            }
            // 查找测试
            volatile int sum = 0;
            for(size_t i = 0; i < TEST_SIZE; i++) {
                auto it = tree.find(test_data[i]);
                if(it != tree.end()) {
                    sum += it->second;
                }
            }
        }
        
        auto end = high_resolution_clock::now();
        return duration_cast<milliseconds>(end - start).count();
    }
    
    double testCacheAwareTree() {
        cout << "测试cache_aware_tree..." << endl;
        
        auto start = high_resolution_clock::now();
        
        for(size_t iter = 0; iter < ITERATIONS; iter++) {
            CacheAwareBPlusTree<int, int> tree;
            // 插入测试
            for(size_t i = 0; i < TEST_SIZE; i++) {
                tree.insert(test_data[i], test_data[i]);
            }
            // 查找测试
            volatile int sum = 0;
            int value;
            for(size_t i = 0; i < TEST_SIZE; i++) {
                if(tree.find(test_data[i], value)) {
                    sum += value;
                }
            }
        }
        
        auto end = high_resolution_clock::now();
        return duration_cast<milliseconds>(end - start).count();
    }
};

int main() {
    TreeTester tester;
    tester.runTest();
    return 0;
} 