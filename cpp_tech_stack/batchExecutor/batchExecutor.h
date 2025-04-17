#include <vector>
#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <future>
#include <chrono>
#include <atomic>
#include <memory>
#include <iostream>

/**
 * 批处理执行器
 * TaskType: 任务类型
 * ResultType: 结果类型
 */
template<typename TaskType, typename ResultType>
class BatchExecutor {
public:
    using TaskCallback = std::function<void(const ResultType&)>;
    using BatchProcessor = std::function<std::vector<ResultType>(const std::vector<TaskType>&)>;
    
    struct TaskItem {
        TaskType task;
        TaskCallback callback;
        
        TaskItem(TaskType t, TaskCallback cb) : task(std::move(t)), callback(std::move(cb)) {}
    };
    
    /**
     * 构造函数
     * @param batchProcessor 批处理函数，处理一批任务并返回对应结果
     * @param maxBatchSize 最大批处理大小
     * @param maxWaitTimeMs 最大等待时间(毫秒)
     * @param processingThreads 处理线程数
     * @param callbackThreads 回调线程数
     * @param enableNUMABinding 是否启用NUMA绑核
     */
    BatchExecutor(
        BatchProcessor batchProcessor,
        size_t maxBatchSize = 64,
        size_t maxWaitTimeMs = 100,
        size_t processingThreads = 4,
        size_t callbackThreads = 2,
        bool enableNUMABinding = false
    ) : batchProcessor_(std::move(batchProcessor)),
        maxBatchSize_(maxBatchSize),
        maxWaitTimeMs_(maxWaitTimeMs),
        running_(true),
        enableNUMABinding_(enableNUMABinding) {
        
        // 预分配内存以减少运行时分配
        taskPrealloc_.reserve(maxBatchSize);
        resultsPrealloc_.reserve(maxBatchSize);
        
        // 启动处理线程
        for (size_t i = 0; i < processingThreads; ++i) {
            processingThreads_.emplace_back(&BatchExecutor::processingThread, this);
        }
        
        // 启动回调线程
        for (size_t i = 0; i < callbackThreads; ++i) {
            callbackThreads_.emplace_back(&BatchExecutor::callbackThread, this);
        }
        
        // 如果启用NUMA绑定，将线程绑定到特定核心
        if (enableNUMABinding_) {
            bindThreadsToNUMA();
        }
    }
    
    /**
     * 析构函数，停止所有线程
     */
    ~BatchExecutor() {
        shutdown();
    }
    
    /**
     * 提交任务
     * @param task 任务
     * @param callback 回调函数
     */
    void submit(TaskType task, TaskCallback callback) {
        {
            std::lock_guard<std::mutex> lock(taskMutex_);
            taskQueue_.emplace(std::move(task), std::move(callback));
        }
        taskCV_.notify_one();
    }
    
    /**
     * 提交任务并返回future
     * @param task 任务
     * @return 包含结果的future
     */
    std::future<ResultType> submitWithFuture(TaskType task) {
        auto promise = std::make_shared<std::promise<ResultType>>();
        std::future<ResultType> future = promise->get_future();
        
        submit(std::move(task), [promise](const ResultType& result) {
            promise->set_value(result);
        });
        
        return future;
    }
    
    /**
     * 关闭执行器
     */
    void shutdown() {
        {
            std::lock_guard<std::mutex> lock(taskMutex_);
            if (!running_) return;
            running_ = false;
        }
        
        taskCV_.notify_all();
        callbackCV_.notify_all();
        
        for (auto& thread : processingThreads_) {
            if (thread.joinable()) {
                thread.join();
            }
        }
        
        for (auto& thread : callbackThreads_) {
            if (thread.joinable()) {
                thread.join();
            }
        }
    }
    
    /**
     * 获取待处理任务数量
     */
    size_t pendingTasks() const {
        std::lock_guard<std::mutex> lock(taskMutex_);
        return taskQueue_.size();
    }
    
