#include <iostream>
#include <chrono>
#include <vector>
#include <numa.h>
#include <numaif.h>
#include <iomanip>
#include <thread>
#include <random>

using namespace std;
using namespace std::chrono;

/**
 * @brief NUMA内存分配策略测试类
 */
class NumaAllocationTester {
private:
    static const size_t BUFFER_SIZE = 1024 * 1024 * 256;  // 256MB
    static const size_t ITERATIONS = 100000;
    static const size_t STRIDE = 64;  // 缓存行大小
    const int num_nodes;
    vector<size_t> random_indices;
    
public:
    NumaAllocationTester() : num_nodes(numa_num_configured_nodes()) {
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
        
        cout << "NUMA节点数量: " << num_nodes << endl;
        cout << "CPU核心数量: " << numa_num_configured_cpus() << endl;
        
        // 打印NUMA拓扑信息
        printNumaTopology();
    }
    
    void runTest() {
        cout << "开始NUMA内存分配策略测试...\n" << endl;
        
        auto local_time = testLocalAlloc();
        auto interleaved_time = testInterleavedAlloc();
        auto remote_time = testRemoteAlloc();
        
        cout << "\n测试结果汇总:" << endl;
        cout << "----------------------------------------" << endl;
        cout << setw(20) << "分配策略" << setw(15) << "带宽(MB/s)" << endl;
        cout << "----------------------------------------" << endl;
        cout << setw(20) << "本地分配" << setw(15) << calculateBandwidth(local_time) << endl;
        cout << setw(20) << "交错分配" << setw(15) << calculateBandwidth(interleaved_time) << endl;
        cout << setw(20) << "远程分配" << setw(15) << calculateBandwidth(remote_time) << endl;
        cout << "----------------------------------------" << endl;
        
        if(local_time > 0 && remote_time > 0) {
            cout << "远程访问惩罚: " << fixed << setprecision(2) 
                 << (double)remote_time/local_time << "x" << endl;
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
    
    double calculateBandwidth(double time_ms) {
        // 计算带宽 (MB/s)
        return (BUFFER_SIZE * ITERATIONS) / (1024.0 * 1024.0 * time_ms) * 1000.0;
    }
    
    double testLocalAlloc() {
        cout << "测试本地内存分配..." << endl;
        
        // 在当前节点分配内存
        void* mem = numa_alloc_local(BUFFER_SIZE);
        if(!mem) throw runtime_error("内存分配失败");
        
        // 初始化内存
        memset(mem, 0, BUFFER_SIZE);
        
        // 绑定到当前CPU
        int current_cpu = sched_getcpu();
        cpu_set_t cpuset;
        CPU_ZERO(&cpuset);
        CPU_SET(current_cpu, &cpuset);
        pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset);
        
        // 测试访问性能
        auto start = high_resolution_clock::now();
        volatile char sum = 0;
        char* buffer = (char*)mem;
        
        for(size_t i = 0; i < ITERATIONS; i++) {
            sum += buffer[random_indices[i]];
            buffer[random_indices[i]] = sum;
        }
        
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(end - start).count();
        
        numa_free(mem, BUFFER_SIZE);
        return duration;
    }
    
    double testInterleavedAlloc() {
        cout << "测试交错内存分配..." << endl;
        
        void* mem = numa_alloc_interleaved(BUFFER_SIZE);
        if(!mem) throw runtime_error("内存分配失败");
        
        memset(mem, 0, BUFFER_SIZE);
        
        auto start = high_resolution_clock::now();
        volatile char sum = 0;
        char* buffer = (char*)mem;
        
        for(size_t i = 0; i < ITERATIONS; i++) {
            sum += buffer[random_indices[i]];
            buffer[random_indices[i]] = sum;
        }
        
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(end - start).count();
        
        numa_free(mem, BUFFER_SIZE);
        return duration;
    }
    
    double testRemoteAlloc() {
        if(num_nodes < 2) {
            cout << "系统只有一个NUMA节点，跳过远程分配测试" << endl;
            return 0;
        }
        
        cout << "测试远程内存分配..." << endl;
        
        // 获取当前CPU所在的NUMA节点
        int current_node = numa_node_of_cpu(sched_getcpu());
        // 选择一个远程节点
        int remote_node = (current_node + 1) % num_nodes;
        
        // 在远程节点分配内存
        void* mem = numa_alloc_onnode(BUFFER_SIZE, remote_node);
        if(!mem) throw runtime_error("内存分配失败");
        
        memset(mem, 0, BUFFER_SIZE);
        
        auto start = high_resolution_clock::now();
        volatile char sum = 0;
        char* buffer = (char*)mem;
        
        for(size_t i = 0; i < ITERATIONS; i++) {
            sum += buffer[random_indices[i]];
            buffer[random_indices[i]] = sum;
        }
        
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(end - start).count();
        
        numa_free(mem, BUFFER_SIZE);
        return duration;
    }
};

int main() {
    try {
        NumaAllocationTester tester;
        tester.runTest();
    } catch(const exception& e) {
        cerr << "错误: " << e.what() << endl;
        return 1;
    }
    return 0;
} 