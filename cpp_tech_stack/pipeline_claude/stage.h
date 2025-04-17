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