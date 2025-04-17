#include <atomic>
#include <thread>
#include <iostream>
#include <cassert>
#include <vector>
#include <chrono>

// 用于演示的全局原子变量
std::atomic<int> x{0};
std::atomic<int> y{0};
std::atomic<int> data{0};
std::atomic<bool> ready{false};
std::atomic<int> counter{0};

/**
 * @brief 演示 memory_order_relaxed
 * 最宽松的内存序，只保证原子性，不保证操作顺序
 */
void demonstrate_relaxed() {
    std::cout << "\n=== Demonstrating memory_order_relaxed ===\n";
    
    // 重置变量
    x = 0;
    y = 0;
    
    auto thread1 = std::thread([]() {
        x.store(1, std::memory_order_relaxed);  // A
        y.store(2, std::memory_order_relaxed);  // B
    });

    auto thread2 = std::thread([]() {
        int y_val = y.load(std::memory_order_relaxed);  // C
        int x_val = x.load(std::memory_order_relaxed);  // D
        std::cout << "y=" << y_val << ", x=" << x_val << std::endl;
        // 注意：由于是relaxed序，这里可能看到 y=2,x=0 的结果
    });

    thread1.join();
    thread2.join();
}

/**
 * @brief 演示 Release-Acquire 语义
 * producer-consumer 模式的典型应用
 */
void demonstrate_release_acquire() {
    std::cout << "\n=== Demonstrating Release-Acquire semantics ===\n";
    
    // 重置变量
    data = 0;
    ready = false;
    
    auto producer = std::thread([]() {
        // 生产者准备数据
        data.store(42, std::memory_order_relaxed);
        // 使用release语义发布数据
        ready.store(true, std::memory_order_release);
        std::cout << "Producer: data prepared and published\n";
    });

    auto consumer = std::thread([]() {
        // 消费者等待数据就绪
        while (!ready.load(std::memory_order_acquire)) {
            // 自旋等待
            std::this_thread::yield();
        }
        // 读取数据 - 保证能看到producer写入的值
        int value = data.load(std::memory_order_relaxed);
        assert(value == 42);  // 这个断言一定成功
        std::cout << "Consumer: successfully read value " << value << std::endl;
    });

    producer.join();
    consumer.join();
}

/**
 * @brief 演示 Sequential Consistency
 * 最严格的内存序，保证全局一致的操作顺序
 */
void demonstrate_sequential_consistency() {
    std::cout << "\n=== Demonstrating Sequential Consistency ===\n";
    
    // 重置计数器
    counter = 0;
    
    std::vector<std::thread> threads;
    const int thread_count = 5;
    const int iterations = 1000;

    // 创建多个线程增加计数器
    for (int i = 0; i < thread_count; ++i) {
        threads.emplace_back([iterations]() {
            for (int j = 0; j < iterations; ++j) {
                counter.fetch_add(1, std::memory_order_seq_cst);
            }
        });
    }

    // 同时创建一个读取线程
    auto reader = std::thread([]() {
        for (int i = 0; i < 5; ++i) {
            int value = counter.load(std::memory_order_seq_cst);
            std::cout << "Counter value: " << value << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    });

    // 等待所有线程完成
    for (auto& t : threads) {
        t.join();
    }
    reader.join();

    std::cout << "Final counter value: " << counter.load() << std::endl;
}

/**
 * @brief 性能比较测试
 * 比较不同内存序的性能差异
 */
void compare_performance() {
    std::cout << "\n=== Comparing Performance of Different Memory Orders ===\n";
    
    const int iterations = 10000000;
    std::atomic<int> test_counter{0};
    auto start_time = std::chrono::high_resolution_clock::now();

    // 测试 relaxed 序
    for (int i = 0; i < iterations; ++i) {
        test_counter.fetch_add(1, std::memory_order_relaxed);
    }
    
    auto end_relaxed = std::chrono::high_resolution_clock::now();
    auto duration_relaxed = std::chrono::duration_cast<std::chrono::microseconds>
                          (end_relaxed - start_time);

    // 测试 seq_cst 序
    for (int i = 0; i < iterations; ++i) {
        test_counter.fetch_add(1, std::memory_order_seq_cst);
    }
    
    auto end_seqcst = std::chrono::high_resolution_clock::now();
    auto duration_seqcst = std::chrono::duration_cast<std::chrono::microseconds>
                         (end_seqcst - end_relaxed);

    std::cout << "Relaxed ordering took: " << duration_relaxed.count() << " microseconds\n";
    std::cout << "Sequential consistency took: " << duration_seqcst.count() << " microseconds\n";
}

int main() {
    std::cout << "Memory Order Demonstration Program\n";
    std::cout << "==================================\n";

    // 演示不同的内存序
    demonstrate_relaxed();
    demonstrate_release_acquire();
    demonstrate_sequential_consistency();
    
    // 性能比较
    compare_performance();

    return 0;
} 