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