    /**
     * 获取待执行回调数量
     */
    size_t pendingCallbacks() const {
        std::lock_guard<std::mutex> lock(callbackMutex_);
        return callbackQueue_.size();
    }
    
private:
    struct ResultItem {
        ResultType result;
        TaskCallback callback;
        
        ResultItem(ResultType r, TaskCallback cb) : result(std::move(r)), callback(std::move(cb)) {}
    };
    
    // 处理线程函数
    void processingThread() {
        // 线程本地存储的批处理缓冲区，避免频繁分配
        std::vector<TaskItem> batch;
        std::vector<TaskType> batchTasks;
        batch.reserve(maxBatchSize_);
        batchTasks.reserve(maxBatchSize_);
        
        while (running_) {
            batch.clear();
            batchTasks.clear();
            
            // 尝试从其他队列窃取工作
            bool hasWork = getTaskBatch(batch);
            
            // 如果没有工作且执行器仍在运行，则尝试窃取工作
            if (!hasWork && running_ && enableWorkStealing_) {
                // 尝试从其他队列窃取工作(实现工作窃取逻辑)
                hasWork = stealWork(batch);
            }
            
            // 如果没有任务，继续
            if (batch.empty()) {
                continue;
            }
            
            // 准备批处理输入
            for (const auto& item : batch) {
                batchTasks.push_back(item.task);
            }
            
            try {
                // 执行批处理
                std::vector<ResultType> results = batchProcessor_(batchTasks);
                
                // 检查结果大小是否匹配
                if (results.size() != batch.size()) {
                    std::cerr << "警告：批处理结果数量(" << results.size() 
                              << ")与任务数量(" << batch.size() << ")不匹配!" << std::endl;
                    continue;
                }
                
                // 将结果与回调函数放入回调队列
                enqueueBatchResults(std::move(results), batch);
            }
            catch (const std::exception& e) {
                std::cerr << "批处理执行异常: " << e.what() << std::endl;
            }
        }
    }
    
    // 从队列获取一批任务
    bool getTaskBatch(std::vector<TaskItem>& batch) {
        std::unique_lock<std::mutex> lock(taskMutex_);
        
        // 等待直到有任务、到达最大等待时间或关闭
        auto waitUntil = std::chrono::steady_clock::now() + std::chrono::milliseconds(maxWaitTimeMs_);
        
        // 条件：队列不为空或executor已关闭
        taskCV_.wait_until(lock, waitUntil, [this]() {
            return !taskQueue_.empty() || !running_;
        });
        
        // 如果已关闭且队列为空，则退出
        if (!running_ && taskQueue_.empty()) {
            return false;
        }
        
        // 收集当前可用任务，不超过maxBatchSize_
        while (!taskQueue_.empty() && batch.size() < maxBatchSize_) {
            batch.push_back(std::move(taskQueue_.front()));
            taskQueue_.pop();
        }
        
        return !batch.empty();
    }
    
    // 将一批结果加入回调队列
    void enqueueBatchResults(std::vector<ResultType>&& results, const std::vector<TaskItem>& batch) {
        std::lock_guard<std::mutex> lock(callbackMutex_);
        for (size_t i = 0; i < batch.size(); ++i) {
            callbackQueue_.emplace(std::move(results[i]), std::move(batch[i].callback));
        }
        callbackCV_.notify_all();
    }
    
    // 工作窃取逻辑 - 这里只是一个占位，实际实现需要有多个工作队列
    bool stealWork(std::vector<TaskItem>& batch) {
        // 实际实现中，这里应该尝试从其他线程的工作队列中窃取任务
        // 为简化示例，这里返回false表示没有窃取到工作
        return false;
    }
    
