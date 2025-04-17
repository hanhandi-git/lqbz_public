// batch_strategy.h
#pragma once
#include <chrono>

class BatchStrategy {
public:
    BatchStrategy(size_t maxSize, std::chrono::milliseconds maxTime)
        : maxSize_(maxSize)
        , maxTime_(maxTime)
        , lastTriggerTime_(std::chrono::steady_clock::now()) {}
        
    bool shouldTrigger(size_t currentSize) {
        auto now = std::chrono::steady_clock::now();
        auto elapsed = now - lastTriggerTime_;
        
        return currentSize >= maxSize_ || elapsed >= maxTime_;
    }
    
    void reset() {
        lastTriggerTime_ = std::chrono::steady_clock::now();
    }

private:
    size_t maxSize_;
    std::chrono::milliseconds maxTime_;
    std::chrono::steady_clock::time_point lastTriggerTime_;
};

// batch_collector.h
#pragma once
#include <folly/MPMCQueue.h>
#include "batch.h"
#include "batch_strategy.h"

class BatchCollector {
public:
    BatchCollector(size_t batchSize, std::chrono::milliseconds maxTime)
        : currentBatch_(batchSize)
        , strategy_(batchSize, maxTime) {}
        
    void collect(Task task) {
        currentBatch_.addTask(std::move(task));
        
        if(strategy_.shouldTrigger(currentBatch_.size())) {
            processBatch();
        }
    }
    
private:
    void processBatch() {
        if(currentBatch_.size() > 0) {
            // Process batch
            strategy_.reset();
            currentBatch_ = Batch(currentBatch_.size());
        }
    }
    
    Batch currentBatch_;
    BatchStrategy strategy_;
};

// batch_processor.h
#pragma once
#include <folly/executors/ThreadPoolExecutor.h>
#include "batch.h"

class BatchProcessor {
public:
    using ProcessorFunc = std::function<folly::dynamic(const Batch&)>;
    
    BatchProcessor(ProcessorFunc func, folly::ThreadPoolExecutor* executor)
        : processor_(std::move(func))
        , executor_(executor) {}
        
    folly::Future<folly::dynamic> process(Batch batch) {
        return folly::via(executor_, [this, batch = std::move(batch)]() mutable {
            return processor_(batch);
        });
    }

private:
    ProcessorFunc processor_;
    folly::ThreadPoolExecutor* executor_;
};

// stage.h
#pragma once
#include <folly/MPMCQueue.h>
#include "batch_collector.h"
#include "batch_processor.h"

class Stage {
public:
    Stage(std::string name, 
          size_t batchSize,
          std::chrono::milliseconds maxTime,
          BatchProcessor::ProcessorFunc processor,
          folly::ThreadPoolExecutor* executor)
        : name_(std::move(name))
        , collector_(batchSize, maxTime)
        , processor_(std::move(processor), executor)
        , inputQueue_(1000)
        , running_(false) {}
        
    void start() {
        running_ = true;
        worker_ = std::thread([this] {
            while(running_) {
                Task task;
                if(inputQueue_.tryReadUntil(
                    std::chrono::steady_clock::now() + std::chrono::milliseconds(100),
                    task)) {
                    collector_.collect(std::move(task));
                }
            }
        });
    }
    
    void stop() {
        running_ = false;
        if(worker_.joinable()) {
            worker_.join();
        }
    }
    
    void submit(Task task) {
        inputQueue_.blockingWrite(std::move(task));
    }
    
    folly::MPMCQueue<Task>& getInputQueue() { return inputQueue_; }

private:
    std::string name_;
    BatchCollector collector_;
    BatchProcessor processor_;
    folly::MPMCQueue<Task> inputQueue_;
    std::thread worker_;
    std::atomic<bool> running_;
};

// pipeline.h
#pragma once
#include <folly/executors/ThreadPoolExecutor.h>
#include <folly/executors/CPUThreadPoolExecutor.h>
#include "stage.h"

class Pipeline {
public:
    Pipeline(size_t numThreads = std::thread::hardware_concurrency())
        : executor_(std::make_unique<folly::CPUThreadPoolExecutor>(numThreads)) {}
        
    ~Pipeline() {
        stop();
    }
    
    void addStage(std::string name,
                  size_t batchSize,
                  std::chrono::milliseconds maxTime,
                  BatchProcessor::ProcessorFunc processor) {
        stages_.emplace_back(
            std::move(name),
            batchSize,
            maxTime,
            std::move(processor),
            executor_.get());
    }
    
    void start() {
        for(auto& stage : stages_) {
            stage.start();
        }
    }
    
    void stop() {
        for(auto& stage : stages_) {
            stage.stop();
        }
    }
    
    folly::Future<folly::dynamic> submit(Task task) {
        if(!stages_.empty()) {
            auto future = task.getFuture();
            stages_[0].submit(std::move(task));
            return future;
        }
        return folly::makeFuture<folly::dynamic>(
            folly::make_exception_wrapper<std::runtime_error>("No stages"));
    }

private:
    std::vector<Stage> stages_;
    std::unique_ptr<folly::ThreadPoolExecutor> executor_;
};