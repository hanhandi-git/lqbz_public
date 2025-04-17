#include <iostream>
#include <vector>
#include <chrono>
#include <folly/futures/Future.h>
#include <folly/dynamic.h>
#include <folly/futures/collect.h>
#include "pipeline.h"
#include "task.h"
#include "batch.h"

int main() {
    Pipeline pipeline;
    
    // 添加处理阶段
    pipeline.addStage(
        "Stage1",
        100,  // batch size
        std::chrono::seconds(5),  // max time
        [](const Batch& batch) {
            folly::dynamic result = folly::dynamic::object;
            // 批量处理逻辑
            return result;
        }
    );
    
    pipeline.addStage(
        "Stage2",
        50,   // batch size
        std::chrono::seconds(3),  // max time
        [](const Batch& batch) {
            folly::dynamic result = folly::dynamic::object;
            // 批量处理逻辑
            return result;
        }
    );
    
    // 启动流水线
    pipeline.start();
    
    // 提交任务
    std::vector<folly::Future<folly::dynamic>> futures;
    for(int i = 0; i < 1000; i++) {
        Task task(i);
        task.setData(folly::dynamic::object("value", i));
        futures.push_back(pipeline.submit(std::move(task)));
    }
    
    // 等待所有任务完成
    auto results = folly::collect(futures).get();
    
    // 停止流水线
    pipeline.stop();
    
    return 0;
}