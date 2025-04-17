#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include <atomic>
#include <iomanip>

using namespace std;
using namespace std::chrono;

/**
 * @brief 测试伪共享对性能的影响
 */
class FalseSharingTester {
private:
    // 有伪共享的数据结构
    struct BadCounter {
        std::atomic<long> value{0};
    };
    
    // 避免伪共享的数据结构
    struct alignas(64) GoodCounter {
        std::atomic<long> value{0};
        char padding[64 - sizeof(std::atomic<long>)];
    };
    
    static const size_t NUM_THREADS = 4;
    static const size_t ITERATIONS = 10000000;
    vector<BadCounter> bad_counters;
    vector<GoodCounter> good_counters;
    
public:
    FalseSharingTester() 
        : bad_counters(NUM_THREADS), good_counters(NUM_THREADS) {}
    
    void runTest() {
        cout << "开始伪共享测试...\n" << endl;
        
        auto bad_time = testWithFalseSharing();
        auto good_time = testWithoutFalseSharing();
        
        cout << "\n测试结果汇总:" << endl;
        cout << "----------------------------------------" << endl;
        cout << setw(20) << "实现方式" << setw(15) << "耗时(ms)" << endl;
        cout << "----------------------------------------" << endl;
        cout << setw(20) << "有伪共享" << setw(15) << bad_time << endl;
        cout << setw(20) << "无伪共享" << setw(15) << good_time << endl;
        cout << "----------------------------------------" << endl;
        
        if(bad_time > 0) {
            double speedup = static_cast<double>(bad_time) / good_time;
            cout << "性能提升: " << fixed << setprecision(2) << speedup << "x" << endl;
        }
    }
    
private:
    double testWithFalseSharing() {
        cout << "测试有伪共享的版本..." << endl;
        
        auto start = high_resolution_clock::now();
        vector<thread> threads;
        
        // 创建多个线程，每个线程更新自己的计数器
        for(size_t i = 0; i < NUM_THREADS; i++) {
            threads.emplace_back([this, i]() {
                for(size_t j = 0; j < ITERATIONS; j++) {
                    bad_counters[i].value.fetch_add(1, memory_order_relaxed);
                }
            });
        }
        
        // 等待所有线程完成
        for(auto& t : threads) {
            t.join();
        }
        
        auto end = high_resolution_clock::now();
        return duration_cast<milliseconds>(end - start).count();
    }
    
    double testWithoutFalseSharing() {
        cout << "测试无伪共享的版本..." << endl;
        
        auto start = high_resolution_clock::now();
        vector<thread> threads;
        
        // 创建多个线程，每个线程更新自己的计数器
        for(size_t i = 0; i < NUM_THREADS; i++) {
            threads.emplace_back([this, i]() {
                for(size_t j = 0; j < ITERATIONS; j++) {
                    good_counters[i].value.fetch_add(1, memory_order_relaxed);
                }
            });
        }
        
        // 等待所有线程完成
        for(auto& t : threads) {
            t.join();
        }
        
        auto end = high_resolution_clock::now();
        return duration_cast<milliseconds>(end - start).count();
    }
};

int main() {
    FalseSharingTester tester;
    tester.runTest();
    return 0;
} 