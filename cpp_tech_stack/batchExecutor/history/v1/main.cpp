#include <iostream>
#include <string>
#include <random>
#include <thread>
#include <future>
#include <iomanip>
#include "batchExecutor.h"
// 模拟RPC请求
struct RPCRequest {
    int id;
    std::string data;
    
    RPCRequest() : id(0) {}
    RPCRequest(int i, std::string d) : id(i), data(std::move(d)) {}
};

// 模拟RPC响应
struct RPCResponse {
    int id;
    bool success;
    std::string result;
    
    RPCResponse() : id(0), success(false) {}
    RPCResponse(int i, bool s, std::string r) : id(i), success(s), result(std::move(r)) {}
};

// 模拟RPC服务器处理批量请求
std::vector<RPCResponse> mockRpcService(const std::vector<RPCRequest>& requests) {
    // std::cout << "[RPC服务] 收到批量请求，大小: " << requests.size() << std::endl;
    
    // 模拟网络延迟
    std::this_thread::sleep_for(std::chrono::milliseconds(50 + requests.size() * 2));
    
    std::vector<RPCResponse> responses;
    responses.reserve(requests.size());
    
    for (const auto& req : requests) {
        // 模拟处理
        std::string result = "处理结果-" + req.data + "-" + std::to_string(req.id);
        responses.emplace_back(req.id, true, std::move(result));
    }
    
    return responses;
}

int main() {
    // 创建批处理执行器
    BatchExecutor<RPCRequest, RPCResponse> executor(
        mockRpcService,  // 批处理函数
        32,              // 最大批量大小
        200,             // 最大等待时间(ms)
        3,               // 处理线程数
        2                // 回调线程数
    );
    
    // 追踪完成的任务数
    std::atomic<int> completedTasks{0};
    
    // 测试异步提交
    const int totalTasks = 100;
    
    std::cout << "开始提交 " << totalTasks << " 个任务..." << std::endl;
    auto startTime = std::chrono::steady_clock::now();
    
    // 模拟多个客户端提交任务
    std::vector<std::thread> clientThreads;
    for (int i = 0; i < 4; ++i) {
        clientThreads.emplace_back([&executor, &completedTasks, i, totalTasks]() {
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> delayDist(1, 5);
            
            for (int j = 0; j < totalTasks / 4; ++j) {
                int taskId = i * (totalTasks / 4) + j;
                RPCRequest request(taskId, "数据-" + std::to_string(taskId));
                
                // 提交任务
                executor.submit(std::move(request), [taskId, &completedTasks](const RPCResponse& response) {
                    // std::cout << "[回调] 任务ID: " << std::setw(3) << taskId 
                    //           << ", 线程ID: " << std::this_thread::get_id()
                    //           << ", 结果: " << response.result << std::endl;
                    completedTasks++;
                });
                
                // 模拟客户端提交间隔
                std::this_thread::sleep_for(std::chrono::milliseconds(delayDist(gen)));
            }
        });
    }
    
    // 测试future方式
    std::vector<std::future<RPCResponse>> futures;
    for (int i = 0; i < 10; ++i) {
        RPCRequest request(totalTasks + i, "Future-" + std::to_string(i));
        futures.push_back(executor.submitWithFuture(std::move(request)));
    }
    
    // 等待所有客户端线程完成
    for (auto& thread : clientThreads) {
        thread.join();
    }
    
    // 处理future结果
    for (auto& future : futures) {
        try {
            RPCResponse response = future.get();
            std::cout << "[Future] 任务ID: " << response.id 
                      << ", 结果: " << response.result << std::endl;
            completedTasks++;
        }
        catch (const std::exception& e) {
            std::cerr << "Future异常: " << e.what() << std::endl;
        }
    }
    
    // 等待所有任务完成
    while (completedTasks < totalTasks + futures.size()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    
    auto endTime = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
    
    std::cout << "所有任务完成! 耗时: " << duration.count() << "ms" << std::endl;
    std::cout << "平均单任务耗时: " << static_cast<double>(duration.count()) / (totalTasks + futures.size()) << "ms" << std::endl;
    
    // 展示对比传统逐个处理的方式
    std::cout << "\n传统逐个处理方式模拟:" << std::endl;
    startTime = std::chrono::steady_clock::now();
    
    // 修改为处理相同数量的任务(totalTasks + futures.size())
    for (int i = 0; i < totalTasks + futures.size(); ++i) {
        RPCRequest request(i, "单个-" + std::to_string(i));
        std::vector<RPCRequest> singleBatch = {request};
        auto responses = mockRpcService(singleBatch);
        
        // 只打印部分结果，避免输出过多
        // if (i < 10 || i >= totalTasks + futures.size() - 5) {
        //     std::cout << "[单个] 任务ID: " << i << ", 结果: " << responses[0].result << std::endl;
        // } else if (i == 10) {
        //     std::cout << "... (省略中间日志) ..." << std::endl;
        // }
    }
    
    endTime = std::chrono::steady_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
    std::cout << "逐个处理 " << (totalTasks + futures.size()) << " 个任务耗时: " << duration.count() << "ms" << std::endl;
    std::cout << "平均单任务耗时: " << static_cast<double>(duration.count()) / (totalTasks + futures.size()) << "ms" << std::endl;
    
    // 安全关闭
    executor.shutdown();
    
    return 0;
}