    // NUMA绑定逻辑
    void bindThreadsToNUMA() {
#ifdef _GNU_SOURCE
        // 这里实现实际的NUMA绑定逻辑
        // 例如，使用sched_setaffinity()函数设置线程CPU亲和性
        
        const int numCores = std::thread::hardware_concurrency();
        int coreId = 0;
        
        // 绑定处理线程
        for (size_t i = 0; i < processingThreads_.size(); ++i) {
            cpu_set_t cpuset;
            CPU_ZERO(&cpuset);
            CPU_SET(coreId % numCores, &cpuset);
            
            pthread_t threadId = processingThreads_[i].native_handle();
            pthread_setaffinity_np(threadId, sizeof(cpu_set_t), &cpuset);
            
            coreId++;
        }
        
        // 绑定回调线程
        for (size_t i = 0; i < callbackThreads_.size(); ++i) {
            cpu_set_t cpuset;
            CPU_ZERO(&cpuset);
            CPU_SET(coreId % numCores, &cpuset);
            
            pthread_t threadId = callbackThreads_[i].native_handle();
            pthread_setaffinity_np(threadId, sizeof(cpu_set_t), &cpuset);
            
            coreId++;
        }
#endif
    }
    
    // 回调线程函数
    void callbackThread() {
        while (running_ || !callbackQueue_.empty()) {
            ResultItem item = {ResultType(), nullptr};
            
            {
                std::unique_lock<std::mutex> lock(callbackMutex_);
                
                callbackCV_.wait(lock, [this]() {
                    return !callbackQueue_.empty() || !running_;
                });
                
                if (callbackQueue_.empty() && !running_) {
                    return;
                }
                
                if (!callbackQueue_.empty()) {
                    item = std::move(callbackQueue_.front());
                    callbackQueue_.pop();
                }
            }
            
            // 执行回调
            if (item.callback) {
                try {
                    item.callback(item.result);
                }
                catch (const std::exception& e) {
                    std::cerr << "回调执行异常: " << e.what() << std::endl;
                }
            }
        }
    }
    
private:
    // 批处理函数
    BatchProcessor batchProcessor_;
    
    // 配置参数
    size_t maxBatchSize_;
    size_t maxWaitTimeMs_;
    std::atomic<bool> running_;
    bool enableNUMABinding_ = false;
    bool enableWorkStealing_ = false;  // 是否启用工作窃取
    
    // 任务队列
    std::queue<TaskItem> taskQueue_;
    mutable std::mutex taskMutex_;
    std::condition_variable taskCV_;
    
    // 回调队列
    std::queue<ResultItem> callbackQueue_;
    mutable std::mutex callbackMutex_;
    std::condition_variable callbackCV_;
    
    // 线程池
    std::vector<std::thread> processingThreads_;
    std::vector<std::thread> callbackThreads_;
    
    // 预分配的存储空间，减少运行时内存分配
    std::vector<TaskType> taskPrealloc_;
    std::vector<ResultType> resultsPrealloc_;
};

// 编译时计算优化示例 - 使用模板元编程计算批处理大小
template<size_t MinBatchSize, size_t MaxBatchSize, typename Predicate>
constexpr size_t ComputeOptimalBatchSize(Predicate p) {
    for (size_t i = MinBatchSize; i <= MaxBatchSize; i *= 2) {
        if (p(i)) return i;
    }
    return MaxBatchSize;
}

// 使用编译时计算的示例
// template<typename TaskType, typename ResultType, 
//          size_t OptimalBatchSize = ComputeOptimalBatchSize<8, 256>(
//              [](size_t s) { return s >= 64; })>
// class OptimizedBatchExecutor : public BatchExecutor<TaskType, ResultType> {
// public:
//     OptimizedBatchExecutor(
//         typename BatchExecutor<TaskType, ResultType>::BatchProcessor batchProcessor,
//         size_t maxWaitTimeMs = 100,
//         size_t processingThreads = 4,
//         size_t callbackThreads = 2,
//         bool enableNUMABinding = false
//     ) : BatchExecutor<TaskType, ResultType>(
//             std::move(batchProcessor),
//             OptimalBatchSize,  // 使用编译时计算的批处理大小
//             maxWaitTimeMs,
//             processingThreads,
//             callbackThreads,
//             enableNUMABinding) {}
// };