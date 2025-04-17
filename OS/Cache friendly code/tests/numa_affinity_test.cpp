#include <iostream>
#include <chrono>
#include <vector>
#include <numa.h>
#include <numaif.h>
#include <iomanip>
#include <thread>
#include <atomic>

using namespace std;
using namespace std::chrono;

/**
 * @brief NUMA线程亲和性测试类
 */
class NumaAffinityTester {
private:
    static const size_t BUFFER_SIZE = 1024 * 1024 * 256;  // 256MB
    static const size_t NUM_THREADS = 4;
    static const size_t ITERATIONS = 10000000;
    const int num_nodes;
    atomic<size_t> total_ops{0};
    
public:
    NumaAffinityTester() : num_nodes(numa_num_configured_nodes()) {
        if(numa_available() < 0) {
            throw runtime_error("NUMA不可用");
        }
        
        cout << "NUMA节点数量: " << num_nodes << endl;
        cout << "CPU核心数量: " << numa_num_configured_cpus() << endl;
        
        // 打印NUMA拓扑信息
        printNumaTopology();
    }
    
    void runTest() {
        cout << "开始NUMA线程亲和性测试...\n" << endl;
        
        auto matched_time = testMatchedAffinity();
        auto mismatched_time = testMismatchedAffinity();
        auto unbound_time = testUnboundThreads();
        
        cout << "\n测试结果汇总:" << endl;
        cout << "----------------------------------------" << endl;
        cout << setw(20) << "亲和性类型" << setw(15) << "吞吐量(Mops/s)" << endl;
        cout << "----------------------------------------" << endl;
        cout << setw(20) << "匹配亲和性" << setw(15) << calculateThroughput(matched_time) << endl;
        cout << setw(20) << "不匹配亲和性" << setw(15) << calculateThroughput(mismatched_time) << endl;
        cout << setw(20) << "无绑定" << setw(15) << calculateThroughput(unbound_time) << endl;
        cout << "----------------------------------------" << endl;
        
        if(matched_time > 0 && mismatched_time > 0) {
            cout << "不匹配惩罚: " << fixed << setprecision(2) 
                 << (double)mismatched_time/matched_time << "x" << endl;
        }
    }
    
private:
    void printNumaTopology() {
        cout << "\nNUMA拓扑信息:" << endl;
        for(int i = 0; i < num_nodes; i++) {
            long size = numa_node_size(i, nullptr);
            cout << "节点 " << i << ": " 
                 << (size / (1024*1024)) << " MB 内存" << endl;
            
            struct bitmask* cpumask = numa_allocate_cpumask();
            numa_node_to_cpus(i, cpumask);
            cout << "    CPU核心: ";
            for(int j = 0; j < numa_num_configured_cpus(); j++) {
                if(numa_bitmask_isbitset(cpumask, j)) {
                    cout << j << " ";
                }
            }
            cout << endl;
            numa_free_cpumask(cpumask);
        }
        cout << endl;
    }
    
    double calculateThroughput(double time_ms) {
        // 计算吞吐量 (Mops/s)
        return (total_ops / 1000000.0) / (time_ms / 1000.0);
    }
    
    void bindThreadToNode(int node) {
        struct bitmask* nodemask = numa_allocate_nodemask();
        numa_bitmask_setbit(nodemask, node);
        numa_bind(nodemask);
        numa_free_nodemask(nodemask);
    }
    
    void workerThread(char* buffer, size_t size, int node = -1) {
        if(node >= 0) {
            bindThreadToNode(node);
        }
        
        volatile char sum = 0;
        for(size_t i = 0; i < ITERATIONS; i++) {
            size_t idx = i % size;
            sum += buffer[idx];
            buffer[idx] = sum;
            total_ops++;
        }
    }
    
