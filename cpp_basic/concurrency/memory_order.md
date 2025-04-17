# C++内存序（Memory Order）详解

## 1. 基本概念

内存序（Memory Order）定义了多线程程序中共享变量的访问规则，主要用于控制原子操作的执行顺序。C++提供了6种内存序：

1. `memory_order_relaxed`: 最宽松的内存序，只保证原子性
2. `memory_order_consume`: 保证当前线程中依赖于该原子变量的操作不会重排
3. `memory_order_acquire`: 读取操作，保证后续读写不会重排到这个操作之前
4. `memory_order_release`: 写入操作，保证之前的读写不会重排到这个操作之后
5. `memory_order_acq_rel`: 组合了acquire和release的语义
6. `memory_order_seq_cst`: 最严格的内存序，保证全序关系

## 2. 示例代码

### 2.1 Relaxed序示例

```cpp
#include <atomic>
#include <thread>
#include <iostream>

std::atomic<int> x{0};
std::atomic<int> y{0};

void thread1() {
    x.store(1, std::memory_order_relaxed);  // A
    y.store(2, std::memory_order_relaxed);  // B
}

void thread2() {
    int y_val = y.load(std::memory_order_relaxed);  // C
    int x_val = x.load(std::memory_order_relaxed);  // D
    std::cout << "y=" << y_val << ", x=" << x_val << std::endl;
}
```

### 2.2 Release-Acquire序示例

```cpp
#include <atomic>
#include <thread>
#include <iostream>

std::atomic<int> data{0};
std::atomic<bool> ready{false};

void producer() {
    data.store(42, std::memory_order_relaxed);  // 准备数据
    ready.store(true, std::memory_order_release);  // 发布
}

void consumer() {
    while (!ready.load(std::memory_order_acquire)) {}  // 获取
    assert(data.load(std::memory_order_relaxed) == 42);  // 一定成功
}
```

### 2.3 Sequential Consistency示例

```cpp
#include <atomic>
#include <thread>
#include <iostream>

std::atomic<int> counter{0};

void increment() {
    counter.fetch_add(1, std::memory_order_seq_cst);
}

void read() {
    int value = counter.load(std::memory_order_seq_cst);
    std::cout << "Counter value: " << value << std::endl;
}
```

## 3. 不同内存序的时序图

```mermaid
sequenceDiagram
    participant T1 as Thread 1
    participant M as Memory
    participant T2 as Thread 2
    
    Note over T1,T2: Release-Acquire 模式
    T1->>M: Store Data (Relaxed)
    T1->>M: Store Ready=true (Release)
    M-->>T2: Load Ready (Acquire)
    Note right of T2: 自旋等待直到Ready为true
    M-->>T2: Load Data (Relaxed)
    Note right of T2: 保证能看到正确的Data值
```

## 4. 常见使用场景

1. Relaxed序：
   - 计数器累加
   - 不需要同步的状态标志

2. Release-Acquire序：
   - 生产者-消费者模式
   - 线程间通信
   - 数据发布

3. Sequential Consistency：
   - 需要严格顺序保证的场景
   - 调试和验证
   - 简单但性能要求不高的场景

## 5. 性能考虑

内存序从松到严的性能开销：
1. Relaxed (最快)
2. Release/Acquire
3. Sequential Consistency (最慢)

```mermaid
graph LR
    A[Relaxed] -->|性能损失| B[Release/Acquire]
    B -->|性能损失| C[Sequential Consistency]
    style A fill:#f9f,stroke:#333,stroke-width:2px
    style B fill:#bbf,stroke:#333,stroke-width:2px
    style C fill:#bfb,stroke:#333,stroke-width:2px
``` 

## 6. 详细场景时序图

### 6.1 Relaxed序 - 多线程计数器场景

```mermaid
sequenceDiagram
    participant T1 as Thread 1
    participant T2 as Thread 2
    participant T3 as Thread 3
    participant C as Counter (atomic)
    
    Note over T1,T3: 多线程并发计数场景
    T1->>C: fetch_add(1, relaxed)
    T2->>C: fetch_add(1, relaxed)
    T3->>C: load(relaxed)
    Note right of T3: 可能看到任意中间值
    T2->>C: fetch_add(1, relaxed)
    T1->>C: fetch_add(1, relaxed)
    Note over T1,T3: 只保证原子性，不保证操作顺序
```

### 6.2 Release-Acquire序 - 生产者消费者场景

```mermaid
sequenceDiagram
    participant P as Producer
    participant B as Buffer
    participant F as Flag
    participant C1 as Consumer 1
    participant C2 as Consumer 2
    
    Note over P,C2: 典型生产者-消费者模式
    P->>B: 写入数据1 (Relaxed)
    P->>B: 写入数据2 (Relaxed)
    P->>B: 写入数据3 (Relaxed)
    P->>F: store(true, Release)
    Note right of P: 保证之前的写入对消费者可见
    
    C1-->>F: load(Acquire)
    Note right of C1: 自旋等待直到Flag为true
    F-->>C1: 返回true
    B-->>C1: 读取数据 (Relaxed)
    Note right of C1: 保证能看到所有数据
    
    C2-->>F: load(Acquire)
    F-->>C2: 返回true
    B-->>C2: 读取数据 (Relaxed)
    Note right of C2: 同样能看到完整数据
```

### 6.3 Sequential Consistency - 严格顺序场景

```mermaid
sequenceDiagram
    participant T1 as Thread 1
    participant X as Variable X
    participant Y as Variable Y
    participant T2 as Thread 2
    participant T3 as Observer
    
    Note over T1,T3: 需要严格顺序保证的场景
    T1->>X: store(1, seq_cst)
    T1->>Y: store(1, seq_cst)
    
    T2->>Y: load(seq_cst)
    T2->>X: load(seq_cst)
    
    T3->>X: load(seq_cst)
    T3->>Y: load(seq_cst)
    
    Note over T1,T3: 所有线程观察到的操作顺序都是一致的
    Note over T1,T3: 如果T2看到Y=1，则一定能看到X=1
    Note over T1,T3: T3观察到的顺序与T1、T2一致
```

### 说明：

1. **Relaxed序场景**：
   - 适用于简单的计数器场景
   - 多个线程可以并发递增计数器
   - 不保证读取操作能看到最新值
   - 但保证每次递增操作是原子的

2. **Release-Acquire序场景**：
   - 生产者写入多个数据后，通过flag发布
   - 消费者首先获取flag状态
   - 一旦flag为true，保证能看到之前写入的所有数据
   - 多个消费者都能正确看到完整数据

3. **Sequential Consistency序场景**：
   - 所有操作都有严格的全局顺序
   - 所有线程观察到的顺序是一致的
   - 适合需要严格同步的场景
   - 性能开销最大，但最容易推理
