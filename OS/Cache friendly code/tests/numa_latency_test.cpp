#include <iostream>
#include <chrono>
#include <vector>
#include <numa.h>
#include <numaif.h>
#include <iomanip>
#include <thread>
#include <random>
#include <immintrin.h>

using namespace std;
using namespace std::chrono;

/**
 * @brief NUMA访问延迟测试类
 */
class NumaLatencyTester {
private:
    static const size_t BUFFER_SIZE = 1024 * 1024 * 64;  // 增加到64MB
    static const size_t STRIDE = 64;  // 每次跳过一个缓存行
    static const size_t ITERATIONS = 10000000;
    const int num_nodes;
    
    // 添加随机访问
    vector<size_t> random_indices;
    
public:
    NumaLatencyTester() : num_nodes(numa_num_configured_nodes()) {
        if(num_nodes < 2) {
            cout << "警告: 系统只有一个NUMA节点，无法测试远程访问延迟" << endl;
        }
        
        // 初始化NUMA库
        if(numa_available() < 0) {
            throw runtime_error("NUMA不可用");
        }
        
        // 生成随机访问序列
        random_indices.resize(ITERATIONS);
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<size_t> dis(0, (BUFFER_SIZE/STRIDE) - 1);
        for(size_t i = 0; i < ITERATIONS; i++) {
            random_indices[i] = dis(gen) * STRIDE;
        }
        
        // 打印NUMA信息
        cout << "NUMA节点数量: " << num_nodes << endl;
        cout << "CPU核心数量: " << numa_num_configured_cpus() << endl;
    }
    
    void runTest() {
        cout << "开始NUMA访问延迟测试...\n" << endl;
        
        // 测试本地访问
        auto local_time = testLocalAccess();
        
        // 如果有多个节点，测试远程访问
        double remote_time = 0;
        if(num_nodes >= 2) {
            remote_time = testRemoteAccess();
        }
        
        // 输出结果
        cout << "\n测试结果汇总:" << endl;
        cout << "----------------------------------------" << endl;
        cout << setw(20) << "访问类型" << setw(15) << "平均延迟(ns)" << endl;
        cout << "----------------------------------------" << endl;
        cout << setw(20) << "本地访问" << setw(15) << local_time << endl;
        if(num_nodes >= 2) {
            cout << setw(20) << "远程访问" << setw(15) << remote_time << endl;
            cout << "----------------------------------------" << endl;
            cout << "远程访问惩罚: " << fixed << setprecision(2) 
                 << remote_time/local_time << "x" << endl;
        }
        cout << "----------------------------------------" << endl;
    }
    
private:
    double testLocalAccess() {
        cout << "测试本地内存访问..." << endl;
        
        // 在当前NUMA节点分配内存
        void* local_mem = numa_alloc_onnode(BUFFER_SIZE, numa_node_of_cpu(sched_getcpu()));
        if(!local_mem) {
            throw runtime_error("本地内存分配失败");
        }
        
        // 初始化内存并刷新缓存
        char* buffer = (char*)local_mem;
        for(size_t i = 0; i < BUFFER_SIZE; i++) {
            buffer[i] = i & 0xFF;
        }
        
        // 清除缓存
        for(size_t i = 0; i < BUFFER_SIZE; i += 64) {
            _mm_clflush(&buffer[i]);
        }
        _mm_mfence();
        
        // 测量访问延迟
        auto start = high_resolution_clock::now();
        volatile char sum = 0;
        
        for(size_t i = 0; i < ITERATIONS; i++) {
            sum += buffer[random_indices[i]];
        }
        
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<nanoseconds>(end - start).count();
        
        numa_free(local_mem, BUFFER_SIZE);
        return static_cast<double>(duration) / ITERATIONS;
    }
    
    double testRemoteAccess() {
        cout << "测试远程内存访问..." << endl;
        
        // 获取当前CPU所在的NUMA节点
        int current_node = numa_node_of_cpu(sched_getcpu());
        
        // 选择一个不同的NUMA节点
        int remote_node = (current_node + 1) % num_nodes;
        
        // 在远程节点分配内存
        void* remote_mem = numa_alloc_onnode(BUFFER_SIZE, remote_node);
        if(!remote_mem) {
            throw runtime_error("远程内存分配失败");
        }
        
        // 初始化内存并刷新缓存
        char* buffer = (char*)remote_mem;
        for(size_t i = 0; i < BUFFER_SIZE; i++) {
            buffer[i] = i & 0xFF;
        }
        
        // 清除缓存
        for(size_t i = 0; i < BUFFER_SIZE; i += 64) {
            _mm_clflush(&buffer[i]);
        }
        _mm_mfence();
        
        // 确保线程运行在本地节点上
        cpu_set_t cpuset;
        CPU_ZERO(&cpuset);
        for(int i = 0; i < numa_num_configured_cpus(); i++) {
            if(numa_node_of_cpu(i) == current_node) {
                CPU_SET(i, &cpuset);
            }
        }
        pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset);
        
        // 测量访问延迟
        auto start = high_resolution_clock::now();
        volatile char sum = 0;
        
        for(size_t i = 0; i < ITERATIONS; i++) {
            sum += buffer[random_indices[i]];
        }
        
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<nanoseconds>(end - start).count();
        
        numa_free(remote_mem, BUFFER_SIZE);
        return static_cast<double>(duration) / ITERATIONS;
    }
};

int main() {
    try {
        NumaLatencyTester tester;
        tester.runTest();
    } catch(const exception& e) {
        cerr << "错误: " << e.what() << endl;
        return 1;
    }
    return 0;
} 