    double testMatchedAffinity() {
        cout << "测试匹配的内存和线程亲和性..." << endl;
        
        total_ops = 0;
        vector<thread> threads;
        vector<void*> memories(NUM_THREADS);
        
        // 为每个线程在本地节点分配内存
        for(size_t i = 0; i < NUM_THREADS; i++) {
            int node = i % num_nodes;
            memories[i] = numa_alloc_onnode(BUFFER_SIZE, node);
            if(!memories[i]) throw runtime_error("内存分配失败");
            memset(memories[i], 0, BUFFER_SIZE);
        }
        
        auto start = high_resolution_clock::now();
        
        // 创建线程并绑定到对应节点
        for(size_t i = 0; i < NUM_THREADS; i++) {
            int node = i % num_nodes;
            threads.emplace_back(&NumaAffinityTester::workerThread, 
                               this, (char*)memories[i], BUFFER_SIZE, node);
        }
        
        for(auto& t : threads) {
            t.join();
        }
        
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(end - start).count();
        
        // 释放内存
        for(auto mem : memories) {
            numa_free(mem, BUFFER_SIZE);
        }
        
        return duration;
    }
    
    double testMismatchedAffinity() {
        if(num_nodes < 2) {
            cout << "系统只有一个NUMA节点，跳过不匹配测试" << endl;
            return 0;
        }
        
        cout << "测试不匹配的内存和线程亲和性..." << endl;
        
        total_ops = 0;
        vector<thread> threads;
        vector<void*> memories(NUM_THREADS);
        
        // 为每个线程在远程节点分配内存
        for(size_t i = 0; i < NUM_THREADS; i++) {
            int node = i % num_nodes;
            int remote_node = (node + 1) % num_nodes;
            memories[i] = numa_alloc_onnode(BUFFER_SIZE, remote_node);
            if(!memories[i]) throw runtime_error("内存分配失败");
            memset(memories[i], 0, BUFFER_SIZE);
        }
        
        auto start = high_resolution_clock::now();
        
        // 创建线程并绑定到与内存不同的节点
        for(size_t i = 0; i < NUM_THREADS; i++) {
            int node = i % num_nodes;
            threads.emplace_back(&NumaAffinityTester::workerThread, 
                               this, (char*)memories[i], BUFFER_SIZE, node);
        }
        
        for(auto& t : threads) {
            t.join();
        }
        
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(end - start).count();
        
        // 释放内存
        for(auto mem : memories) {
            numa_free(mem, BUFFER_SIZE);
        }
        
        return duration;
    }
    
    double testUnboundThreads() {
        cout << "测试无绑定的线程..." << endl;
        
        total_ops = 0;
        vector<thread> threads;
        vector<void*> memories(NUM_THREADS);
        
        // 使用交错内存分配
        for(size_t i = 0; i < NUM_THREADS; i++) {
            memories[i] = numa_alloc_interleaved(BUFFER_SIZE);
            if(!memories[i]) throw runtime_error("内存分配失败");
            memset(memories[i], 0, BUFFER_SIZE);
        }
        
        auto start = high_resolution_clock::now();
        
        // 创建不绑定的线程
        for(size_t i = 0; i < NUM_THREADS; i++) {
            threads.emplace_back(&NumaAffinityTester::workerThread, 
                               this, (char*)memories[i], BUFFER_SIZE, -1);
        }
        
        for(auto& t : threads) {
            t.join();
        }
        
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(end - start).count();
        
        // 释放内存
        for(auto mem : memories) {
            numa_free(mem, BUFFER_SIZE);
        }
        
        return duration;
    }
};

// 在类外定义静态成员变量
const size_t NumaAffinityTester::BUFFER_SIZE;
const size_t NumaAffinityTester::NUM_THREADS;
const size_t NumaAffinityTester::ITERATIONS;

int main() {
    try {
        NumaAffinityTester tester;
        tester.runTest();
    } catch(const exception& e) {
        cerr << "错误: " << e.what() << endl;
        return 1;
    }
    return 0;
} 